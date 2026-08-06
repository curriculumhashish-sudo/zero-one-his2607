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

NursingRecordListJsonVO::Wrapper NursingController::execListNursingRecords(
    const NursingRecordQuery::Wrapper& query)
{
    auto jvo = NursingRecordListJsonVO::createShared();

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

    NursingService service;
    auto data = service.listRecords(query);
    jvo->success(data);
    return jvo;
}

NursingRecordSaveJsonVO::Wrapper NursingController::execSaveNursingRecords(
    const NursingRecordSaveRequestDTO::Wrapper& dto)
{
    auto jvo = NursingRecordSaveJsonVO::createShared();

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
    if (!dto->records || dto->records->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    NursingService service;
    auto data = service.saveRecords(dto);
    jvo->success(data);
    return jvo;
}

NursingRecordDeleteJsonVO::Wrapper NursingController::execDeleteNursingRecord(
    const NursingRecordDeleteRequestDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = NursingRecordDeleteJsonVO::createShared();
    if (!dto || !dto->encounterId || dto->encounterId->empty() ||
        !dto->patientId || dto->patientId->empty() ||
        !dto->vitalSignId || dto->vitalSignId->empty() ||
        !dto->idempotencyKey || dto->idempotencyKey->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    auto data = NursingService().deleteRecord(dto, payload.getUsername());
    data->success ? jvo->success(data) : jvo->fail(data);
    return jvo;
}
