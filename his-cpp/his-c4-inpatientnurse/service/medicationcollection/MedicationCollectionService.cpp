/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#include "MedicationCollectionService.h"
#include "dao/medicationcollection/MedicationCollectionDAO.h"
#include "id/UuidFacade.h"
#include <algorithm>
#include <set>
#include <sstream>

PagePendingDispenseSummaryLinesDTO::Wrapper MedicationCollectionService::listPending(
	const PagePendingDispenseSummaryLinesQuery::Wrapper& query)
{
	auto page = PagePendingDispenseSummaryLinesDTO::createShared();
	page->rows = oatpp::List<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>::createShared();
	page->patientGroups = oatpp::List<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;
	MedicationCollectionDAO dao;
	page->total = dao.countPending(query);
	page->calcPages();
	std::map<std::string, NurseStationDispenseSummaryPatientGroupDTO::Wrapper> patients;
	std::map<std::string, NurseStationDispenseSummaryOrderRowDTO::Wrapper> orders;
	for (const auto& row : dao.selectPending(query))
	{
		auto& patientGroup = patients[row.encounterId];
		if (!patientGroup)
		{
			patientGroup = NurseStationDispenseSummaryPatientGroupDTO::createShared();
			patientGroup->encounterId = row.encounterId;
			patientGroup->patientId = row.patientId;
			patientGroup->patientName = row.patientName;
			patientGroup->bedDisplay = row.bedName;
			patientGroup->orderRows =
				oatpp::List<NurseStationDispenseSummaryOrderRowDTO::Wrapper>::createShared();
			page->patientGroups->push_back(patientGroup);
			// Retain the legacy alias while clients migrate to patientGroups.
			page->rows->push_back(patientGroup);
		}
		const auto orderKey = row.encounterId + ":" + row.medReqId;
		auto& order = orders[orderKey];
		if (!order)
		{
			order = NurseStationDispenseSummaryOrderRowDTO::createShared();
			order->medReqId = row.medReqId;
			order->medicationId = row.medicationId;
			order->medicationName = row.medicationName;
			order->specificationText = row.specification;
			order->groupId = row.groupId;
			order->therapyEnum = row.therapy;
			order->therapyEnumText = row.therapy == "LONG_TERM"
				? ZH_WORDS_GETTER("business-text.enum.therapy-long-term")
				: ZH_WORDS_GETTER("business-text.enum.therapy-temporary");
			order->dosageInstruction = row.dosage;
			order->methodCode = row.method;
			order->methodCodeText = row.method;
			order->frequencyCode = row.frequency;
			order->frequencyCodeText = row.frequency;
			order->locationId = row.locationId;
			order->locationName = row.locationName;
			order->timePoints =
				oatpp::List<NurseStationDispenseSummaryTimePointDTO::Wrapper>::createShared();
			patientGroup->orderRows->push_back(order);
		}
		auto timePoint = NurseStationDispenseSummaryTimePointDTO::createShared();
		timePoint->dispenseId = row.dispenseId;
		timePoint->procedureId = row.procedureId;
		timePoint->plannedDispenseTime = row.plannedTime;
		timePoint->quantity = row.quantity;
		timePoint->unitCode = row.unitCode;
		timePoint->unitCodeText = row.unitCode;
		timePoint->chargeUnitPrice = row.price;
		order->timePoints->push_back(timePoint);
	}
	return page;
}

namespace
{
	struct SummaryActionItem
	{
		std::string summaryNo;
		uint64_t version = 0;
		bool hasVersion = true;
	};

	const std::string RESULT_MARKER = "\n--RESULT--\n";

	std::string summarizeFingerprint(
		std::vector<std::pair<std::string, std::string>> items)
	{
		std::sort(items.begin(), items.end());
		std::ostringstream stream;
		for (const auto& item : items)
			stream << item.first << '\t' << item.second << '\n';
		return stream.str();
	}

	std::string actionFingerprint(std::vector<SummaryActionItem> items)
	{
		std::sort(items.begin(), items.end(),
			[](const SummaryActionItem& left, const SummaryActionItem& right)
			{
				return left.summaryNo < right.summaryNo;
			});
		std::ostringstream stream;
		for (const auto& item : items)
			stream << item.summaryNo << '\t' << item.version << '\t'
				<< (item.hasVersion ? 1 : 0) << '\n';
		return stream.str();
	}

