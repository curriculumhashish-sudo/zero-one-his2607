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
#include "NursingService.h"
#include "dao/nursing/NursingDAO.h"
#include "domain/do/nursing/VitalSignDO.h"
#include "domain/do/nursing/VitalSignDetailDO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include <algorithm>
#include <cctype>
#include <map>
#include <sstream>

namespace
{
	const std::string NURSING_RESULT_MARKER = "\n--RESULT--\n";

	template<class DTO>
	std::string nursingFingerprint(const oatpp::Object<DTO>& dto)
	{
		return oatpp::parser::json::mapping::ObjectMapper::createShared()
			->writeToString(dto).getValue("");
	}

	template<class ResponseDTO>
	oatpp::Object<ResponseDTO> restoreNursingResult(const std::string& stored,
		const std::string& fingerprint)
	{
		const auto position = stored.find(NURSING_RESULT_MARKER);
		if (position == std::string::npos
			|| stored.substr(0, position) != fingerprint) return nullptr;
		return oatpp::parser::json::mapping::ObjectMapper::createShared()
			->readFromString<oatpp::Object<ResponseDTO>>(
				stored.substr(position + NURSING_RESULT_MARKER.size()).c_str());
	}
}


static void splitDateTime(const std::string& dt, std::string& date, std::string& time)
{
    if (dt.length() >= 16)
    {
        date = dt.substr(0, 10);
        time = dt.substr(11, 5);
    }
    else if (dt.length() >= 10)
    {
        date = dt.substr(0, 10);
    }
}


static std::string buildDateTime(const std::string& date, const std::string& time)
{
    if (date.length() >= 10)
        return date.substr(5) + " " + (time.empty() ? "00:00" : time);
    return date + " " + (time.empty() ? "00:00" : time);
}


static NursingRecordPaperHeaderDTO::Wrapper buildPaperHeader(const PaperHeaderData& d)
{
    auto h = NursingRecordPaperHeaderDTO::createShared();
    h->inpatientNo = d.inpatientNo;
    h->name        = d.patientName;
    h->gender      = d.gender;
    h->age         = std::to_string(d.age);
    h->admitAt     = d.admitAt;
    h->dischargeAt = d.dischargeAt;
    h->wardName    = d.wardName;
    h->bedNo       = d.bedNo;
    return h;
}


