/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/26 00:00:00

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
#include "NursingController.h"
#include "service/nursing/NursingService.h"
#include "dao/common/NurseStationAccessDAO.h"

namespace
{
	bool validNumberRange(const oatpp::String& value, double minimum, double maximum)
	{
		if (!inpatientnurse::validation::present(value)) return true;
		const auto text = inpatientnurse::validation::trim(value->c_str());
		if (!inpatientnurse::validation::floatingPoint(text)) return false;
		const auto number = std::stod(text);
		return number >= minimum && number <= maximum;
	}

	bool validBloodPressure(const oatpp::String& value)
	{
		if (!inpatientnurse::validation::present(value)) return true;
		const auto text = inpatientnurse::validation::trim(value->c_str());
		const auto slash = text.find('/');
		if (slash == std::string::npos) return false;
		const auto systolic = text.substr(0, slash);
		const auto diastolic = text.substr(slash + 1);
		if (!inpatientnurse::validation::floatingPoint(systolic)
			|| !inpatientnurse::validation::floatingPoint(diastolic)) return false;
		const auto high = std::stod(systolic);
		const auto low = std::stod(diastolic);
		return high >= 40 && high <= 300 && low >= 20 && low <= 200 && high > low;
	}

	bool validText(const oatpp::String& value, std::size_t maximum)
	{
		return !value || value->size() <= maximum;
	}

	bool validNursingValues(const NursingRecordSaveItemDTO::Wrapper& record)
	{
		return validNumberRange(record->temperature, 30, 45)
			&& validNumberRange(record->pulse, 20, 250)
			&& validNumberRange(record->heartRate, 20, 250)
			&& validNumberRange(record->respiration, 5, 80)
			&& validNumberRange(record->respiratory, 5, 80)
			&& validBloodPressure(record->bloodPressure)
			&& validNumberRange(record->pupilLeft, 0, 10)
			&& validNumberRange(record->pupilRight, 0, 10)
			&& validNumberRange(record->oxygenSaturation, 0, 100)
			&& validNumberRange(record->oxygenFlow, 0, 100)
			&& validNumberRange(record->oxygenRate, 0, 100)
			&& validNumberRange(record->intakeVolume, 0, 100000)
			&& validNumberRange(record->intakeAmount, 0, 100000)
			&& validNumberRange(record->outputVolume, 0, 100000)
			&& validNumberRange(record->outputAmount, 0, 100000)
			&& validText(record->clientRowId, 64)
			&& validText(record->observation, 1000)
			&& validText(record->nurseSignature, 255)
			&& validText(record->consciousness, 255)
			&& validText(record->intakeName, 255)
			&& validText(record->outputName, 255)
			&& validText(record->outputColor, 255)
			&& validText(record->oxygenMode, 255)
			&& validText(record->skinStatus, 255)
			&& validText(record->conditionMeasure, 500)
			&& validText(record->tubeCare, 500)
			&& validText(record->intakeRoute, 255)
			&& validText(record->outputColorCharacter, 255);
	}
}

NursingRecordListJsonVO::Wrapper NursingController::execListNursingRecords(
	const NursingRecordQuery::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = NursingRecordListJsonVO::createShared();

    if (!query || !inpatientnurse::validation::present(query->encounterId)
        || !inpatientnurse::validation::present(query->patientId)
        || !inpatientnurse::validation::validDateTimeRangeDays(
			query->recordedAtFrom, query->recordedAtTo, 31))
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!NurseStationAccessDAO().canAccessEncounterPatient(
		payload.getId(), query->encounterId.getValue(""),
		query->patientId.getValue("")))
	{
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}

    NursingService service;
    auto data = service.listRecords(query);
    jvo->success(data);
    return jvo;
}

NursingRecordSaveJsonVO::Wrapper NursingController::execSaveNursingRecords(
    const NursingRecordSaveRequestDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = NursingRecordSaveJsonVO::createShared();

	if (!dto || !inpatientnurse::validation::present(dto->encounterId)
        || !inpatientnurse::validation::present(dto->patientId)
		|| !inpatientnurse::validation::present(dto->idempotencyKey)
		|| dto->idempotencyKey->size() > 64
        || !dto->records || dto->records->empty() || dto->records->size() > 50)
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!NurseStationAccessDAO().canAccessEncounterPatient(
		payload.getId(), dto->encounterId.getValue(""), dto->patientId.getValue("")))
	{
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}

    std::set<std::string> slots;
    for (const auto& record : *dto->records)
    {
		if (!record || !inpatientnurse::validation::validDate(record->recordDate, false)
			|| !inpatientnurse::validation::validTime(record->recordTime, false)
			|| !inpatientnurse::validation::present(record->clientRowId)
			|| (inpatientnurse::validation::present(record->vitalSignId)
				&& !record->expectedVersion.getPtr())
			|| !validNursingValues(record)
            || !slots.insert(inpatientnurse::validation::trim(record->recordDate->c_str())
                + " " + (record->recordTime
                    ? inpatientnurse::validation::trim(record->recordTime->c_str()) : "")).second)
        {
            jvo->init(nullptr, RS_PARAMS_INVALID);
            return jvo;
        }
    }

    NursingService service;
	auto data = service.saveRecords(dto, payload.getId());
    if (!data || data->savedCount.getValue(0) != static_cast<v_int32>(dto->records->size()))
        jvo->fail(data);
    else
        jvo->success(data);
	return jvo;
}

NursingRecordDeleteJsonVO::Wrapper NursingController::execDeleteNursingRecord(
	const NursingRecordDeleteRequestDTO::Wrapper& dto,const PayloadDTO& payload)
{
	auto jvo=NursingRecordDeleteJsonVO::createShared();
	if(!dto||!inpatientnurse::validation::present(dto->encounterId)
		||!inpatientnurse::validation::present(dto->patientId)
		||!inpatientnurse::validation::present(dto->vitalSignId)
		||!dto->expectedVersion.getPtr()||dto->expectedVersion.getValue(-1)<0
		||!inpatientnurse::validation::present(dto->idempotencyKey)
		||dto->idempotencyKey->size()>64
		||(dto->reason&&dto->reason->size()>500))
	{
		jvo->init(nullptr,RS_PARAMS_INVALID);
		return jvo;
	}
	if(!NurseStationAccessDAO().canAccessEncounterPatient(
		payload.getId(),dto->encounterId.getValue(""),dto->patientId.getValue("")))
	{
		jvo->init(nullptr,RS_UNAUTHORIZED);
		return jvo;
	}
	auto data=NursingService().deleteRecord(dto,payload.getId());
	data->success?jvo->success(data):jvo->fail(data);
	return jvo;
}