	bool splitStoredResult(const std::string& stored, const std::string& fingerprint,
		std::string& resultData)
	{
		const auto markerPosition = stored.find(RESULT_MARKER);
		if (markerPosition == std::string::npos
			|| stored.substr(0, markerPosition) != fingerprint) return false;
		resultData = stored.substr(markerPosition + RESULT_MARKER.size());
		return true;
	}

	template<class RequestDTO>
	std::vector<SummaryActionItem> distinctActionItems(const oatpp::Object<RequestDTO>& dto)
	{
		std::vector<SummaryActionItem> result;
		std::set<std::string> seen;
		if (!dto)
		{
			return result;
		}
		if (dto->items)
		{
			for (const auto& item : *dto->items)
			{
				const auto summaryNo = item && item->summaryNo ? item->summaryNo.getValue("") : "";
				if (!summaryNo.empty() && seen.insert(summaryNo).second)
				{
					result.push_back({summaryNo, item->version.getValue(0), true});
				}
			}
		}
		// Accept legacy summaryNos while old clients migrate to versioned items.
		if (dto->summaryNos)
		{
			for (const auto& value : *dto->summaryNos)
			{
				const auto summaryNo = value ? value.getValue("") : "";
				if (!summaryNo.empty() && seen.insert(summaryNo).second)
				{
					result.push_back({summaryNo, 0, false});
				}
			}
		}
		return result;
	}

	template<class ResponseDTO>
	void initializeActionResult(const oatpp::Object<ResponseDTO>& result)
	{
		result->successSummaryNos = oatpp::List<oatpp::String>::createShared();
		result->failedSummaryNos = oatpp::List<oatpp::String>::createShared();
		result->failures = oatpp::List<SummaryFailureDTO::Wrapper>::createShared();
		result->summaryNos = oatpp::List<oatpp::String>::createShared();
	}

	template<class ResponseDTO>
	void addSuccess(const oatpp::Object<ResponseDTO>& result,const std::string& summaryNo)
	{
		result->successSummaryNos->push_back(summaryNo);
		result->summaryNos->push_back(summaryNo);
	}

	template<class ResponseDTO>
	void addFailure(const oatpp::Object<ResponseDTO>& result,const std::string& summaryNo,const char* reason)
	{
		result->failedSummaryNos->push_back(summaryNo);
		auto failure = SummaryFailureDTO::createShared();
		failure->summaryNo = summaryNo;
		failure->reason = reason;
		result->failures->push_back(failure);
	}

	template<class ResponseDTO>
	bool restoreActionResult(const std::string& stored, const std::string& fingerprint,
		const oatpp::Object<ResponseDTO>& result)
	{
		std::string resultData;
		if (!splitStoredResult(stored, fingerprint, resultData)) return false;
		std::stringstream lines(resultData);
		std::string line;
		while (std::getline(lines, line))
		{
			std::stringstream values(line);
			std::string state;
			std::string summaryNo;
			std::string reason;
			std::getline(values, state, '\t');
			std::getline(values, summaryNo, '\t');
			std::getline(values, reason);
			if (state == "S") addSuccess(result, summaryNo);
			else if (state == "F") addFailure(result, summaryNo, reason.c_str());
		}
		return true;
	}
}

