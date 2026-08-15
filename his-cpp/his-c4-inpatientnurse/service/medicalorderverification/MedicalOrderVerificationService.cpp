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
#include "MedicalOrderVerificationService.h"
#include "dao/medicalorderverification/MedicalOrderVerificationDAO.h"
#include "dao/common/NurseStationAccessDAO.h"
#include "id/UuidFacade.h"
#include <set>
#include <vector>

namespace
{
const std::string VERIFICATION_RESULT_MARKER = "\n--RESULT--\n";

template<class DTO>
std::string verificationFingerprint(const oatpp::Object<DTO>& dto)
{
	return oatpp::parser::json::mapping::ObjectMapper::createShared()
		->writeToString(dto).getValue("");
}

template<class ResponseDTO>
oatpp::Object<ResponseDTO> restoreVerificationResult(
	MedicalOrderVerificationDAO& dao, const std::string& operation,
	const std::string& idempotencyKey, const std::string& fingerprint)
{
	const auto values = dao.selectIdempotencyResult(operation, idempotencyKey);
	if (values.empty()) return nullptr;
	const auto position = values.front().find(VERIFICATION_RESULT_MARKER);
	if (position == std::string::npos
		|| values.front().substr(0, position) != fingerprint) return nullptr;
	return oatpp::parser::json::mapping::ObjectMapper::createShared()
		->readFromString<oatpp::Object<ResponseDTO>>(
			values.front().substr(position + VERIFICATION_RESULT_MARKER.size()).c_str());
}

template<class ResponseDTO>
bool saveVerificationResult(MedicalOrderVerificationDAO& dao,
	const std::string& operation, const std::string& idempotencyKey,
	const std::string& operatorId, const std::string& fingerprint,
	const oatpp::Object<ResponseDTO>& response)
{
	const auto resultData = oatpp::parser::json::mapping::ObjectMapper::createShared()
		->writeToString(response).getValue("");
	return dao.insertIdempotencyResult(UuidFacade().genUuid(), operation,
		idempotencyKey, operatorId, fingerprint, resultData) == 1;
}

void fillVerificationDisplay(const MedicalOrderVerificationDetailDTO::Wrapper& item)
{
	if (item->statusEnum == "ACTIVE")
	{
		item->statusDisplay = ZH_WORDS_GETTER("business-text.verification.status-pending");
		item->statusEnum = "PENDING";
	}
	else if (item->statusEnum == "VERIFIED")
		item->statusDisplay = ZH_WORDS_GETTER("business-text.verification.status-audited");
	else if (item->statusEnum == "STOPPED" || item->statusEnum == "COMPLETED")
		item->statusDisplay = ZH_WORDS_GETTER("business-text.verification.status-stopped");
	else
		item->statusDisplay = ZH_WORDS_GETTER("business-text.verification.status-voided");
	item->therapyDisplay = item->therapyEnum == "LONG_TERM"
		? ZH_WORDS_GETTER("business-text.enum.therapy-long-term")
		: ZH_WORDS_GETTER("business-text.enum.therapy-temporary");
}

std::vector<VerificationWriteItemDTO::Wrapper> expandVerificationItems(
	MedicalOrderVerificationDAO& dao,
	const oatpp::List<VerificationWriteItemDTO::Wrapper>& source)
{
	std::vector<VerificationWriteItemDTO::Wrapper> expanded;
	std::set<std::string> unique;
	for (const auto& item : *source)
	{
		if (item->orderType != "MED_REQUEST")
		{
			const auto key = item->orderType.getValue("") + ":"
				+ item->requestId.getValue("");
			if (unique.insert(key).second) expanded.push_back(item);
			continue;
		}
		auto lines = dao.selectActiveTcmPrescriptionLines(
			item->requestId.getValue(""), item->encounterId.getValue(""));
		if (lines.empty())
		{
			const auto key = std::string("MED_REQUEST:")
				+ item->requestId.getValue("");
			if (unique.insert(key).second) expanded.push_back(item);
			continue;
		}
		bool representativeFound = false;
		for (auto& line : lines)
		{
			if (line->requestId == item->requestId)
			{
				representativeFound = true;
				if (line->expectedVersion.getValue(-1)
					!= item->expectedVersion.getValue(-1))
					throw std::runtime_error("order status changed");
			}
			const auto key = std::string("MED_REQUEST:")
				+ line->requestId.getValue("");
			if (unique.insert(key).second) expanded.push_back(line);
		}
		if (!representativeFound)
			throw std::runtime_error("order status changed");
	}
	return expanded;
}

VerificationWriteResultDTO::Wrapper makeWriteResult(
	const VerificationWriteItemDTO::Wrapper& item, bool success,
	const std::string& status, const std::string& message)
{
	auto result = VerificationWriteResultDTO::createShared();
	result->orderType = item->orderType;
	result->requestId = item->requestId;
	result->success = success;
	result->targetStatusEnum = status.c_str();
	result->message = message.c_str();
	return result;
}
}

