/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/28 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "TemperatureService.h"
#include "dao/temperature/TemperatureDAO.h"
#include "domain/do/nursing/VitalSignDO.h"
#include "domain/do/nursing/VitalSignDetailDO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <ctime>


static const std::vector<std::string> TIME_SLOTS = {
    "02", "06", "10", "14", "18", "22"
};


static constexpr const char* IDEMPOTENCY_ITEM_CODE = "_IDEMPOTENCY_KEY";
static const std::string TEMPERATURE_RESULT_MARKER = "\n--RESULT--\n";

template<class DTO>
static std::string temperatureFingerprint(const oatpp::Object<DTO>& dto)
{
	return oatpp::parser::json::mapping::ObjectMapper::createShared()
		->writeToString(dto).getValue("");
}

template<class ResponseDTO>
static oatpp::Object<ResponseDTO> restoreTemperatureResult(
	const std::string& stored, const std::string& fingerprint)
{
	const auto position = stored.find(TEMPERATURE_RESULT_MARKER);
	if (position == std::string::npos
		|| stored.substr(0, position) != fingerprint) return nullptr;
	return oatpp::parser::json::mapping::ObjectMapper::createShared()
		->readFromString<oatpp::Object<ResponseDTO>>(
			stored.substr(position + TEMPERATURE_RESULT_MARKER.size()).c_str());
}

static std::string computeNextDate(const std::string& dateStr);
static std::string computePrevDate(const std::string& dateStr);

static bool dateToTime(const std::string& value, std::time_t& result)
{
	if (value.size() < 10) return false;
	std::tm tm{};
	tm.tm_year = std::stoi(value.substr(0, 4)) - 1900;
	tm.tm_mon = std::stoi(value.substr(5, 2)) - 1;
	tm.tm_mday = std::stoi(value.substr(8, 2));
	tm.tm_hour = 12;
	result = std::mktime(&tm);
	return result != static_cast<std::time_t>(-1);
}

static int daysBetween(const std::string& start, const std::string& end)
{
	std::time_t left = 0;
	std::time_t right = 0;
	if (!dateToTime(start, left) || !dateToTime(end, right)) return 0;
	return static_cast<int>(std::difftime(right, left) / (24 * 60 * 60));
}

static std::string addDays(std::string date, int days)
{
	while (days > 0)
	{
		date = computeNextDate(date);
		--days;
	}
	while (days < 0)
	{
		date = computePrevDate(date);
		++days;
	}
	return date;
}

static bool isFixedTimeSlot(const std::string& slot)
{
	return std::find(TIME_SLOTS.begin(), TIME_SLOTS.end(), slot) != TIME_SLOTS.end();
}

