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
#include "AdmissionTransferController.h"
#include "service/admissiontransfer/AdmissionTransferService.h"
#include "dao/common/NurseStationAccessDAO.h"

namespace
{
	bool valueInRange(const oatpp::String& value, double minimum, double maximum)
	{
		if (!inpatientnurse::validation::present(value)) return true;
		const auto text = inpatientnurse::validation::trim(value->c_str());
		if (!inpatientnurse::validation::floatingPoint(text)) return false;
		const auto number = std::stod(text);
		return number >= minimum && number <= maximum;
	}

	bool validAdmissionVitalSigns(const AdmissionVitalSignDTO::Wrapper& vital)
	{
		if (!vital) return false;
		const bool hasValue = inpatientnurse::validation::present(vital->temperature)
			|| inpatientnurse::validation::present(vital->pulse)
			|| inpatientnurse::validation::present(vital->heartRate)
			|| inpatientnurse::validation::present(vital->bloodPressureHigh)
			|| inpatientnurse::validation::present(vital->bloodPressureLow)
			|| inpatientnurse::validation::present(vital->heightCm)
			|| inpatientnurse::validation::present(vital->weightKg);
		if (!hasValue
			|| !valueInRange(vital->temperature, 25.0, 45.0)
			|| !valueInRange(vital->pulse, 20.0, 250.0)
			|| !valueInRange(vital->heartRate, 20.0, 250.0)
			|| !valueInRange(vital->bloodPressureHigh, 40.0, 260.0)
			|| !valueInRange(vital->bloodPressureLow, 20.0, 180.0)
			|| !valueInRange(vital->heightCm, 20.0, 250.0)
			|| !valueInRange(vital->weightKg, 0.2, 500.0)) return false;
		if (inpatientnurse::validation::present(vital->bloodPressureHigh)
			&& inpatientnurse::validation::present(vital->bloodPressureLow)
			&& std::stod(inpatientnurse::validation::trim(
				vital->bloodPressureHigh->c_str()))
				<= std::stod(inpatientnurse::validation::trim(
					vital->bloodPressureLow->c_str()))) return false;
		return true;
	}
}

BedBoardJsonVO::Wrapper AdmissionTransferController::execQueryBedBoard(
	const BedBoardQuery::Wrapper& query, const PayloadDTO& payload)
{
	auto vo = BedBoardJsonVO::createShared();
	if (!query || !inpatientnurse::validation::oneOf(
		query->panel, {"ADMISSION", "DISCHARGE"}, false)
		|| !inpatientnurse::validation::validCsv(query->wardIds, 50)
		|| !inpatientnurse::validation::validCsv(query->houseIds, 50))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	vo->success(AdmissionTransferService().queryBedBoard(query, payload.getId()));
	return vo;
}

BedOperationJsonVO::Wrapper AdmissionTransferController::execExecuteBedOperation(
	const ExecuteBedOperationDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto vo = BedOperationJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::oneOf(
		dto->operationType, {"ASSIGN", "TRANSFER", "SWAP"}, false)
		|| !inpatientnurse::validation::present(dto->encounterId)
		|| !inpatientnurse::validation::present(dto->targetBedId)
		|| !inpatientnurse::validation::present(dto->targetHouseId)
		|| !dto->expectedBedVersion.getPtr()
		|| dto->expectedBedVersion.getValue(-1) < 0
		|| !inpatientnurse::validation::present(dto->idempotencyKey)
		|| !inpatientnurse::validation::oneOf(
			dto->priorityEnum, {"NORMAL", "URGENT"}, false)
		|| (dto->operationType == "SWAP"
			&& !inpatientnurse::validation::present(dto->swapEncounterId))
		|| !dto->participants || dto->participants->size() > 50
		|| (dto->operationType == "ASSIGN"
			&& (dto->participants->empty()
				|| !validAdmissionVitalSigns(dto->admissionVitalSigns))))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	NurseStationAccessDAO accessDao;
	if (!accessDao.canAccessBed(payload.getId(), dto->targetBedId.getValue(""))
		|| (dto->operationType != "ASSIGN"
			&& !accessDao.canAccessEncounter(payload.getId(), dto->encounterId.getValue("")))
		|| (dto->operationType == "SWAP"
			&& !accessDao.canAccessEncounter(
				payload.getId(), dto->swapEncounterId.getValue(""))))
	{
		vo->init(nullptr, RS_UNAUTHORIZED);
		return vo;
	}
	std::set<std::string> participants;
	for (const auto& participant : *dto->participants)
	{
		if (!participant
			|| !inpatientnurse::validation::present(participant->practitionerId)
			|| !inpatientnurse::validation::present(participant->roleCode)
			|| !participants.insert(
				inpatientnurse::validation::trim(participant->practitionerId->c_str())
				+ ":" + inpatientnurse::validation::trim(participant->roleCode->c_str())).second)
		{
			vo->init(nullptr, RS_PARAMS_INVALID);
			return vo;
		}
	}
	auto result = AdmissionTransferService().executeBedOperation(dto, payload.getId());
	result->succeeded ? vo->success(result) : vo->fail(result);
	return vo;
}