oatpp::List<MedicalOrderVerificationDetailDTO::Wrapper> MedicalOrderVerificationService::list(
	const MedicalVerificationQuery::Wrapper& q, const std::string& userId)
{
	auto out=oatpp::List<MedicalOrderVerificationDetailDTO::Wrapper>::createShared();
	if (!q || !NurseStationAccessDAO().canAccessEncounter(
		userId, q->encounterId.getValue(""))) return out;
	for(auto& x:MedicalOrderVerificationDAO().selectItems(q))out->push_back(x);
	return out;
}

MedicalVerificationWorkbenchDTO::Wrapper MedicalOrderVerificationService::listWorkbench(
	const MedicalVerificationWorkbenchQueryDTO::Wrapper& query,
	const std::string& userId)
{
	auto response = MedicalVerificationWorkbenchDTO::createShared();
	if (!query || !query->encounterIds) return response;
	std::map<std::string, MedicalVerificationGroupDTO::Wrapper> groups;
	NurseStationAccessDAO accessDao;
	for (const auto& encounterId : *query->encounterIds) {
		if (!accessDao.canAccessEncounter(userId, encounterId.getValue("")))
			return MedicalVerificationWorkbenchDTO::createShared();
		auto group = MedicalVerificationGroupDTO::createShared();
		group->encounterId = encounterId;
		groups[encounterId.getValue("")] = group;
		response->groups->push_back(group);
	}
	for (auto& item : MedicalOrderVerificationDAO().selectWorkbenchItems(query)) {
		fillVerificationDisplay(item);
		auto group = groups[item->encounterId.getValue("")];
		if (!group) continue;
		if (item->tcmFlag.getValue(false) && item->prescriptionNo
			&& !item->prescriptionNo->empty()) {
			MedicalOrderVerificationDetailDTO::Wrapper existing;
			for (const auto& row : *group->orders) {
				if (row->tcmFlag.getValue(false)
					&& row->prescriptionNo == item->prescriptionNo) {
					existing = row;
					break;
				}
			}
			if (existing) {
				existing->lineRequestIds->push_back(item->requestId);
				continue;
			}
			item->listRowKey = "TCM:" + item->prescriptionNo.getValue("");
		}
		group->orders->push_back(item);
	}
	return response;
}