CreateMedicationSummaryResponseDTO::Wrapper MedicationCollectionService::summarize(
	const CreateMedicationSummaryDTO::Wrapper& dto, const std::string& operatorId)
{
	auto result = CreateMedicationSummaryResponseDTO::createShared();
	result->message = ZH_WORDS_GETTER("business-text.medication-collection.summary-failed");
	if (!dto || !dto->idempotencyKey || dto->idempotencyKey->empty()
		|| !dto->dispenseItems || dto->dispenseItems->empty()
		|| dto->dispenseItems->size() > 500) return result;
	std::vector<std::pair<std::string, std::string>> items;
	for (const auto& item : *dto->dispenseItems)
		if (item && item->dispenseId && item->encounterId)
			items.emplace_back(item->dispenseId.getValue(""), item->encounterId.getValue(""));
	const auto fingerprint = summarizeFingerprint(items);
	MedicationCollectionDAO dao;
	const auto storedResults = dao.selectIdempotencyResult(
		"SUMMARIZE", dto->idempotencyKey.getValue(""));
	if (!storedResults.empty())
	{
		std::string resultData;
		if (!splitStoredResult(storedResults.front(), fingerprint, resultData))
			return result;
		std::stringstream lines(resultData);
		std::string summaryNo;
		while (std::getline(lines, summaryNo))
			if (!summaryNo.empty()) result->summaryNos->push_back(summaryNo);
		result->successCount = static_cast<v_int32>(items.size());
		result->message = ZH_WORDS_GETTER(
			"business-text.medication-collection.summary-success");
		return result;
	}
	dao.getSqlSession()->beginTransaction();
	try
	{
		const auto rows = dao.lockSummarizable(items);
		if (rows.size() != items.size())
			throw std::runtime_error("DISPENSE_STATE_CHANGED");
		std::map<std::string, std::string> summaryByLocation;
		std::ostringstream resultData;
		UuidFacade uuid;
		for (const auto& row : rows)
		{
			auto& summaryNo = summaryByLocation[row.locationId];
			if (summaryNo.empty())
			{
				summaryNo = "MS" + uuid.genUuid();
				result->summaryNos->push_back(summaryNo);
				resultData << summaryNo << '\n';
			}
			if (dao.attachSummary(row.id, summaryNo, operatorId) != 1
				|| dao.insertSupplyLine(uuid.genUuid(), summaryNo, row, operatorId) != 1)
				throw std::runtime_error("MEDICATION_SUMMARY_WRITE_FAILED");
		}
		if (dao.insertIdempotencyResult(uuid.genUuid(), "SUMMARIZE",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint,
			resultData.str()) != 1)
			throw std::runtime_error("IDEMPOTENCY_WRITE_FAILED");
		dao.getSqlSession()->commitTransaction();
		result->successCount = static_cast<v_int32>(rows.size());
		result->message = ZH_WORDS_GETTER(
			"business-text.medication-collection.summary-success");
	}
	catch (...)
	{
		dao.getSqlSession()->rollbackTransaction();
		result->summaryNos = oatpp::List<oatpp::String>::createShared();
		result->successCount = 0;
	}
	return result;
}

SubmitSummaryResponseDTO::Wrapper MedicationCollectionService::submitSummary(
	const SubmitSummaryRequestDTO::Wrapper& dto,const std::string& operatorId)
{
	auto result = SubmitSummaryResponseDTO::createShared();
	initializeActionResult(result);
	const auto items = distinctActionItems(dto);
	if (!dto || !dto->idempotencyKey || dto->idempotencyKey->empty() || items.empty() || items.size() > 50)
	{
		result->message = ZH_WORDS_GETTER("business-text.medication-collection.invalid-summary-numbers");
		return result;
	}

	MedicationCollectionDAO dao;
	const auto fingerprint = actionFingerprint(items);
	const auto storedResults = dao.selectIdempotencyResult(
		"SUBMIT", dto->idempotencyKey.getValue(""));
	if (!storedResults.empty())
	{
		if (!restoreActionResult(storedResults.front(), fingerprint, result))
			addFailure(result, dto->idempotencyKey.getValue(""),
				"IDEMPOTENCY_KEY_CONFLICT");
		result->message = result->failedSummaryNos->empty()
			? ZH_WORDS_GETTER("business-text.medication-collection.submit-success")
			: ZH_WORDS_GETTER("business-text.medication-collection.submit-partial-failed");
		return result;
	}
	for (const auto& item : items)
	{
		dao.getSqlSession()->beginTransaction();
		try
		{
			const auto versions = dao.lockPendingSummarySupplyVersions(item.summaryNo);
			const auto total = dao.countSummarySupplyLines(item.summaryNo);
			const auto expectedVersion = item.hasVersion || versions.empty() ? item.version : versions.front();
			const auto versionsMatch = std::all_of(versions.begin(), versions.end(),
				[expectedVersion](uint64_t value){return value==expectedVersion;});
			if (total == 0 || versions.size() != total || !versionsMatch
				|| dao.submitSummarySupplyLines(item.summaryNo, expectedVersion, operatorId) != total)
			{
				throw std::runtime_error("VERSION_OR_STATUS_CONFLICT");
			}
			dao.getSqlSession()->commitTransaction();
			addSuccess(result, item.summaryNo);
		}
		catch (const std::exception& exception)
		{
			dao.getSqlSession()->rollbackTransaction();
			addFailure(result, item.summaryNo, exception.what());
		}
	}
	std::ostringstream resultData;
	for (const auto& summaryNo : *result->successSummaryNos)
		resultData << "S\t" << summaryNo.getValue("") << '\n';
	for (const auto& failure : *result->failures)
		resultData << "F\t" << failure->summaryNo.getValue("") << '\t'
			<< failure->reason.getValue("") << '\n';
	dao.getSqlSession()->beginTransaction();
	try
	{
		if (dao.insertIdempotencyResult(UuidFacade().genUuid(), "SUBMIT",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint,
			resultData.str()) != 1)
			throw std::runtime_error("IDEMPOTENCY_WRITE_FAILED");
		dao.getSqlSession()->commitTransaction();
	}
	catch (...)
	{
		dao.getSqlSession()->rollbackTransaction();
	}
	result->message = result->failedSummaryNos->empty()
		? ZH_WORDS_GETTER("business-text.medication-collection.submit-success")
		: ZH_WORDS_GETTER("business-text.medication-collection.submit-partial-failed");
	return result;
}