static void mapDetailToRow(const VitalSignDetailDO& d, NursingRecordRowDTO::Wrapper& row)
{
    std::string code = d.getItemCode();
    std::transform(code.begin(), code.end(), code.begin(),
        [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    const std::string& val = d.getItemValue();

    if (code == "TEMP" || code == "TEMPERATURE") row->temperature = val;
    else if (code == "PULSE") row->pulse = val;
    else if (code == "HR" || code == "HEART_RATE") row->heartRate = val;
    else if (code == "RESP" || code == "RESPIRATION")
    {
        row->respiration = val;
        row->respiratory = val;
    }
    else if (code == "BP" || code == "BLOOD_PRESSURE") row->bloodPressure = val;
    else if (code == "CONSCIOUSNESS") row->consciousness = val;
    else if (code == "PUPIL_L" || code == "PUPIL_LEFT") row->pupilLeft = val;
    else if (code == "PUPIL_R" || code == "PUPIL_RIGHT") row->pupilRight = val;
    else if (code == "OBSERVATION") row->observation = val;
    else if (code == "INTAKE_NAME") row->intakeName = val;
    else if (code == "INTAKE" || code == "INTAKE_VOLUME")
    {
        row->intakeVolume = val;
        row->intakeAmount = val;
    }
    else if (code == "OUTPUT_NAME") row->outputName = val;
    else if (code == "OUTPUT" || code == "OUTPUT_VOLUME")
    {
        row->outputVolume = val;
        row->outputAmount = val;
    }
    else if (code == "OUTPUT_COLOR") row->outputColor = val;
    else if (code == "NURSE_SIGNATURE") row->nurseSignature = val;
	else if (code == "SPO2" || code == "OXYGEN_SATURATION") row->oxygenSaturation = val;
	else if (code == "OXYGEN_FLOW") row->oxygenFlow = val;
	else if (code == "OXYGEN_MODE") row->oxygenMode = val;
	else if (code == "OXYGEN_RATE") row->oxygenRate = val;
	else if (code == "SKIN_STATUS") row->skinStatus = val;
	else if (code == "CONDITION_MEASURE") row->conditionMeasure = val;
	else if (code == "TUBE_CARE") row->tubeCare = val;
	else if (code == "INTAKE_ROUTE") row->intakeRoute = val;
	else if (code == "OUTPUT_CHARACTER" || code == "OUTPUT_COLOR_CHARACTER")
		row->outputColorCharacter = val;
}


static void copySaveItemToRow(const NursingRecordSaveItemDTO::Wrapper& src,
                              NursingRecordRowDTO::Wrapper& dst)
{
    dst->dateTime          = src->dateTime;
	dst->clientRowId = src->clientRowId;
    dst->vitalSignId       = src->vitalSignId;
    dst->recordDate        = src->recordDate;
    dst->recordTime        = src->recordTime;
    dst->temperature       = src->temperature;
    dst->pulse             = src->pulse;
    dst->heartRate         = src->heartRate;
    dst->respiration       = src->respiratory && !src->respiratory->empty()
        ? src->respiratory : src->respiration;
    dst->respiratory       = dst->respiration;
    dst->bloodPressure     = src->bloodPressure;
    dst->consciousness     = src->consciousness;
    dst->pupilLeft         = src->pupilLeft;
    dst->pupilRight        = src->pupilRight;
    dst->intakeName        = src->intakeName;
    dst->intakeVolume      = src->intakeAmount && !src->intakeAmount->empty()
        ? src->intakeAmount : src->intakeVolume;
    dst->intakeAmount      = dst->intakeVolume;
    dst->outputName        = src->outputName;
    dst->outputVolume      = src->outputAmount && !src->outputAmount->empty()
        ? src->outputAmount : src->outputVolume;
    dst->outputAmount      = dst->outputVolume;
    dst->outputColor       = src->outputColor;
    dst->observation       = src->observation;
    dst->nurseSignature    = src->nurseSignature;
	dst->oxygenSaturation = src->oxygenSaturation;
	dst->oxygenFlow = src->oxygenFlow;
	dst->oxygenMode = src->oxygenMode;
	dst->oxygenRate = src->oxygenRate;
	dst->skinStatus = src->skinStatus;
	dst->conditionMeasure = src->conditionMeasure;
	dst->tubeCare = src->tubeCare;
	dst->intakeRoute = src->intakeRoute;
	dst->outputColorCharacter = src->outputColorCharacter;
}


static void addDetailItem(std::list<VitalSignDetailDO>& details,
                          const std::string& vitalSignId,
                          const oatpp::String& val,
                          const char* itemCode, const char* units,
                          const std::string& createdBy, const std::string& createdAt,
                          UuidFacade& uf)
{
    if (!val || val->empty()) return;
    VitalSignDetailDO& detail = details.emplace_back();
    detail.setId(uf.genUuid());
    detail.setVitalSignId(vitalSignId);
    detail.setTermCode("vital_signs_code");
    detail.setItemCode(itemCode);
    detail.setItemValue(val.getValue(""));
    if (units) detail.setUnits(units);
    detail.setCreatedBy(createdBy);
    detail.setCreatedAt(createdAt);
    detail.setVersion(0);
	detail.setIsDeleted(0);
}


static oatpp::String preferValue(const oatpp::String& preferred,
    const oatpp::String& fallback)
{
	return preferred && !preferred->empty() ? preferred : fallback;
}


static void addBloodPressureDetails(std::list<VitalSignDetailDO>& details,
	const std::string& vitalSignId, const oatpp::String& value,
	const std::string& createdBy, const std::string& createdAt, UuidFacade& uf)
{
	if (!value || value->empty()) return;
	const std::string text = value.getValue("");
	const auto slash = text.find('/');
	if (slash > 0 && slash + 1 < text.size())
	{
		addDetailItem(details, vitalSignId,
			oatpp::String(text.substr(0, slash).c_str()), "SBP", "mmHg",
			createdBy, createdAt, uf);
		addDetailItem(details, vitalSignId,
			oatpp::String(text.substr(slash + 1).c_str()), "DBP", "mmHg",
			createdBy, createdAt, uf);
		return;
	}
	addDetailItem(details, vitalSignId, value, "BP", "mmHg",
		createdBy, createdAt, uf);
}


NursingRecordListResponseDTO::Wrapper NursingService::listRecords(const NursingRecordQuery::Wrapper& query)
{
    auto dto = NursingRecordListResponseDTO::createShared();
	dto->rows = oatpp::List<NursingRecordRowDTO::Wrapper>::createShared();
	if (!query || !query->encounterId || query->encounterId->empty()
		|| !query->patientId || query->patientId->empty()) return dto;

    NursingDAO dao;
    auto vitalSigns = dao.selectByCondition(query);


    std::string encId = query->encounterId ? query->encounterId.getValue("") : "";
    std::string patId = query->patientId ? query->patientId.getValue("") : "";
    auto paperData = dao.selectPaperHeader(encId, patId);
    dto->paperHeader = buildPaperHeader(*paperData);

    if (vitalSigns.empty())
    {
        dto->rows = oatpp::List<NursingRecordRowDTO::Wrapper>::createShared();
        return dto;
    }


    std::list<std::string> ids;
    for (auto& vs : vitalSigns)
        ids.push_back(vs.getId());


    auto allDetails = dao.selectByVitalSignIds(ids);


    std::map<std::string, std::list<VitalSignDetailDO>> detailMap;
    for (auto& d : allDetails)
        detailMap[d.getVitalSignId()].push_back(std::move(d));

    auto rows = oatpp::List<NursingRecordRowDTO::Wrapper>::createShared();
    for (auto& vs : vitalSigns)
    {
        auto row = NursingRecordRowDTO::createShared();
        row->vitalSignId = vs.getId();
		row->patientId = vs.getPatientId();
		row->encounterId = vs.getEncounterId();
		row->recordedAt = vs.getRecordedAt();
		row->busNo = vs.getBusNo();
		row->locationId = vs.getLocationId();
		row->babyNo = vs.getBabyNo();
		row->recorderId = vs.getRecorderId();
		row->remark = vs.getRemark();
		row->conditionMeasure = vs.getRemark();
		row->version = static_cast<v_int64>(vs.getVersion());
		row->createdBy = vs.getCreatedBy();
		row->createdAt = vs.getCreatedAt();
		row->updatedBy = vs.getUpdatedBy();
		row->updatedAt = vs.getUpdatedAt();

  
        std::string datePart, timePart;
        splitDateTime(vs.getRecordedAt(), datePart, timePart);
        row->recordDate = datePart;
        row->recordTime = timePart;
        row->dateTime = buildDateTime(datePart, timePart);

        auto& details = detailMap[vs.getId()];
        std::string systolic;
        std::string diastolic;
        for (auto& d : details)
        {
			std::string code = d.getItemCode();
			std::transform(code.begin(), code.end(), code.begin(),
				[](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
			if (code == "SBP") systolic = d.getItemValue();
			else if (code == "DBP") diastolic = d.getItemValue();
			else if (code != "_IDEMPOTENCY_KEY") mapDetailToRow(d, row);
		}
		if (!systolic.empty() && !diastolic.empty())
			row->bloodPressure = systolic + "/" + diastolic;

        rows->push_back(row);
    }

    dto->rows = rows;
    return dto;
}


std::string NursingService::generateBusNo(NursingDAO& dao)
{
    std::string today = SimpleDateTimeFormat::format();
    std::string datePart = today.substr(0, 10);
    // Remove dashes: "2026-07-28" → "20260728"
    datePart.erase(std::remove(datePart.begin(), datePart.end(), '-'), datePart.end());


    uint64_t cnt = dao.countByBusNoPrefix("NR" + datePart);

    std::stringstream busNo;
    busNo << "NR" << datePart << std::setw(3) << std::setfill('0') << (cnt + 1);
    return busNo.str();
}


NursingRecordSaveResponseDTO::Wrapper NursingService::saveRecords(
    const NursingRecordSaveRequestDTO::Wrapper& dto, const std::string& operatorId)
{
	auto resp = NursingRecordSaveResponseDTO::createShared();
	resp->savedCount = 0;
	resp->rows = oatpp::List<NursingRecordRowDTO::Wrapper>::createShared();
	resp->results = oatpp::List<NursingRecordSaveItemResultDTO::Wrapper>::createShared();
	if (!dto || !dto->encounterId || dto->encounterId->empty()
		|| !dto->patientId || dto->patientId->empty()
		|| !dto->records || dto->records->empty()) return resp;

    std::string encId = dto->encounterId ? dto->encounterId.getValue("") : "";
    std::string patId = dto->patientId ? dto->patientId.getValue("") : "";

    std::string createdBy = operatorId;
    std::string createdAt = SimpleDateTimeFormat::format();

	UuidFacade uf(1);
	NursingDAO dao;
	const auto fingerprint = nursingFingerprint(dto);
	const auto storedResults = dao.selectIdempotencyResult(
		"SAVE", dto->idempotencyKey.getValue(""));
	if (!storedResults.empty())
	{
		auto restored = restoreNursingResult<NursingRecordSaveResponseDTO>(
			storedResults.front(), fingerprint);
		if (restored) return restored;
		return resp;
	}
	const auto operatorDisplay = dao.selectOperatorDisplay(operatorId);
	TransactionManager tm(&dao);

    int savedCount = 0;
    int busNoSeq = 0; 
	auto savedRows = oatpp::List<NursingRecordRowDTO::Wrapper>::createShared();
	auto savedResults = oatpp::List<NursingRecordSaveItemResultDTO::Wrapper>::createShared();

    try
    {
        for (auto& item : *dto->records.get())
        {
			if (!item || !item->recordDate || item->recordDate->empty())
				throw std::runtime_error("Invalid nursing record.");
			const bool hasBusinessValue=
				(item->temperature&&!item->temperature->empty())||(item->pulse&&!item->pulse->empty())
				||(item->heartRate&&!item->heartRate->empty())||(item->respiration&&!item->respiration->empty())
				||(item->respiratory&&!item->respiratory->empty())
				||(item->bloodPressure&&!item->bloodPressure->empty())||(item->consciousness&&!item->consciousness->empty())
				||(item->pupilLeft&&!item->pupilLeft->empty())||(item->pupilRight&&!item->pupilRight->empty())
				||(item->intakeName&&!item->intakeName->empty())||(item->intakeVolume&&!item->intakeVolume->empty())
				||(item->intakeAmount&&!item->intakeAmount->empty())
				||(item->outputName&&!item->outputName->empty())||(item->outputVolume&&!item->outputVolume->empty())
				||(item->outputAmount&&!item->outputAmount->empty())
				||(item->outputColor&&!item->outputColor->empty())||(item->observation&&!item->observation->empty())
				||(item->nurseSignature&&!item->nurseSignature->empty())
				||(item->oxygenSaturation&&!item->oxygenSaturation->empty())
				||(item->oxygenFlow&&!item->oxygenFlow->empty())||(item->oxygenMode&&!item->oxygenMode->empty())
				||(item->oxygenRate&&!item->oxygenRate->empty())||(item->skinStatus&&!item->skinStatus->empty())
				||(item->conditionMeasure&&!item->conditionMeasure->empty())||(item->tubeCare&&!item->tubeCare->empty())
				||(item->intakeRoute&&!item->intakeRoute->empty())
				||(item->outputColorCharacter&&!item->outputColorCharacter->empty());
			if(!hasBusinessValue)throw std::runtime_error("Blank nursing record.");
            std::string recordDate = item->recordDate ? item->recordDate.getValue("") : "";
            std::string recordTime = item->recordTime ? item->recordTime.getValue("") : "";
            std::string vitalSignId = item->vitalSignId ? item->vitalSignId.getValue("") : "";
            bool updateExisting = false;


            std::string recordedAt = recordDate;
            if (!recordTime.empty())
                recordedAt += " " + recordTime + ":00";
			else if (!recordDate.empty())
				recordedAt += " 00:00:00";
			if (!dao.validRecordedAt(encId, patId, recordedAt))
				throw std::runtime_error("Nursing record time is outside the encounter.");


			std::string existingBusNo;
			std::string existingLocationId;
			std::string existingCreatedBy;
			std::string existingCreatedAt;
			int existingBabyNo = 0;
			if (!vitalSignId.empty())
            {
				auto matching = dao.selectRecord(vitalSignId, encId, patId);
				if (matching.empty())
					throw std::runtime_error("Nursing record does not exist.");
				existingBusNo = matching.front().getBusNo();
				existingLocationId = matching.front().getLocationId();
				existingBabyNo = matching.front().getBabyNo();
				existingCreatedBy = matching.front().getCreatedBy();
				existingCreatedAt = matching.front().getCreatedAt();
                updateExisting = true;
            }
            else
            {

                auto existing = dao.selectBySlot(encId, patId, recordDate, recordTime);
				if (!existing.empty())
				{
					throw std::runtime_error("Nursing record slot already exists.");
                }
                else
                {
                    vitalSignId = uf.genUuid();
                }
            }


            VitalSignDO data;
            data.setId(vitalSignId);
            data.setPatientId(patId);
            data.setEncounterId(encId);
            data.setRecordedAt(recordedAt);
			data.setTypeEnum("NURSING_RECORD");
			data.setRecorderId(operatorId);
			data.setRemark(preferValue(item->conditionMeasure,
				item->observation).getValue(""));


            std::string busNo = updateExisting
				? existingBusNo : generateBusNo(dao);
			if (!updateExisting && busNoSeq++ > 0)
				busNo += std::to_string(busNoSeq - 1);
            data.setBusNo(busNo);

			data.setLocationId(updateExisting ? existingLocationId : "");
			data.setBabyNo(updateExisting ? existingBabyNo : 0);
            data.setCreatedBy(updateExisting ? existingCreatedBy : createdBy);
            data.setCreatedAt(updateExisting ? existingCreatedAt : createdAt);
            data.setVersion(0);
            data.setIsDeleted(0);


			if (updateExisting && !item->expectedVersion.getPtr())
				throw std::runtime_error("Expected version is required.");
			int rows = updateExisting ? dao.updateMainRecord(
				data,item->expectedVersion.getValue(-1)) : dao.insert(data);
            if (rows != 1) throw std::runtime_error("Nursing record version conflict.");
			if (updateExisting) dao.deleteByVitalSignId(vitalSignId);


            std::list<VitalSignDetailDO> details;
            addDetailItem(details, vitalSignId, item->temperature, "TEMP", "C", createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->pulse, "PULSE", "bpm", createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->heartRate, "HR", "bpm", createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, preferValue(item->respiratory, item->respiration),
				"RESP", "breaths/min", createdBy, createdAt, uf);
			addBloodPressureDetails(details, vitalSignId, item->bloodPressure,
				createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->consciousness, "CONSCIOUSNESS", nullptr, createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->pupilLeft, "PUPIL_L", "mm", createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->pupilRight, "PUPIL_R", "mm", createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->intakeName, "INTAKE_NAME", nullptr, createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, preferValue(item->intakeAmount, item->intakeVolume),
				"INTAKE", "ml", createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->outputName, "OUTPUT_NAME", nullptr, createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, preferValue(item->outputAmount, item->outputVolume),
				"OUTPUT", "ml", createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->outputColor, "OUTPUT_COLOR", nullptr, createdBy, createdAt, uf);
			addDetailItem(details, vitalSignId, item->observation,
				"OBSERVATION", nullptr, createdBy, createdAt, uf);
			addDetailItem(details, vitalSignId, item->conditionMeasure,
				"CONDITION_MEASURE", nullptr, createdBy, createdAt, uf);
			oatpp::String canonicalSignature = operatorDisplay.c_str();
			addDetailItem(details, vitalSignId, canonicalSignature,
				"NURSE_SIGNATURE", nullptr, createdBy, createdAt, uf);
			addDetailItem(details, vitalSignId, item->oxygenSaturation, "SPO2", "%", createdBy, createdAt, uf);
			addDetailItem(details, vitalSignId, item->oxygenFlow, "OXYGEN_FLOW", "L/min", createdBy, createdAt, uf);
			addDetailItem(details, vitalSignId, item->oxygenMode, "OXYGEN_MODE", nullptr, createdBy, createdAt, uf);
			addDetailItem(details, vitalSignId, item->oxygenRate, "OXYGEN_RATE", "%", createdBy, createdAt, uf);
			addDetailItem(details, vitalSignId, item->skinStatus, "SKIN_STATUS", nullptr, createdBy, createdAt, uf);
			addDetailItem(details, vitalSignId, item->tubeCare, "TUBE_CARE", nullptr, createdBy, createdAt, uf);
			addDetailItem(details, vitalSignId, item->intakeRoute, "INTAKE_ROUTE", nullptr, createdBy, createdAt, uf);
			addDetailItem(details, vitalSignId, item->outputColorCharacter,
				"OUTPUT_CHARACTER", nullptr, createdBy, createdAt, uf);

            if (!details.empty())
            {
                int detailRows = dao.insertBatch(details);
				if (detailRows != static_cast<int>(details.size()))
					throw std::runtime_error("Nursing record detail insert failed.");
            }


            auto savedRow = NursingRecordRowDTO::createShared();
			copySaveItemToRow(item, savedRow);
			savedRow->nurseSignature = operatorDisplay.c_str();
            savedRow->vitalSignId = vitalSignId;
            savedRow->recordDate  = recordDate;
            savedRow->recordTime  = recordTime;
            savedRow->dateTime    = buildDateTime(recordDate, recordTime);
			savedRow->patientId = patId;
			savedRow->encounterId = encId;
			savedRow->recordedAt = recordedAt;
			savedRow->busNo = busNo;
			savedRow->recorderId = operatorId;
			savedRow->version = updateExisting ? item->expectedVersion.getValue(0)+1 : 0;
            savedRows->push_back(savedRow);
			auto result = NursingRecordSaveItemResultDTO::createShared();
			result->clientRowId = item->clientRowId;
			result->vitalSignId = vitalSignId;
			result->version = savedRow->version;
			result->success = true;
			result->errorCode = "";
			result->message = ZH_WORDS_GETTER("business-text.nursing.save-success");
			result->record = savedRow;
			savedResults->push_back(result);
            savedCount++;
        }


		resp->savedCount = savedCount;
		resp->rows = savedRows;
		resp->results = savedResults;
		const auto resultData = oatpp::parser::json::mapping::ObjectMapper::createShared()
			->writeToString(resp).getValue("");
		if (dao.insertIdempotencyResult(uf.genUuid(), "SAVE",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint,
			resultData) != 1)
			throw std::runtime_error("Nursing idempotency write failed.");
		tm.commit();
    }
	catch (const std::exception& error)
    {
        tm.rollback();
        resp->savedCount = 0;
        resp->rows = oatpp::List<NursingRecordRowDTO::Wrapper>::createShared();
		resp->results = oatpp::List<NursingRecordSaveItemResultDTO::Wrapper>::createShared();
		for (const auto& item : *dto->records)
		{
			auto result = NursingRecordSaveItemResultDTO::createShared();
			result->clientRowId = item ? item->clientRowId : nullptr;
			result->vitalSignId = item ? item->vitalSignId : nullptr;
			result->version = item ? item->expectedVersion.getValue(0) : 0;
			result->success = false;
			const std::string message = error.what();
			if (message == "Nursing record slot already exists.")
				result->errorCode = "SLOT_CONFLICT";
			else if (message == "Expected version is required.")
				result->errorCode = "VERSION_REQUIRED";
			else if (message == "Blank nursing record.")
				result->errorCode = "BLANK_RECORD";
			else if (message == "Nursing record version conflict.")
				result->errorCode = "VERSION_CONFLICT";
			else if (message == "Nursing record does not exist.")
				result->errorCode = "RECORD_NOT_FOUND";
			else if (message == "Nursing record time is outside the encounter.")
				result->errorCode = "RECORD_TIME_INVALID";
			else
				result->errorCode = "SAVE_FAILED";
			result->message = message;
			resp->results->push_back(result);
		}
        return resp;
    }

    resp->savedCount = savedCount;
    resp->rows = savedRows;
	resp->results = savedResults;
    return resp;
}

NursingRecordDeleteResponseDTO::Wrapper NursingService::deleteRecord(
	const NursingRecordDeleteRequestDTO::Wrapper& dto,const std::string& operatorId)
{
	auto response=NursingRecordDeleteResponseDTO::createShared();
	response->success=false;
	response->statusEnum="ACTIVE";
	response->errorCode="INVALID_REQUEST";
	response->message=ZH_WORDS_GETTER("business-text.nursing.invalid-delete");
	if(!dto)return response;
	NursingDAO dao;
	dao.getSqlSession()->beginTransaction();
	try{
		const auto fingerprint=nursingFingerprint(dto);
		const auto storedResults=dao.selectIdempotencyResult(
			"DELETE",dto->idempotencyKey.getValue(""));
		if(!storedResults.empty()){
			auto restored=restoreNursingResult<NursingRecordDeleteResponseDTO>(
				storedResults.front(),fingerprint);
			if(!restored)throw std::runtime_error("Nursing idempotency conflict.");
			dao.getSqlSession()->commitTransaction();
			return restored;
		}
		if(dao.softDeleteRecord(dto->vitalSignId,dto->encounterId,dto->patientId,
			dto->expectedVersion.getValue(-1),operatorId,dto->reason.getValue(""))!=1)
			throw std::runtime_error("Nursing record conflict.");
		dao.deleteByVitalSignId(dto->vitalSignId);
		response->success=true;
		response->statusEnum="DELETED";
		response->version=dto->expectedVersion.getValue(0)+1;
		response->errorCode="";
		response->message=ZH_WORDS_GETTER("business-text.nursing.delete-success");
		const auto resultData=oatpp::parser::json::mapping::ObjectMapper::createShared()
			->writeToString(response).getValue("");
		if(dao.insertIdempotencyResult(UuidFacade().genUuid(),"DELETE",
			dto->idempotencyKey.getValue(""),operatorId,fingerprint,resultData)!=1)
			throw std::runtime_error("Nursing idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}catch(...){
		dao.getSqlSession()->rollbackTransaction();
		response->errorCode="VERSION_CONFLICT";
		response->message=ZH_WORDS_GETTER("business-text.nursing.delete-conflict");
	}
	return response;
}
