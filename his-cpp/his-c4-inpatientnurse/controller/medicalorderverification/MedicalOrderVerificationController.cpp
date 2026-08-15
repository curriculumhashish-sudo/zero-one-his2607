/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/21 00:00:00

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
#include "MedicalOrderVerificationController.h"
#include "service/medicalorderverification/MedicalOrderVerificationService.h"
#include "dao/common/NurseStationAccessDAO.h"
#include "dao/medicalorderverification/MedicalOrderVerificationDAO.h"

MedicalOrderVerificationJsonVO::Wrapper MedicalOrderVerificationController::execQueryMedicalOrderVerification(
    const MedicalVerificationQuery::Wrapper& query, const PayloadDTO& payload) {
	auto vo=MedicalOrderVerificationJsonVO::createShared();
	if (!query || !inpatientnurse::validation::present(query->encounterId)
		|| !inpatientnurse::validation::oneOf(query->isPassed, {"true", "false"})
		|| !inpatientnurse::validation::oneOf(query->statusEnum, {"ACTIVE", "VERIFIED"})
		|| !inpatientnurse::validation::oneOf(
			query->medicalType, {"MED_REQUEST", "SERVICE_REQUEST"})
		|| !inpatientnurse::validation::oneOf(
			query->orderType, {"MED_REQUEST", "SERVICE_REQUEST"})
		|| !inpatientnurse::validation::oneOf(
			query->medicalPeriod, {"LONG_TERM", "TEMPORARY"})
		|| !inpatientnurse::validation::oneOf(
			query->therapyEnum, {"LONG_TERM", "TEMPORARY"}))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	if (!NurseStationAccessDAO().canAccessEncounter(
		payload.getId(), query->encounterId.getValue("")))
	{
		vo->init(nullptr, RS_UNAUTHORIZED);
		return vo;
	}
	vo->success(MedicalOrderVerificationService().list(query, payload.getId()));
	return vo;
}

MedicalVerificationWorkbenchJsonVO::Wrapper
MedicalOrderVerificationController::execQueryMedicalOrderVerificationWorkbench(
	const MedicalVerificationWorkbenchQueryDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	auto vo = MedicalVerificationWorkbenchJsonVO::createShared();
	if (!dto || !dto->encounterIds || dto->encounterIds->empty()
		|| dto->encounterIds->size() > 50
		|| !inpatientnurse::validation::oneOf(dto->auditStatus,
			{"PENDING", "AUDITED", "STOPPED", "VOIDED"}, false)
		|| !inpatientnurse::validation::oneOf(dto->therapyType,
			{"ALL", "LONG_TERM", "TEMPORARY"}, false)
		|| !inpatientnurse::validation::oneOf(dto->medicineCategory,
			{"WEST", "TCM"}, false)) {
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	std::set<std::string> unique;
	for (const auto& encounterId : *dto->encounterIds) {
		if (!inpatientnurse::validation::present(encounterId)
			|| !unique.insert(encounterId.getValue("")).second) {
			vo->init(nullptr, RS_PARAMS_INVALID);
			return vo;
		}
		if (!NurseStationAccessDAO().canAccessEncounter(
			payload.getId(), encounterId.getValue(""))) {
			vo->init(nullptr, RS_UNAUTHORIZED);
			return vo;
		}
	}
	vo->success(MedicalOrderVerificationService().listWorkbench(dto, payload.getId()));
	return vo;
}

VerifyPassJsonVO::Wrapper MedicalOrderVerificationController::execVerifyPass(
    const VerifyPassRequestDTO::Wrapper& dto,const PayloadDTO& payload)
{
    auto jvo = VerifyPassJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->idempotencyKey)
		|| dto->idempotencyKey->size() > 64
		|| !dto->items || dto->items->empty() || dto->items->size() > 50)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	std::set<std::string> requests;
	for (const auto& item : *dto->items)
	{
		if (!item || !inpatientnurse::validation::oneOf(
			item->orderType, {"MED_REQUEST", "SERVICE_REQUEST"}, false)
			|| !inpatientnurse::validation::present(item->requestId)
			|| !item->expectedVersion.getPtr() || item->expectedVersion.getValue(-1) < 0
			|| !requests.insert(inpatientnurse::validation::trim(item->orderType->c_str())
				+ ":" + inpatientnurse::validation::trim(item->requestId->c_str())).second)
		{
			jvo->init(nullptr, RS_PARAMS_INVALID);
			return jvo;
		}
		if (!inpatientnurse::validation::present(item->encounterId))
			item->encounterId = MedicalOrderVerificationDAO().selectEncounterId(
				item->orderType.getValue(""), item->requestId.getValue("")).c_str();
		if (!inpatientnurse::validation::present(item->encounterId)
			|| !NurseStationAccessDAO().canAccessEncounter(
				payload.getId(), item->encounterId.getValue(""))) {
			jvo->init(nullptr, RS_UNAUTHORIZED);
			return jvo;
		}
	}
    auto result=MedicalOrderVerificationService().verify(dto,payload.getId()); result->success ? jvo->success(result) : jvo->fail(result);
    return jvo;
}

RejectOrdersJsonVO::Wrapper MedicalOrderVerificationController::execRejectOrders(
    const RejectOrdersRequestDTO::Wrapper& dto,const PayloadDTO& payload)
{
    auto jvo = RejectOrdersJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->idempotencyKey)
		|| dto->idempotencyKey->size() > 64
		|| !dto->items || dto->items->empty() || dto->items->size() > 50
		|| !inpatientnurse::validation::present(dto->reason)
		|| dto->reason->size() > 500)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	std::set<std::string> requests;
	for (const auto& item : *dto->items)
	{
		if (!item || !inpatientnurse::validation::oneOf(
			item->orderType, {"MED_REQUEST", "SERVICE_REQUEST"}, false)
			|| !inpatientnurse::validation::present(item->requestId)
			|| !item->expectedVersion.getPtr() || item->expectedVersion.getValue(-1) < 0
			|| !requests.insert(inpatientnurse::validation::trim(item->orderType->c_str())
				+ ":" + inpatientnurse::validation::trim(item->requestId->c_str())).second)
		{
			jvo->init(nullptr, RS_PARAMS_INVALID);
			return jvo;
		}
		if (!inpatientnurse::validation::present(item->encounterId))
			item->encounterId = MedicalOrderVerificationDAO().selectEncounterId(
				item->orderType.getValue(""), item->requestId.getValue("")).c_str();
		if (!inpatientnurse::validation::present(item->encounterId)
			|| !NurseStationAccessDAO().canAccessEncounter(
				payload.getId(), item->encounterId.getValue(""))) {
			jvo->init(nullptr, RS_UNAUTHORIZED);
			return jvo;
		}
	}
    auto result=MedicalOrderVerificationService().reject(dto,payload.getId()); result->success ? jvo->success(result) : jvo->fail(result);
    return jvo;
}
