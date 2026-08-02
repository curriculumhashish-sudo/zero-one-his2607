 /*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/23

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
#include "DrugController.h"
#include "../../service/drug/DrugService.h"
#include "../ApiDeclarativeServicesHelper.h"
#include <exception>

DrugListPageJsonVO::Wrapper DrugController::execQueryDrug(const DrugQuery::Wrapper &query)
{
    auto jvo = DrugListPageJsonVO::createShared();
    jvo->success(DrugService().listAll(query));
    return jvo;
}

DrugSelectPageJsonVO::Wrapper DrugController::execQuerySelectDrug(const DrugSelectQuery::Wrapper &query)
{
    auto jvo = DrugSelectPageJsonVO::createShared();
    jvo->success(DrugService().SelectlistAll(query));
    return jvo;
    
}

StringJsonVO::Wrapper DrugController::execAddDrug(const DrugAddDTO::Wrapper &dto, const PayloadDTO &payload)
{
    auto jvo = StringJsonVO::createShared();
    const std::string errorMessage = dto ? dto->validate() : "invalid request";
    if (!errorMessage.empty())
    {
        jvo->init(errorMessage, RS_PARAMS_INVALID);
        return jvo;
    }

    dto->setPayload(&payload);
    const std::string id = DrugService().saveData(dto);
    if (!id.empty())
    {
        jvo->success(id);
    }
    else
    {
        jvo->fail("");
    }
    return jvo;
}

DrugDetailJsonVO::Wrapper DrugController::execQueryById(const String &id)
{
    auto jvo = DrugDetailJsonVO::createShared();
    if (!id)
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    jvo->success(DrugService().getById(id.getValue({})));
    return jvo;
}

static ResultStatus mapDrugServiceError(DrugServiceError error)
{
    switch (error)
    {
    case DrugServiceError::InvalidInput:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.invalidInput"), RS_PARAMS_INVALID.getCode());
    case DrugServiceError::NotFound:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.notFound"), RS_FAIL.getCode());
    case DrugServiceError::InvalidStatus:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.invalidStatus"), RS_FAIL.getCode());
    case DrugServiceError::Referenced:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.referenced"), RS_FAIL.getCode());
    case DrugServiceError::CrossTenant:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.crossTenant"), RS_FORBIDDEN.getCode());
    case DrugServiceError::ConcurrentModification:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.concurrentModification"), RS_FAIL.getCode());
    case DrugServiceError::DatabaseFailure:
    default:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.databaseFailure"), RS_SERVER_ERROR.getCode());
    }
}

DrugModifyJsonVO::Wrapper DrugController::execModifyDrug(const DrugModifyDTO::Wrapper &dto,
                                                          const PayloadDTO &payload)
{
    // 创建响应对象。
    auto jvo = DrugModifyJsonVO::createShared();

    // 校验主键。
    if (!dto || !dto->id)
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    // 校验修改表单。
    const std::string validationError = dto->validate();
    if (!validationError.empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    // 执行修改。
    try
    {
        dto->setPayload(&payload);
        auto result = DrugService().updateDrug(dto);
        if (result)
        {
            jvo->success(result);
        }
        else
        {
            jvo->fail(nullptr);
        }
    }
    catch (const DrugServiceException &error)
    {
        jvo->init(nullptr, mapDrugServiceError(error.getError()));
    }
    catch (const std::exception &)
    {
        jvo->init(nullptr, RS_SERVER_ERROR);
    }

    // 返回统一响应。
    return jvo;
}

NoDataJsonVO::Wrapper DrugController::execRemoveDrug(const DrugRemoveDTO::Wrapper &dto,
                                                      const PayloadDTO &payload)
{
    // 创建响应对象。
    auto jvo = NoDataJsonVO::createShared();

    // 校验删除批次。
    if (!dto)
    {
        jvo->init(RS_PARAMS_INVALID);
        return jvo;
    }
    const std::string validationError = dto->validate();
    if (!validationError.empty())
    {
        jvo->init(RS_PARAMS_INVALID);
        return jvo;
    }

    // 执行批量删除。
    try
    {
        dto->setPayload(&payload);
        if (DrugService().removeDrugs(dto))
        {
            jvo->init(RS_SUCCESS);
        }
        else
        {
            jvo->init(RS_FAIL);
        }
    }
    catch (const DrugServiceException &error)
    {
        jvo->init(mapDrugServiceError(error.getError()));
    }
    catch (const std::exception &)
    {
        jvo->init(RS_SERVER_ERROR);
    }

    // 返回无 data 的统一响应。
    return jvo;
}

NoDataJsonVO::Wrapper DrugController::execEnableDisableDrug(const DrugEnableDisableDTO::Wrapper &dto,
                                                             const PayloadDTO &payload)
{
    // 创建响应对象。
    auto jvo = NoDataJsonVO::createShared();

    // 校验目标状态和批次。
    if (!dto)
    {
        jvo->init(RS_PARAMS_INVALID);
        return jvo;
    }
    const std::string validationError = dto->validate();
    if (!validationError.empty())
    {
        jvo->init(RS_PARAMS_INVALID);
        return jvo;
    }

    // 执行整批状态变更。
    try
    {
        dto->setPayload(&payload);
        if (DrugService().enableDisableDrugs(dto))
        {
            jvo->init(RS_SUCCESS);
        }
        else
        {
            jvo->init(RS_FAIL);
        }
    }
    catch (const DrugServiceException &error)
    {
        jvo->init(mapDrugServiceError(error.getError()));
    }
    catch (const std::exception &)
    {
        jvo->init(RS_SERVER_ERROR);
    }

    // 返回统一响应。
    return jvo;
}

/*DrugDetailJsonVO::Wrapper DrugController::execQueryOne(const String& id, const PayloadDTO& payload)
  {
    // 创建声明式客户端并转发凭证。
    API_CLIENT_CREATE(client, objectMapper, DrugApiClient, "drug-api");
    const std::string token = PayloadDTO::getTokenPrefix() + payload.getToken();
    return client->queryById(token, id)->readBodyToDto<DrugDetailJsonVO::Wrapper>(objectMapper);
}

DrugListPageJsonVO::Wrapper DrugController::execQueryAll(
    const DrugQuery::Wrapper &query,
    const PayloadDTO &payload)
{
    // 创建声明式客户端并转发凭证。
    API_CLIENT_CREATE(client, objectMapper, DrugApiClient, "drug-api");
    const std::string token = PayloadDTO::getTokenPrefix() + payload.getToken();
    return client->queryAll(
                     token,
                     query->pageIndex,
                     query->pageSize,
                     URIUtil::urlEncode(query->keywords))
        ->readBodyToDto<DrugListPageJsonVO::Wrapper>(objectMapper);
}
*/