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

TemperatureSheetDataJsonVO::Wrapper TemperatureController::execGetTemperatureData(
    const TemperatureSheetGetDataRequestDTO::Wrapper& query)
{
    auto jvo = TemperatureSheetDataJsonVO::createShared();

    if (!query->encounterId || query->encounterId->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    if (!query->patientId || query->patientId->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    TemperatureService service;
    auto data = service.getTemperatureData(query);
    jvo->success(data);
    return jvo;
}

TemperatureSheetSaveJsonVO::Wrapper TemperatureController::execSaveTemperatureRecord(
    const TemperatureSheetSaveRequestDTO::Wrapper& dto)
{
    auto jvo = TemperatureSheetSaveJsonVO::createShared();

    if (!dto->encounterId || dto->encounterId->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    if (!dto->patientId || dto->patientId->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    if (!dto->recordDate || dto->recordDate->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    if (!dto->timeSlot || dto->timeSlot->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    TemperatureService service;
    auto data = service.saveTemperatureRecord(dto);
    jvo->success(data);
    return jvo;
}
