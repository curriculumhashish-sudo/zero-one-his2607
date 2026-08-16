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
#include "TemperatureController.h"
#include "service/temperature/TemperatureService.h"
#include "dao/common/NurseStationAccessDAO.h"

TemperatureInitJsonVO::Wrapper TemperatureController::execGetTemperatureInit()
{
	auto response = TemperatureInitJsonVO::createShared();
	response->success(TemperatureService().getInitConfig());
	return response;
}

TemperatureInitJsonVO::Wrapper TemperatureController::execGetTemperatureInit()
{
    auto jvo = TemperatureInitJsonVO::createShared();
    jvo->success(TemperatureInitDTO::createShared());
    return jvo;
}

TemperatureSheetDataJsonVO::Wrapper TemperatureController::execGetTemperatureData(
    const TemperatureSheetGetDataRequestDTO::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = TemperatureSheetDataJsonVO::createShared();

    if (!query || !inpatientnurse::validation::present(query->encounterId)
        || !inpatientnurse::validation::present(query->patientId)
        || !inpatientnurse::validation::validWindowRange(query->windowRange)
		|| query->pageIndex.getValue(0) < 0 || query->pageIndex.getValue(0) > 1000000
        || !inpatientnurse::validation::validDate(query->focusRecordDate))
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
	if (!NurseStationAccessDAO().canAccessEncounterPatient(
		payload.getId(), query->encounterId.getValue(""), query->patientId.getValue("")))
	{
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}

    TemperatureService service;
    auto data = service.getTemperatureData(query);
    jvo->success(data);
    return jvo;
}

TemperatureSheetSaveJsonVO::Wrapper TemperatureController::execSaveTemperatureRecord(
    const TemperatureSheetSaveRequestDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = TemperatureSheetSaveJsonVO::createShared();

    if (!dto || !inpatientnurse::validation::present(dto->encounterId)
        || !inpatientnurse::validation::present(dto->patientId)
        || !inpatientnurse::validation::validDate(dto->recordDate, false)
		|| !inpatientnurse::validation::oneOf(dto->timeSlot,
			{"02", "06", "10", "14", "18", "22"}, false)
		|| (inpatientnurse::validation::present(dto->vitalSignId)
			&& !dto->expectedVersion.getPtr())
        || !dto->detailItems || dto->detailItems->empty() || dto->detailItems->size() > 50)
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
	std::set<std::string> itemCodes;
    for (const auto& item : *dto->detailItems)
    {
		if (!item || !inpatientnurse::validation::present(item->itemCode)
			|| item->itemCode->size() > 100 || (item->itemValue && item->itemValue->size() > 500)
			|| (item->remark && item->remark->size() > 500)
            || !itemCodes.insert(inpatientnurse::validation::trim(item->itemCode->c_str())).second)
        {
            jvo->init(nullptr, RS_PARAMS_INVALID);
            return jvo;
        }
    }

    TemperatureService service;
	auto data = service.saveTemperatureRecord(dto, payload.getId());
    if (!data || !data->vitalSignId || data->vitalSignId->empty())
        jvo->fail(data);
    else
        jvo->success(data);
    return jvo;
}

TemperatureSheetBatchSaveJsonVO::Wrapper TemperatureController::execSaveTemperatureRecords(
	const TemperatureSheetBatchSaveRequestDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto response = TemperatureSheetBatchSaveJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->idempotencyKey)
		|| dto->idempotencyKey->size() > 64
		|| !inpatientnurse::validation::present(dto->encounterId)
		|| !inpatientnurse::validation::present(dto->patientId)
		|| !inpatientnurse::validation::validDate(dto->recordDate, false)
		|| !dto->slots || dto->slots->empty() || dto->slots->size() > 6)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}
	if (!NurseStationAccessDAO().canAccessEncounterPatient(
		payload.getId(), dto->encounterId.getValue(""), dto->patientId.getValue("")))
	{
		response->init(nullptr, RS_UNAUTHORIZED);
		return response;
	}
	std::set<std::string> slots;
	bool containsLastSlot = false;
	for (const auto& slot : *dto->slots)
	{
		if (!slot || !inpatientnurse::validation::oneOf(slot->timeSlot,
				{"02", "06", "10", "14", "18", "22"}, false)
			|| !slots.insert(slot->timeSlot.getValue("")).second
			|| (inpatientnurse::validation::present(slot->vitalSignId)
				&& !slot->expectedVersion.getPtr())
			|| !slot->detailItems || slot->detailItems->empty() || slot->detailItems->size() > 50)
		{
			response->init(nullptr, RS_PARAMS_INVALID);
			return response;
		}
		containsLastSlot = containsLastSlot || slot->timeSlot.getValue("") == "22";
		std::set<std::string> itemCodes;
		for (const auto& item : *slot->detailItems)
		{
			if (!item || !inpatientnurse::validation::present(item->itemCode)
				|| item->itemCode->size() > 100 || (item->itemValue && item->itemValue->size() > 500)
				|| (item->remark && item->remark->size() > 500)
				|| !itemCodes.insert(inpatientnurse::validation::trim(item->itemCode->c_str())).second)
			{
				response->init(nullptr, RS_PARAMS_INVALID);
				return response;
			}
		}
	}
	if (dto->dailySupplement && !dto->dailySupplement->empty())
	{
		if (!containsLastSlot || dto->dailySupplement->size() > 20)
		{
			response->init(nullptr, RS_PARAMS_INVALID);
			return response;
		}
		std::set<std::string> supplementCodes;
		for (const auto& item : *dto->dailySupplement)
		{
			if (!item || !inpatientnurse::validation::present(item->itemCode)
				|| item->itemCode->size() > 100 || (item->itemValue && item->itemValue->size() > 500)
				|| !supplementCodes.insert(
					inpatientnurse::validation::trim(item->itemCode->c_str())).second)
			{
				response->init(nullptr, RS_PARAMS_INVALID);
				return response;
			}
		}
	}
	auto data = TemperatureService().saveTemperatureRecords(dto, payload.getId());
	if (data && data->savedCount.getValue(0) == static_cast<v_int32>(dto->slots->size()))
		response->success(data);
	else
		response->fail(data);
	return response;
}