RecallInpatientDischargeJsonVO::Wrapper AdmissionTransferController::execRecallInpatientDischarge(
	const RecallInpatientDischargeDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto vo = RecallInpatientDischargeJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->encounterId))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	if (!NurseStationAccessDAO().canAccessEncounter(
		payload.getId(), dto->encounterId.getValue("")))
	{
		vo->init(nullptr, RS_UNAUTHORIZED);
		return vo;
	}
	auto result = AdmissionTransferService().recallDischarge(dto, payload.getId());
	result->succeeded ? vo->success(result) : vo->fail(result);
	return vo;
}

DischargePendingListJsonVO::Wrapper AdmissionTransferController::execQueryPendingExecutions(
	const DischargePendingQuery::Wrapper& query, const PayloadDTO& payload)
{
	auto vo = DischargePendingListJsonVO::createShared();
	if (!query || !inpatientnurse::validation::present(query->encounterId))
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
	vo->success(AdmissionTransferService().queryPendingExecutions(query->encounterId));
	return vo;
}

DischargePendingListJsonVO::Wrapper AdmissionTransferController::execQueryPendingMedications(
	const DischargePendingQuery::Wrapper& query, const PayloadDTO& payload)
{
	auto vo = DischargePendingListJsonVO::createShared();
	if (!query || !inpatientnurse::validation::present(query->encounterId))
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
	vo->success(AdmissionTransferService().queryPendingMedications(query->encounterId));
	return vo;
}

ExecuteInpatientDischargeJsonVO::Wrapper AdmissionTransferController::execExecuteInpatientDischarge(
	const ExecuteInpatientDischargeDTO::Wrapper& dto,const PayloadDTO& payload)
{
	auto vo = ExecuteInpatientDischargeJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->encounterId))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	if (!NurseStationAccessDAO().canAccessEncounter(
		payload.getId(), dto->encounterId.getValue("")))
	{
		vo->init(nullptr, RS_UNAUTHORIZED);
		return vo;
	}
	auto result = AdmissionTransferService().discharge(dto, payload.getId());
	result->succeeded ? vo->success(result) : vo->fail(result);
	return vo;
}

ClearDischargedBedJsonVO::Wrapper AdmissionTransferController::execClearDischargedBed(
	const ClearDischargedBedDTO::Wrapper& dto,const PayloadDTO& payload)
{
	auto vo = ClearDischargedBedJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->encounterId))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	if (!NurseStationAccessDAO().canAccessEncounter(
		payload.getId(), dto->encounterId.getValue("")))
	{
		vo->init(nullptr, RS_UNAUTHORIZED);
		return vo;
	}
	auto result = AdmissionTransferService().clearBed(dto, payload.getId());
	result->succeeded ? vo->success(result) : vo->fail(result);
	return vo;
}