VerifyPassResponseDTO::Wrapper MedicalOrderVerificationService::verify(
	const VerifyPassRequestDTO::Wrapper& dto,const std::string& userId)
{
	auto out=VerifyPassResponseDTO::createShared();out->processedCount=0;out->success=false;
	out->results=oatpp::List<VerificationWriteResultDTO::Wrapper>::createShared();
	if(!dto||!dto->items||dto->items->empty())return out;
	NurseStationAccessDAO accessDao;
	for (const auto& item : *dto->items)
		if (!item || !accessDao.canAccessEncounter(
			userId, item->encounterId.getValue(""))) return out;
	MedicalOrderVerificationDAO dao;
	const auto fingerprint = verificationFingerprint(dto);
	const auto restored = restoreVerificationResult<VerifyPassResponseDTO>(
		dao, "VERIFY_PASS", dto->idempotencyKey.getValue(""), fingerprint);
	if (restored) return restored;
	if (!dao.selectIdempotencyResult(
		"VERIFY_PASS", dto->idempotencyKey.getValue("")).empty()) return out;
	dao.getSqlSession()->beginTransaction();
	try{
		auto expanded = expandVerificationItems(dao, dto->items);
		for(auto& i:expanded){
			if(dao.verify(i->orderType,i->requestId,i->encounterId,
				i->expectedVersion,userId)!=1
				&& !dao.hasWriteState(i->orderType, i->requestId,
					i->encounterId, "VERIFIED", userId))
				throw std::runtime_error("order status changed");
			out->processedCount=out->processedCount+1;
			out->results->push_back(makeWriteResult(i,true,"VERIFIED",
				ZH_WORDS_GETTER("business-text.verification.verify-success")));
		}
		out->success=true;
		if (!saveVerificationResult(dao, "VERIFY_PASS",
			dto->idempotencyKey.getValue(""), userId, fingerprint, out))
			throw std::runtime_error("Verification idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}catch(...){dao.getSqlSession()->rollbackTransaction();out->processedCount=0;
		out->success=false;
		out->results=oatpp::List<VerificationWriteResultDTO::Wrapper>::createShared();
		for(const auto& i:*dto->items)out->results->push_back(makeWriteResult(
			i,false,"ACTIVE",ZH_WORDS_GETTER("business-text.verification.status-conflict")));}
	return out;
}

RejectOrdersResponseDTO::Wrapper MedicalOrderVerificationService::reject(
	const RejectOrdersRequestDTO::Wrapper& dto,const std::string& userId)
{
	auto out=RejectOrdersResponseDTO::createShared();out->processedCount=0;out->success=false;
	out->results=oatpp::List<VerificationWriteResultDTO::Wrapper>::createShared();
	if(!dto||!dto->items||dto->items->empty()||!dto->reason||dto->reason->empty())return out;
	NurseStationAccessDAO accessDao;
	for (const auto& item : *dto->items)
		if (!item || !accessDao.canAccessEncounter(
			userId, item->encounterId.getValue(""))) return out;
	MedicalOrderVerificationDAO dao;
	const auto fingerprint = verificationFingerprint(dto);
	const auto restored = restoreVerificationResult<RejectOrdersResponseDTO>(
		dao, "REJECT_ORDERS", dto->idempotencyKey.getValue(""), fingerprint);
	if (restored) return restored;
	if (!dao.selectIdempotencyResult(
		"REJECT_ORDERS", dto->idempotencyKey.getValue("")).empty()) return out;
	dao.getSqlSession()->beginTransaction();
	try{
		auto expanded = expandVerificationItems(dao, dto->items);
		for(auto& i:expanded){
			if(dao.reject(i->orderType,i->requestId,i->encounterId,
				i->expectedVersion,dto->reason,userId)!=1)
			{
				if (!dao.hasWriteState(i->orderType, i->requestId,
					i->encounterId, "DRAFT", userId))
					throw std::runtime_error("order status changed");
			}
			out->processedCount=out->processedCount+1;
			out->results->push_back(makeWriteResult(i,true,"DRAFT",
				ZH_WORDS_GETTER("business-text.verification.reject-success")));
		}
		out->success=true;
		if (!saveVerificationResult(dao, "REJECT_ORDERS",
			dto->idempotencyKey.getValue(""), userId, fingerprint, out))
			throw std::runtime_error("Verification idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}catch(...){dao.getSqlSession()->rollbackTransaction();out->processedCount=0;
		out->success=false;
		out->results=oatpp::List<VerificationWriteResultDTO::Wrapper>::createShared();
		for(const auto& i:*dto->items)out->results->push_back(makeWriteResult(
			i,false,"ACTIVE",ZH_WORDS_GETTER("business-text.verification.status-conflict")));}
	return out;
}