CancelSummaryResponseDTO::Wrapper MedicationCollectionService::cancelSummary(
	const CancelSummaryRequestDTO::Wrapper& dto,const std::string& operatorId)
{
	auto result = CancelSummaryResponseDTO::createShared();
	initializeActionResult(result);
	const auto items = distinctActionItems(dto);
	if (!dto || !dto->idempotencyKey || dto->idempotencyKey->empty() || items.empty() || items.size() > 50)
	{
		result->message = ZH_WORDS_GETTER("business-text.medication-collection.invalid-summary-numbers");
		return result;
	}

	MedicationCollectionDAO dao;
	const auto fingerprint = actionFingerprint(items);
	const auto storedResults = dao.selectIdempotencyResult(
		"CANCEL", dto->idempotencyKey.getValue(""));
	if (!storedResults.empty())
	{
		if (!restoreActionResult(storedResults.front(), fingerprint, result))
			addFailure(result, dto->idempotencyKey.getValue(""),
				"IDEMPOTENCY_KEY_CONFLICT");
		result->message = result->failedSummaryNos->empty()
			? ZH_WORDS_GETTER("business-text.medication-collection.cancel-success")
			: ZH_WORDS_GETTER("business-text.medication-collection.cancel-partial-failed");
		return result;
	}
	for (const auto& item : items)
	{
		dao.getSqlSession()->beginTransaction();
		try
		{
			const auto versions = dao.lockPendingSummarySupplyVersions(item.summaryNo);
			const auto supplyCount = dao.countSummarySupplyLines(item.summaryNo);
			const auto expectedVersion = item.hasVersion || versions.empty() ? item.version : versions.front();
			const auto versionsMatch = std::all_of(versions.begin(), versions.end(),
				[expectedVersion](uint64_t value){return value==expectedVersion;});
			const auto dispenseCount = dao.countSummaryDispenses(item.summaryNo);
			if (supplyCount == 0 || versions.size() != supplyCount || !versionsMatch
				|| dao.removePendingSummarySupplyLines(item.summaryNo, expectedVersion, operatorId) != supplyCount
				|| (dispenseCount > 0 && dao.releaseSummaryDispenses(item.summaryNo, operatorId) != dispenseCount))
			{
				throw std::runtime_error("VERSION_OR_STATUS_CONFLICT");
			}
			dao.getSqlSession()->commitTransaction();
			addSuccess(result, item.summaryNo);
		}
		catch (const std::exception& exception)
		{
			dao.getSqlSession()->rollbackTransaction();
			addFailure(result, item.summaryNo, exception.what());
		}
	}
	std::ostringstream resultData;
	for (const auto& summaryNo : *result->successSummaryNos)
		resultData << "S\t" << summaryNo.getValue("") << '\n';
	for (const auto& failure : *result->failures)
		resultData << "F\t" << failure->summaryNo.getValue("") << '\t'
			<< failure->reason.getValue("") << '\n';
	dao.getSqlSession()->beginTransaction();
	try
	{
		if (dao.insertIdempotencyResult(UuidFacade().genUuid(), "CANCEL",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint,
			resultData.str()) != 1)
			throw std::runtime_error("IDEMPOTENCY_WRITE_FAILED");
		dao.getSqlSession()->commitTransaction();
	}
	catch (...)
	{
		dao.getSqlSession()->rollbackTransaction();
	}
	result->message = result->failedSummaryNos->empty()
		? ZH_WORDS_GETTER("business-text.medication-collection.cancel-success")
		: ZH_WORDS_GETTER("business-text.medication-collection.cancel-partial-failed");
	return result;
}