static std::string normalizedCode(const oatpp::String& value)
{
	if (!value) return {};
	std::string code = value->c_str();
	std::transform(code.begin(), code.end(), code.begin(),
		[](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
	return code;
}

static bool parseNumber(const oatpp::String& value, double& parsed)
{
	if (!value || value->empty()) return false;
	try
	{
		std::size_t used = 0;
		parsed = std::stod(value->c_str(), &used);
		return used == value->size() && std::isfinite(parsed);
	}
	catch (...) { return false; }
}

static bool withinRange(const oatpp::String& value, double minimum, double maximum)
{
	double parsed = 0;
	return parseNumber(value, parsed) && parsed >= minimum && parsed <= maximum;
}

static bool validBloodPressure(const std::string& value)
{
	const auto separator = value.find('/');
	if (separator == std::string::npos || separator == 0 || separator + 1 >= value.size())
		return false;
	try
	{
		std::size_t leftUsed = 0;
		std::size_t rightUsed = 0;
		const double systolic = std::stod(value.substr(0, separator), &leftUsed);
		const double diastolic = std::stod(value.substr(separator + 1), &rightUsed);
		return leftUsed == separator && rightUsed == value.size() - separator - 1
			&& systolic >= 50 && systolic <= 260
			&& diastolic >= 30 && diastolic <= 160
			&& systolic > diastolic;
	}
	catch (...) { return false; }
}

static bool validateTemperatureDetails(
	const oatpp::List<TemperatureSheetDetailItemDTO::Wrapper>& items,
	const std::string& timeSlot, bool dailySupplement, std::string& errorCode)
{
	if (!items || items->empty())
	{
		errorCode = "BLANK_RECORD";
		return false;
	}
	bool hasCooledTemperature = false;
	bool hasTemperature = false;
	bool coolingEvent = false;
	bool eventTimeRequired = false;
	bool hasEventTime = false;
	for (const auto& item : *items)
	{
		if (!item || !item->itemCode || item->itemCode->empty())
		{
			errorCode = "INVALID_ITEM";
			return false;
		}
		const std::string code = normalizedCode(item->itemCode);
		const std::string value = item->itemValue.getValue("");
		static const std::set<std::string> allowedCodes = {
			"TEMP", "COOLED_TEMP", "PULSE", "HEART_RATE", "HR",
			"RESPIRATION", "RESP", "BLOOD_PRESSURE", "BP",
			"SPECIAL_EVENT", "EVENT_TIME", "WEIGHT", "HEIGHT", "WAIST",
			"ABDOMINAL_CIRCUMFERENCE", "STOOL", "URINE_FREQ", "INTAKE",
			"URINE_VOL", "OTHER_OUTPUT", "OTHER"
		};
		if (allowedCodes.find(code) == allowedCodes.end())
		{
			errorCode = "UNSUPPORTED_ITEM_CODE";
			return false;
		}
		if (dailySupplement && timeSlot != "22")
		{
			errorCode = "DAILY_SUPPLEMENT_SLOT_INVALID";
			return false;
		}
		if ((code == "TEMP" || code == "COOLED_TEMP")
			&& !withinRange(item->itemValue, 34, 42))
		{
			errorCode = "TEMPERATURE_OUT_OF_RANGE";
			return false;
		}
		if ((code == "PULSE" || code == "HEART_RATE" || code == "HR")
			&& !withinRange(item->itemValue, 20, 250))
		{
			errorCode = "PULSE_OUT_OF_RANGE";
			return false;
		}
		if ((code == "RESPIRATION" || code == "RESP")
			&& !withinRange(item->itemValue, 5, 80))
		{
			errorCode = "RESPIRATION_OUT_OF_RANGE";
			return false;
		}
		if ((code == "BLOOD_PRESSURE" || code == "BP") && !validBloodPressure(value))
		{
			errorCode = "BLOOD_PRESSURE_INVALID";
			return false;
		}
		if (code == "WEIGHT" && !withinRange(item->itemValue, 0.1, 500))
		{
			errorCode = "WEIGHT_OUT_OF_RANGE";
			return false;
		}
		if (code == "HEIGHT" && !withinRange(item->itemValue, 20, 250))
		{
			errorCode = "HEIGHT_OUT_OF_RANGE";
			return false;
		}
		if ((code == "WAIST" || code == "ABDOMINAL_CIRCUMFERENCE")
			&& !withinRange(item->itemValue, 10, 300))
		{
			errorCode = "ABDOMINAL_CIRCUMFERENCE_OUT_OF_RANGE";
			return false;
		}
		if ((code == "INTAKE" || code == "URINE_VOL" || code == "OTHER_OUTPUT")
			&& !withinRange(item->itemValue, 0, 100000))
		{
			errorCode = "VOLUME_OUT_OF_RANGE";
			return false;
		}
		if ((code == "STOOL" || code == "URINE_FREQ")
			&& !withinRange(item->itemValue, 0, 100))
		{
			errorCode = "COUNT_OUT_OF_RANGE";
			return false;
		}
		if (code == "TEMP") hasTemperature = true;
		if (code == "COOLED_TEMP") hasCooledTemperature = true;
		if (code == "EVENT_TIME")
		{
			hasEventTime = value.size() == 5 && value[2] == ':'
				&& std::isdigit(static_cast<unsigned char>(value[0]))
				&& std::isdigit(static_cast<unsigned char>(value[1]))
				&& std::isdigit(static_cast<unsigned char>(value[3]))
				&& std::isdigit(static_cast<unsigned char>(value[4]))
				&& std::stoi(value.substr(0, 2)) < 24
				&& std::stoi(value.substr(3, 2)) < 60;
			if (!hasEventTime)
			{
				errorCode = "EVENT_TIME_INVALID";
				return false;
			}
		}
		if (code == "SPECIAL_EVENT")
		{
			static const std::set<std::string> allowedEvents = {
				"TRANSFER", "SURGERY", "DELIVERY", "DISCHARGE", "DEATH",
				"OUT_EXAM", "REFUSED", "PHYSICAL_COOLING", "DRUG_COOLING",
				"TEMP_NOT_RISING", "PULSE_DEFICIT"
			};
			if (allowedEvents.find(value) == allowedEvents.end())
			{
				errorCode = value == "ADMISSION"
					? "ADMISSION_EVENT_MANAGED_BY_SERVER" : "SPECIAL_EVENT_INVALID";
				return false;
			}
			coolingEvent = value == "PHYSICAL_COOLING" || value == "DRUG_COOLING";
			eventTimeRequired = value == "TRANSFER" || value == "DELIVERY"
				|| value == "DISCHARGE" || value == "DEATH";
		}
	}
	if (coolingEvent && (!hasTemperature || !hasCooledTemperature))
	{
		errorCode = "COOLED_TEMPERATURE_REQUIRED";
		return false;
	}
	if (eventTimeRequired && !hasEventTime)
	{
		errorCode = "EVENT_TIME_REQUIRED";
		return false;
	}
	if (!coolingEvent && hasCooledTemperature)
	{
		errorCode = "COOLING_EVENT_REQUIRED";
		return false;
	}
	errorCode.clear();
	return true;
}


static std::string extractHour(const std::string& recordedAt)
{
    if (recordedAt.length() >= 13)
        return recordedAt.substr(11, 2);
    return "";
}


static std::string extractDate(const std::string& recordedAt)
{
    if (recordedAt.length() >= 10)
        return recordedAt.substr(0, 10);
    return recordedAt;
}


static TemperatureSheetDetailItemDTO::Wrapper mapDetailToItem(const VitalSignDetailDO& d)
{
    auto item = TemperatureSheetDetailItemDTO::createShared();
    item->itemCode  = d.getItemCode();
    item->itemValue = d.getItemValue();
    item->remark    = d.getRemark();
    item->termCode  = d.getTermCode();
    return item;
}


static TemperatureSheetTemperaturePointDTO::Wrapper buildTemperaturePoint(const VitalSignDO& vs,
                                                                           const std::list<VitalSignDetailDO>& details)
{
    auto pt = TemperatureSheetTemperaturePointDTO::createShared();
    pt->vitalSignId = vs.getId();
    pt->recordedAt  = vs.getRecordedAt();
	pt->version = static_cast<v_int64>(vs.getVersion());

	for (auto& d : details)
    {
		const auto& code = d.getItemCode();
		if (code == IDEMPOTENCY_ITEM_CODE) continue;
		if (code == "TEMP") pt->temperature = d.getItemValue();
		else if (code == "COOLED_TEMP") pt->cooledTemperature = d.getItemValue();
		else if (code == "PULSE") pt->pulse = d.getItemValue();
		else if (code == "HEART_RATE") pt->heartRate = d.getItemValue();
		else if (code == "RESPIRATION") pt->respiration = d.getItemValue();
		else if (code == "BLOOD_PRESSURE") pt->bloodPressure = d.getItemValue();
		else if (code == "SPECIAL_EVENT") pt->specialEvent = d.getItemValue();
		else if (d.getTermCode() == "special_event_code") pt->specialEvent = code;
    }
	pt->version = vs.getVersion();
    return pt;
}

static std::string computeNextDate(const std::string& dateStr)
{
	if (dateStr.length() < 10) return dateStr;
	int year = std::stoi(dateStr.substr(0, 4));
	int month = std::stoi(dateStr.substr(5, 2));
	int day = std::stoi(dateStr.substr(8, 2));
	static const int days[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	int maximum = days[month];
	if (month == 2 && (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))) maximum = 29;
	if (++day > maximum)
	{
		day = 1;
		if (++month > 12) { month = 1; ++year; }
	}
	std::stringstream result;
	result << year << "-" << std::setw(2) << std::setfill('0') << month
		<< "-" << std::setw(2) << std::setfill('0') << day;
	return result.str();
}

static bool sameDetails(const std::list<VitalSignDetailDO>& existing,
	const oatpp::List<TemperatureSheetDetailItemDTO::Wrapper>& submitted)
{
	std::map<std::string, std::string> left;
	std::map<std::string, std::string> right;
	for (const auto& detail : existing)
		if (detail.getItemCode() != IDEMPOTENCY_ITEM_CODE)
		{
			std::string code = detail.getItemCode();
			std::transform(code.begin(), code.end(), code.begin(),
				[](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
			left[code] = detail.getItemValue();
		}
	if (submitted)
	{
		for (const auto& item : *submitted)
			if (item && item->itemCode)
				right[normalizedCode(item->itemCode)] = item->itemValue.getValue("");
	}
	return left == right;
}

static bool hasIdempotencyKey(
	const std::list<VitalSignDetailDO>& details, const std::string& key)
{
	return std::any_of(details.begin(), details.end(), [&](const VitalSignDetailDO& detail) {
		return detail.getItemCode() == IDEMPOTENCY_ITEM_CODE
			&& detail.getItemValue() == key;
	});
}

static TemperatureInitOptionDTO::Wrapper makeInitOption(const std::string& code,
	const std::string& name, const std::string& unit, const std::string& range, int order)
{
	auto item = TemperatureInitOptionDTO::createShared();
	item->code = code;
	item->name = name;
	item->unit = unit;
	item->reasonableRange = range;
	item->displayOrder = order;
	item->eventTimeRequired = false;
	item->verticalDisplay = false;
	item->cooledTemperatureRequired = false;
	return item;
}

TemperatureInitResponseDTO::Wrapper TemperatureService::getInitConfig()
{
	auto response = TemperatureInitResponseDTO::createShared();
	response->windowRanges->push_back("3d");
	response->windowRanges->push_back("7d");
	response->windowRanges->push_back("14d");
	for (size_t index = 0; index < TIME_SLOTS.size(); ++index)
		response->timeSlots->push_back(makeInitOption(TIME_SLOTS[index], TIME_SLOTS[index], "", "", index + 1));

	const std::vector<std::pair<std::string, std::string>> events = {
		{"ADMISSION", ZH_WORDS_GETTER("temperature.option.event.admission")},
		{"TRANSFER", ZH_WORDS_GETTER("temperature.option.event.transfer")},
		{"SURGERY", ZH_WORDS_GETTER("temperature.option.event.surgery")},
		{"DELIVERY", ZH_WORDS_GETTER("temperature.option.event.delivery")},
		{"DISCHARGE", ZH_WORDS_GETTER("temperature.option.event.discharge")},
		{"DEATH", ZH_WORDS_GETTER("temperature.option.event.death")},
		{"OUT_EXAM", ZH_WORDS_GETTER("temperature.option.event.outExam")},
		{"REFUSED", ZH_WORDS_GETTER("temperature.option.event.refused")},
		{"PHYSICAL_COOLING", ZH_WORDS_GETTER("temperature.option.event.physicalCooling")},
		{"DRUG_COOLING", ZH_WORDS_GETTER("temperature.option.event.drugCooling")},
		{"TEMP_NOT_RISING", ZH_WORDS_GETTER("temperature.option.event.tempNotRising")},
		{"PULSE_DEFICIT", ZH_WORDS_GETTER("temperature.option.event.pulseDeficit")}
	};
	for (size_t index = 0; index < events.size(); ++index)
	{
		auto event = makeInitOption(events[index].first, events[index].second, "", "", index + 1);
		event->eventTimeRequired = events[index].first == "TRANSFER" || events[index].first == "DELIVERY"
			|| events[index].first == "DISCHARGE" || events[index].first == "DEATH";
		event->verticalDisplay = true;
		event->cooledTemperatureRequired = events[index].first == "PHYSICAL_COOLING"
			|| events[index].first == "DRUG_COOLING";
		response->specialEvents->push_back(event);
	}
	response->vitalSignItems->push_back(makeInitOption("TEMP", ZH_WORDS_GETTER("temperature.option.item.temperature"), "C", "34-42", 1));
	response->vitalSignItems->push_back(makeInitOption("COOLED_TEMP", ZH_WORDS_GETTER("temperature.option.item.cooledTemperature"), "C", "34-42", 2));
	response->vitalSignItems->push_back(makeInitOption("PULSE", ZH_WORDS_GETTER("temperature.option.item.pulse"), "bpm", "20-250", 3));
	response->vitalSignItems->push_back(makeInitOption("HEART_RATE", ZH_WORDS_GETTER("temperature.option.item.heartRate"), "bpm", "20-250", 4));
	response->vitalSignItems->push_back(makeInitOption("RESPIRATION", ZH_WORDS_GETTER("temperature.option.item.respiration"), "breaths/min", "5-80", 5));
	response->vitalSignItems->push_back(makeInitOption("BLOOD_PRESSURE", ZH_WORDS_GETTER("temperature.option.item.bloodPressure"), "mmHg", "", 6));
	response->vitalSignItems->push_back(makeInitOption("WEIGHT", ZH_WORDS_GETTER("temperature.option.item.weight"), "kg", "0.1-500", 7));
	response->vitalSignItems->push_back(makeInitOption("HEIGHT", ZH_WORDS_GETTER("temperature.option.item.height"), "cm", "20-250", 8));
	response->vitalSignItems->push_back(makeInitOption("WAIST", ZH_WORDS_GETTER("temperature.option.item.waist"), "cm", "10-300", 9));
	response->vitalSignItems->push_back(makeInitOption("STOOL", ZH_WORDS_GETTER("temperature.option.item.stool"), "times", "0-100", 10));
	response->vitalSignItems->push_back(makeInitOption("URINE_FREQ", ZH_WORDS_GETTER("temperature.option.item.urineFrequency"), "times", "0-100", 11));
	response->vitalSignItems->push_back(makeInitOption("INTAKE", ZH_WORDS_GETTER("temperature.option.item.intake"), "ml", "0-100000", 12));
	response->vitalSignItems->push_back(makeInitOption("URINE_VOL", ZH_WORDS_GETTER("temperature.option.item.urineVolume"), "ml", "0-100000", 13));
	response->vitalSignItems->push_back(makeInitOption("OTHER_OUTPUT", ZH_WORDS_GETTER("temperature.option.item.otherOutput"), "ml", "0-100000", 14));
	return response;
}

static std::string computePrevDate(const std::string& dateStr)
{
    if (dateStr.length() < 10) return dateStr;
    int y = std::stoi(dateStr.substr(0, 4));
    int m = std::stoi(dateStr.substr(5, 2));
    int d = std::stoi(dateStr.substr(8, 2));

    auto daysInMonth = [](int year, int month) -> int {
        static const int mdays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        int d = mdays[month - 1];
        if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) d = 29;
        return d;
    };

    d--;
    if (d < 1) { m--; if (m < 1) { m = 12; y--; } d = daysInMonth(y, m); }

    std::stringstream ss;
    ss << y << "-" << std::setw(2) << std::setfill('0') << m
       << "-" << std::setw(2) << std::setfill('0') << d;
    return ss.str();
}


std::string TemperatureService::generateBusNo(TemperatureDAO& dao)
{
    std::string today = SimpleDateTimeFormat::format();
    std::string datePart = today.substr(0, 10);
    // Remove dashes: "2026-07-28" → "20260728"
    datePart.erase(std::remove(datePart.begin(), datePart.end(), '-'), datePart.end());

    // Count today's records to derive the sequence number
    uint64_t cnt = dao.countByBusNoPrefix("TS" + datePart);

    std::stringstream busNo;
    busNo << "TS" << datePart << std::setw(3) << std::setfill('0') << (cnt + 1);
    return busNo.str();
}


TemperatureSheetGetDataResponseDTO::Wrapper TemperatureService::getTemperatureData(
    const TemperatureSheetGetDataRequestDTO::Wrapper& query)
{
    auto resp = TemperatureSheetGetDataResponseDTO::createShared();
	if (!query || !query->encounterId || query->encounterId->empty()
		|| !query->patientId || query->patientId->empty()) return resp;

	const std::string encId = query->encounterId.getValue("");
	const std::string patId = query->patientId.getValue("");
	const std::string requestedFocus = query->focusRecordDate.getValue("");
	const std::string originalFocus = requestedFocus.empty()
		? SimpleDateTimeFormat::format().substr(0, 10) : requestedFocus;
	const std::string winRange = query->windowRange.getValue("7d");
	const int windowDays = std::stoi(winRange.substr(0, winRange.size() - 1));

	TemperatureDAO dao;
	auto header = dao.selectPaperHeader(encId, patId);
	const std::string today = SimpleDateTimeFormat::format().substr(0, 10);
	std::string admissionDate = header->admitAt.size() >= 10
		? header->admitAt.substr(0, 10) : today;
	std::string endAnchor = header->dischargeAt.size() >= 10
		? header->dischargeAt.substr(0, 10) : today;
	if (endAnchor > today) endAnchor = today;
	if (endAnchor < admissionDate) endAnchor = admissionDate;
	const int totalDays = std::max(1, daysBetween(admissionDate, endAnchor) + 1);
	const int totalPages = std::max(1, (totalDays + windowDays - 1) / windowDays);
	int requestedPage = query->pageIndex.getValue(0);
	if (!requestedFocus.empty())
	{
		const std::string boundedFocus = std::min(
			std::max(requestedFocus, admissionDate), endAnchor);
		requestedPage = std::max(0, daysBetween(admissionDate, boundedFocus) / windowDays);
	}
	const int pageIndex = std::min(requestedPage, totalPages - 1);
	const std::string dateFrom = addDays(admissionDate, pageIndex * windowDays);
	std::string dateTo = addDays(dateFrom, windowDays - 1);
	if (dateTo > endAnchor) dateTo = endAnchor;

	auto vitalSigns = dao.selectByCondition(encId, patId, dateFrom, dateTo + " 23:59:59");
	std::list<std::string> ids;
	for (const auto& vitalSign : vitalSigns) ids.push_back(vitalSign.getId());
	auto allDetails = dao.selectByVitalSignIds(ids);
	std::map<std::string, std::list<VitalSignDetailDO>> detailMap;
	for (auto& detail : allDetails) detailMap[detail.getVitalSignId()].push_back(std::move(detail));
	std::map<std::string, std::map<std::string, const VitalSignDO*>> grid;
	for (const auto& vitalSign : vitalSigns)
		grid[extractDate(vitalSign.getRecordedAt())][extractHour(vitalSign.getRecordedAt())] = &vitalSign;

	auto gridRows = oatpp::List<TemperatureSheetGridRowDTO::Wrapper>::createShared();
	auto fillSlots = oatpp::List<TemperatureSheetFillSlotDTO::Wrapper>::createShared();
	auto tempPoints = oatpp::List<TemperatureSheetTemperaturePointDTO::Wrapper>::createShared();
	for (const auto& vitalSign : vitalSigns)
		tempPoints->push_back(buildTemperaturePoint(vitalSign, detailMap[vitalSign.getId()]));
	if (header->admitAt.size() >= 10
		&& header->admitAt.substr(0, 10) >= dateFrom
		&& header->admitAt.substr(0, 10) <= dateTo)
	{
		auto admissionPoint = TemperatureSheetTemperaturePointDTO::createShared();
		admissionPoint->recordedAt = header->admitAt;
		admissionPoint->specialEvent = "ADMISSION";
		admissionPoint->version = static_cast<v_int64>(0);
		tempPoints->push_back(admissionPoint);
	}

	for (std::string date = dateFrom;; date = computeNextDate(date))
	{
		auto row = TemperatureSheetGridRowDTO::createShared();
		row->recordDate = date;
		row->slotCells = oatpp::List<TemperatureSheetGridSlotCellDTO::Wrapper>::createShared();
		for (const auto& slot : TIME_SLOTS)
		{
			auto cell = TemperatureSheetGridSlotCellDTO::createShared();
			cell->timeSlot = slot;
			cell->detailItems = oatpp::List<TemperatureSheetDetailItemDTO::Wrapper>::createShared();
			auto dateIt = grid.find(date);
			if (dateIt != grid.end())
			{
				auto slotIt = dateIt->second.find(slot);
				if (slotIt != dateIt->second.end())
				{
					const auto* vitalSign = slotIt->second;
					cell->vitalSignId = vitalSign->getId();
					cell->version = vitalSign->getVersion();
		for (const auto& detail : detailMap[vitalSign->getId()])
						if (detail.getItemCode() != IDEMPOTENCY_ITEM_CODE)
							cell->detailItems->push_back(mapDetailToItem(detail));
				}
			}
			row->slotCells->push_back(cell);
			if (date == originalFocus)
			{
				auto fill = TemperatureSheetFillSlotDTO::createShared();
				fill->timeSlot = slot;
				fill->vitalSignId = cell->vitalSignId;
				fill->version = cell->version;
				fill->detailItems = cell->detailItems;
				fillSlots->push_back(fill);
			}
		}
		gridRows->push_back(row);
		if (date == dateTo) break;
	}

	auto pagination = TemperatureSheetPaginationDTO::createShared();
	pagination->dateFrom = dateFrom;
	pagination->dateTo = dateTo;
	pagination->pageIndex = pageIndex;
	pagination->totalPages = totalPages;
	auto meta = TemperatureSheetDerivedMetaDTO::createShared();
	meta->hospitalizationDaysByDate = oatpp::Fields<oatpp::Int32>::createShared();
	meta->postoperativeDaysByDate = oatpp::Fields<oatpp::Int32>::createShared();
	meta->weekNoByDate = oatpp::Fields<oatpp::Int32>::createShared();
	const auto surgeryDates = dao.selectSurgeryDates(encId);
	for (std::string date = dateFrom;; date = computeNextDate(date))
	{
		const int dayNumber = std::max(1, daysBetween(admissionDate, date) + 1);
		meta->hospitalizationDaysByDate[date.c_str()] = dayNumber;
		meta->weekNoByDate[date.c_str()] = ((dayNumber - 1) / 7) + 1;
		std::string latestSurgery;
		for (const auto& surgeryDate : surgeryDates)
			if (surgeryDate <= date && (latestSurgery.empty() || surgeryDate > latestSurgery))
				latestSurgery = surgeryDate;
		if (!latestSurgery.empty())
			meta->postoperativeDaysByDate[date.c_str()] = daysBetween(latestSurgery, date);
		if (date == dateTo) break;
	}
	resp->paperHeader = oatpp::Fields<oatpp::String>::createShared();
	resp->paperHeader["inpatientNo"] = header->inpatientNo;
	resp->paperHeader["name"] = header->patientName;
	resp->paperHeader["gender"] = header->gender;
	resp->paperHeader["age"] = header->age;
	resp->paperHeader["admitAt"] = header->admitAt;
	resp->paperHeader["dischargeAt"] = header->dischargeAt;
	resp->paperHeader["wardName"] = header->wardName;
	resp->paperHeader["bedNo"] = header->bedNo;
	resp->derivedMeta = meta;
	resp->gridRows = gridRows;
	resp->temperaturePoints = tempPoints;
	resp->fillSlots = fillSlots;
	resp->pagination = pagination;
    return resp;
}


TemperatureSheetSaveResponseDTO::Wrapper TemperatureService::saveTemperatureRecord(
    const TemperatureSheetSaveRequestDTO::Wrapper& dto, const std::string& operatorId)
{
    auto resp = TemperatureSheetSaveResponseDTO::createShared();
	resp->vitalSignId = "";
	resp->busNo = "";
	resp->nextTimeSlot = "";
	resp->version = -1;
	resp->errorCode = "INVALID_REQUEST";
	if (!dto || !dto->encounterId || dto->encounterId->empty()
		|| !dto->patientId || dto->patientId->empty()
		|| !dto->recordDate || dto->recordDate->empty()
		|| !dto->timeSlot || dto->timeSlot->empty()
		|| !dto->detailItems || dto->detailItems->empty()) return resp;

    std::string encId      = dto->encounterId  ? dto->encounterId.getValue("")  : "";
    std::string patId      = dto->patientId    ? dto->patientId.getValue("")    : "";
    std::string recordDate = dto->recordDate   ? dto->recordDate.getValue("")   : "";
    std::string timeSlot   = dto->timeSlot     ? dto->timeSlot.getValue("")     : "";
	std::string vitalSignId = dto->vitalSignId ? dto->vitalSignId.getValue("")  : "";
	std::string validationError;
	if (!isFixedTimeSlot(timeSlot)
		|| !validateTemperatureDetails(dto->detailItems, timeSlot, false, validationError))
	{
		resp->errorCode = validationError.empty() ? "TIME_SLOT_INVALID" : validationError;
		return resp;
	}

	std::string createdBy  = operatorId;
	std::string createdAt  = SimpleDateTimeFormat::format();
	const std::string recordedAt = recordDate + " " + timeSlot + ":00:00";
	std::string busNo;
    UuidFacade uf(1);

	TemperatureDAO dao;
	auto encounterHeader = dao.selectPaperHeader(encId, patId);
	const std::string today = SimpleDateTimeFormat::format().substr(0, 10);
	if ((encounterHeader->admitAt.size() >= 10
			&& recordDate < encounterHeader->admitAt.substr(0, 10))
		|| recordDate > today || recordedAt > createdAt
		|| (encounterHeader->dischargeAt.size() >= 10
			&& recordDate > encounterHeader->dischargeAt.substr(0, 10)))
	{
		resp->errorCode = "DATE_OUTSIDE_ENCOUNTER";
		return resp;
	}

    TransactionManager tm(&dao);

    try
    {
        auto existing = dao.selectBySlot(encId, patId, recordDate, timeSlot);
		if (!existing.empty() && dto->idempotencyKey && !dto->idempotencyKey->empty())
		{
			auto details = dao.selectByVitalSignIds({existing.front().getId()});
			if (hasIdempotencyKey(details, dto->idempotencyKey.getValue("")))
			{
				if (!sameDetails(details, dto->detailItems))
					throw std::runtime_error("IDEMPOTENCY_CONFLICT");
				tm.commit();
				resp->vitalSignId = existing.front().getId();
				resp->busNo = existing.front().getBusNo();
				resp->version = existing.front().getVersion();
				resp->errorCode = "";
				return resp;
			}
		}
		bool updateExisting = !vitalSignId.empty();
		if (updateExisting)
        {
			if (existing.empty() || existing.front().getId() != vitalSignId
				|| !dto->expectedVersion.getPtr())
				throw std::runtime_error("VERSION_CONFLICT");
			VitalSignDO data;
			data.setId(existing.front().getId());
			data.setEncounterId(existing.front().getEncounterId());
			data.setPatientId(existing.front().getPatientId());
			data.setRecordedAt(recordedAt);
			data.setRecorderId(operatorId);
			data.setUpdatedBy(operatorId);
			data.setRemark(existing.front().getRemark());
			if (dao.updateMainRecord(data, dto->expectedVersion.getValue(-1)) != 1)
				throw std::runtime_error("VERSION_CONFLICT");
			if (dao.deleteByVitalSignId(vitalSignId) < 0)
				throw std::runtime_error("DATABASE_ERROR");
			busNo = existing.front().getBusNo();
			resp->version = dto->expectedVersion.getValue(0) + 1;
		}
		else
		{
			if (!existing.empty())
            {
				auto details = dao.selectByVitalSignIds({existing.front().getId()});
				throw std::runtime_error("SLOT_CONFLICT");
            }
			VitalSignDO data;
			data.setId(uf.genUuid());
			vitalSignId = data.getId();
			data.setPatientId(patId);
			data.setEncounterId(encId);
			data.setRecordedAt(recordedAt);
			data.setTypeEnum("TEMPERATURE_SHEET");
			busNo = generateBusNo(dao);
			data.setBusNo(busNo);
			data.setCreatedBy(createdBy);
			data.setCreatedAt(createdAt);
			data.setRecorderId(operatorId);
			data.setVersion(0);
			data.setIsDeleted(0);
			if (dao.insert(data) != 1) throw std::runtime_error("DATABASE_ERROR");
			resp->version = static_cast<v_int64>(0);
		}

        std::list<VitalSignDetailDO> details;
        if (dto->detailItems)
        {
            for (auto& item : *dto->detailItems.get())
            {
                if (!item) continue;
				VitalSignDetailDO& detail = details.emplace_back();
                detail.setId(uf.genUuid());
                detail.setVitalSignId(vitalSignId);
				detail.setTermCode(item->termCode && !item->termCode->empty()
					? item->termCode.getValue("") : "vital_signs_code");
				detail.setItemCode(normalizedCode(item->itemCode));
                detail.setItemValue(item->itemValue ? item->itemValue.getValue("") : "");
                detail.setRemark(item->remark ? item->remark.getValue("") : "");
                detail.setCreatedBy(createdBy);
                detail.setCreatedAt(createdAt);
                detail.setVersion(0);
                detail.setIsDeleted(0);
            }
        }
		if (dto->idempotencyKey && !dto->idempotencyKey->empty())
		{
			VitalSignDetailDO& detail = details.emplace_back();
			detail.setId(uf.genUuid());
			detail.setVitalSignId(vitalSignId);
			detail.setTermCode("system_internal");
			detail.setItemCode(IDEMPOTENCY_ITEM_CODE);
			detail.setItemValue(dto->idempotencyKey.getValue(""));
			detail.setCreatedBy(createdBy);
			detail.setCreatedAt(createdAt);
			detail.setVersion(0);
			detail.setIsDeleted(0);
		}

		if (details.empty() || dao.insertBatch(details) != static_cast<int>(details.size()))
			throw std::runtime_error("Temperature detail insert failed.");

        tm.commit();
    }
    catch (const std::runtime_error& error)
    {
        tm.rollback();
        resp->vitalSignId = "";
        resp->busNo = "";
        resp->nextTimeSlot = "";
		resp->errorCode = error.what();
        return resp;
    }
	catch (...)
	{
		tm.rollback();
		resp->errorCode = "DATABASE_ERROR";
		return resp;
	}

    std::string nextSlot;
    for (size_t i = 0; i < TIME_SLOTS.size(); i++)
    {
        if (TIME_SLOTS[i] == timeSlot && i + 1 < TIME_SLOTS.size())
        {
            nextSlot = TIME_SLOTS[i + 1];
            break;
        }
    }

    resp->vitalSignId  = vitalSignId;
    resp->busNo        = busNo;
    resp->nextTimeSlot = nextSlot;
	resp->errorCode = "";
    return resp;
}

TemperatureSheetBatchSaveResponseDTO::Wrapper TemperatureService::saveTemperatureRecords(
	const TemperatureSheetBatchSaveRequestDTO::Wrapper& dto, const std::string& operatorId)
{
	auto response = TemperatureSheetBatchSaveResponseDTO::createShared();
	if (!dto || !dto->slots) return response;
	TemperatureDAO auditDao;
	const auto fingerprint = temperatureFingerprint(dto);
	const auto storedResults = auditDao.selectIdempotencyResult(
		"SAVE_BATCH", dto->idempotencyKey.getValue(""));
	if (!storedResults.empty())
	{
		auto restored = restoreTemperatureResult<TemperatureSheetBatchSaveResponseDTO>(
			storedResults.front(), fingerprint);
		return restored ? restored : response;
	}
	for (const auto& slot : *dto->slots)
	{
		auto result = TemperatureSheetBatchSaveItemResultDTO::createShared();
		result->timeSlot = slot ? slot->timeSlot : "";
		result->success = false;
		result->errorCode = "INVALID_REQUEST";
		if (slot)
		{
			auto combinedDetails = oatpp::List<TemperatureSheetDetailItemDTO::Wrapper>::createShared();
			if (slot->detailItems)
				for (const auto& item : *slot->detailItems) combinedDetails->push_back(item);
			if (slot->timeSlot.getValue("") == "22" && dto->dailySupplement)
				for (const auto& item : *dto->dailySupplement) combinedDetails->push_back(item);
			auto request = TemperatureSheetSaveRequestDTO::createShared();
			request->encounterId = dto->encounterId;
			request->patientId = dto->patientId;
			request->recordDate = dto->recordDate;
			request->timeSlot = slot->timeSlot;
			request->vitalSignId = slot->vitalSignId;
			request->expectedVersion = slot->expectedVersion;
			request->idempotencyKey = dto->idempotencyKey;
			request->detailItems = combinedDetails;
			auto saved = saveTemperatureRecord(request, operatorId);
			if (saved && saved->vitalSignId && !saved->vitalSignId->empty())
			{
				result->success = true;
				result->vitalSignId = saved->vitalSignId;
				result->version = saved->version;
				result->nextTimeSlot = saved->nextTimeSlot;
				result->errorCode = "";
				result->message = ZH_WORDS_GETTER("business-text.temperature.save-success");
				result->savedCell = TemperatureSheetGridSlotCellDTO::createShared();
				result->savedCell->timeSlot = slot->timeSlot;
				result->savedCell->vitalSignId = saved->vitalSignId;
				result->savedCell->version = saved->version;
				result->savedCell->detailItems = combinedDetails;
				response->savedCount = response->savedCount.getValue(0) + 1;
			}
			else if (saved)
			{
				result->errorCode = saved->errorCode;
				result->message = saved->errorCode;
			}
		}
		response->results->push_back(result);
	}
	const auto resultData = oatpp::parser::json::mapping::ObjectMapper::createShared()
		->writeToString(response).getValue("");
	auditDao.insertIdempotencyResult(UuidFacade().genUuid(), "SAVE_BATCH",
		dto->idempotencyKey.getValue(""), operatorId, fingerprint, resultData);
	return response;
}
