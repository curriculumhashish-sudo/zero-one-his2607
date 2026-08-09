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
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <exception>
#include <sstream>

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

// 将业务码转换为中文词典中的响应状态。
static ResultStatus medicineV2Status(int code)
{
    switch (code)
    {
    case MedicineV2BusinessCode::PARAM_INVALID:
        return ResultStatus(ZH_WORDS_GETTER("drug.v2.message.paramInvalid"), code);
    case MedicineV2BusinessCode::UNAUTHORIZED:
        return ResultStatus(ZH_WORDS_GETTER("drug.v2.message.unauthorized"), code);
    case MedicineV2BusinessCode::FORBIDDEN:
        return ResultStatus(ZH_WORDS_GETTER("drug.v2.message.forbidden"), code);
    case MedicineV2BusinessCode::NOT_FOUND:
        return ResultStatus(ZH_WORDS_GETTER("drug.v2.message.notFound"), code);
    case MedicineV2BusinessCode::PARTIAL_SUCCESS:
        return ResultStatus(ZH_WORDS_GETTER("drug.v2.message.partialSuccess"), code);
    case MedicineV2BusinessCode::VERSION_CONFLICT:
        return ResultStatus(ZH_WORDS_GETTER("drug.v2.message.versionConflict"), code);
    case MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED:
        return ResultStatus(ZH_WORDS_GETTER("drug.v2.message.businessFailed"), code);
    case MedicineV2BusinessCode::SERVER_ERROR:
        return ResultStatus(ZH_WORDS_GETTER("drug.v2.message.serverError"), code);
    case MedicineV2BusinessCode::SUCCESS:
    default:
        return ResultStatus(ZH_WORDS_GETTER("drug.v2.message.success"),
                            MedicineV2BusinessCode::SUCCESS);
    }
}

// 将业务层异常类型转换为统一业务码。
static int medicineV2ServiceCode(DrugServiceError error)
{
    switch (error)
    {
    case DrugServiceError::NotFound:
        return MedicineV2BusinessCode::NOT_FOUND;
    case DrugServiceError::CrossTenant:
        return MedicineV2BusinessCode::FORBIDDEN;
    case DrugServiceError::ConcurrentModification:
        return MedicineV2BusinessCode::VERSION_CONFLICT;
    case DrugServiceError::DatabaseFailure:
        return MedicineV2BusinessCode::SERVER_ERROR;
    case DrugServiceError::InvalidInput:
    case DrugServiceError::InvalidStatus:
    case DrugServiceError::Referenced:
    default:
        return MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED;
    }
}

// 校验路径中的药品编号格式。
static bool isValidMedicineV2Id(const String &id)
{
    if (!id)
    {
        return false;
    }
    const std::string value = id.getValue("");
    return !value.empty() && value.size() <= 64 &&
           std::all_of(value.cbegin(), value.cend(), [](unsigned char character)
                       { return std::isalnum(character) != 0 || character == '-' || character == '_'; });
}

// 根据批量成功、失败明细计算整体业务码。
static int medicineV2BatchCode(const MedicineBatchResultDTO::Wrapper &result)
{
    if (!result || !result->failedItems || result->failedItems->empty())
    {
        return MedicineV2BusinessCode::SUCCESS;
    }
    if (result->successItems && !result->successItems->empty())
    {
        return MedicineV2BusinessCode::PARTIAL_SUCCESS;
    }
    const auto allFailuresAre = [&](int code)
    {
        return std::all_of(result->failedItems->begin(), result->failedItems->end(),
                           [code](const MedicineFailedItemDTO::Wrapper &item)
                           { return item && item->code.getValue(0) == code; });
    };
    if (allFailuresAre(MedicineV2BusinessCode::SERVER_ERROR))
    {
        return MedicineV2BusinessCode::SERVER_ERROR;
    }
    if (allFailuresAre(MedicineV2BusinessCode::NOT_FOUND))
    {
        return MedicineV2BusinessCode::NOT_FOUND;
    }
    if (allFailuresAre(MedicineV2BusinessCode::FORBIDDEN))
    {
        return MedicineV2BusinessCode::FORBIDDEN;
    }
    if (allFailuresAre(MedicineV2BusinessCode::VERSION_CONFLICT))
    {
        return MedicineV2BusinessCode::VERSION_CONFLICT;
    }
    return MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED;
}

// 校验管理员、药品菜单或配置的细粒度权限。
static bool hasConfiguredMedicineV2Authority(const std::list<std::string> &authorities,
                                              const char *configurationName)
{
    // 管理员或现有药品菜单权限可执行；部署环境可追加更细粒度的操作权限。
    if (std::find(authorities.cbegin(), authorities.cend(), "SUPER_ADMIN") != authorities.cend() ||
        std::find(authorities.cbegin(), authorities.cend(), "Navigation.Medicine") != authorities.cend())
    {
        return true;
    }

    if (configurationName == nullptr)
    {
        return false;
    }
    const char *configuredValue = std::getenv(configurationName);
    if (configuredValue == nullptr || *configuredValue == '\0')
    {
        return false;
    }

    std::string configured(configuredValue);
    std::replace(configured.begin(), configured.end(), ';', ',');
    std::istringstream values(configured);
    std::string requiredAuthority;
    while (std::getline(values, requiredAuthority, ','))
    {
        const auto first = requiredAuthority.find_first_not_of(" \t\r\n");
        const auto last = requiredAuthority.find_last_not_of(" \t\r\n");
        if (first == std::string::npos)
        {
            continue;
        }
        requiredAuthority = requiredAuthority.substr(first, last - first + 1);
        if (std::find(authorities.cbegin(), authorities.cend(), requiredAuthority) != authorities.cend())
        {
            return true;
        }
    }
    return false;
}

// 解析并校验药品接口使用的 Bearer 凭证。
std::shared_ptr<CustomerAuthorizeObject> DrugController::authenticateMedicineV2(
    const String &authorization)
{
    // 局部捕获认证异常，避免落入会改写状态码的全局错误处理器。
    if (!authorization || !getDefaultAuthorizationHandler())
    {
        return nullptr;
    }
    try
    {
        const auto authorized = getDefaultAuthorizationHandler()->handleAuthorization(authorization);
        return std::dynamic_pointer_cast<CustomerAuthorizeObject>(authorized);
    }
    catch (...)
    {
        return nullptr;
    }
}

// 根据操作类型读取权限配置并校验当前用户。
bool DrugController::hasMedicineV2Authority(const PayloadDTO &payload, MedicineV2Operation operation)
{
    const char *configurationName = nullptr;
    switch (operation)
    {
    case MedicineV2Operation::Modify:
        configurationName = "DRUG_V2_MODIFY_AUTHORITIES";
        break;
    case MedicineV2Operation::Status:
        configurationName = "DRUG_V2_STATUS_AUTHORITIES";
        break;
    case MedicineV2Operation::Delete:
        configurationName = "DRUG_V2_DELETE_AUTHORITIES";
        break;
    }
    return hasConfiguredMedicineV2Authority(payload.getAuthorities(), configurationName);
}

// 生成单项写操作的标准错误响应。
std::shared_ptr<DrugController::OutgoingResponse> DrugController::medicineV2VersionErrorResponse(int code)
{
    auto body = MedicineVersionJsonVO::createShared();
    body->init(MedicineVersionItemDTO::createShared(), medicineV2Status(code));
    auto response = createDtoResponse(medicineV2HttpStatus(code), body);
    if (code == MedicineV2BusinessCode::UNAUTHORIZED)
    {
        response->putHeader(Header::WWW_AUTHENTICATE, "Bearer realm=\"API\"");
    }
    return response;
}

// 生成批量写操作的标准错误响应。
std::shared_ptr<DrugController::OutgoingResponse> DrugController::medicineV2BatchErrorResponse(int code)
{
    auto body = MedicineBatchJsonVO::createShared();
    body->init(MedicineBatchResultDTO::createEmpty(), medicineV2Status(code));
    auto response = createDtoResponse(medicineV2HttpStatus(code), body);
    if (code == MedicineV2BusinessCode::UNAUTHORIZED)
    {
        response->putHeader(Header::WWW_AUTHENTICATE, "Bearer realm=\"API\"");
    }
    return response;
}

// 校验修改请求并调用业务服务。
MedicineVersionJsonVO::Wrapper DrugController::execUpdateMedicineV2(
    const MedicineUpdateDTO::Wrapper &dto, const PayloadDTO &payload)
{
    auto jvo = MedicineVersionJsonVO::createShared();
    if (!dto || !isValidMedicineV2Id(dto->id) || !dto->validate().empty())
    {
        jvo->init(MedicineVersionItemDTO::createShared(),
                  medicineV2Status(MedicineV2BusinessCode::PARAM_INVALID));
        return jvo;
    }
    try
    {
        dto->setPayload(&payload);
        jvo->init(DrugService().updateMedicineV2(dto->id.getValue(""), dto),
                  medicineV2Status(MedicineV2BusinessCode::SUCCESS));
    }
    catch (const DrugServiceException &error)
    {
        jvo->init(MedicineVersionItemDTO::createShared(),
                  medicineV2Status(medicineV2ServiceCode(error.getError())));
    }
    catch (const std::exception &)
    {
        jvo->init(MedicineVersionItemDTO::createShared(),
                  medicineV2Status(MedicineV2BusinessCode::SERVER_ERROR));
    }
    return jvo;
}

// 校验批量启停请求并汇总每个药品的处理结果。
MedicineBatchJsonVO::Wrapper DrugController::execUpdateMedicineStatusV2(
    const MedicineStatusUpdateDTO::Wrapper &dto, const PayloadDTO &payload)
{
    auto jvo = MedicineBatchJsonVO::createShared();
    if (!dto || !dto->validate().empty())
    {
        jvo->init(MedicineBatchResultDTO::createEmpty(),
                  medicineV2Status(MedicineV2BusinessCode::PARAM_INVALID));
        return jvo;
    }
    try
    {
        dto->setPayload(&payload);
        auto result = DrugService().updateMedicineStatusV2(dto);
        jvo->init(result, medicineV2Status(medicineV2BatchCode(result)));
    }
    catch (const DrugServiceException &error)
    {
        jvo->init(MedicineBatchResultDTO::createEmpty(),
                  medicineV2Status(medicineV2ServiceCode(error.getError())));
    }
    catch (const std::exception &)
    {
        jvo->init(MedicineBatchResultDTO::createEmpty(),
                  medicineV2Status(MedicineV2BusinessCode::SERVER_ERROR));
    }
    return jvo;
}

// 校验批量删除请求并汇总每个药品的处理结果。
MedicineBatchJsonVO::Wrapper DrugController::execBatchDeleteMedicinesV2(
    const MedicineBatchDeleteDTO::Wrapper &dto, const PayloadDTO &payload)
{
    auto jvo = MedicineBatchJsonVO::createShared();
    if (!dto || !dto->validate().empty())
    {
        jvo->init(MedicineBatchResultDTO::createEmpty(),
                  medicineV2Status(MedicineV2BusinessCode::PARAM_INVALID));
        return jvo;
    }
    try
    {
        dto->setPayload(&payload);
        auto result = DrugService().batchDeleteMedicinesV2(dto);
        jvo->init(result, medicineV2Status(medicineV2BatchCode(result)));
    }
    catch (const DrugServiceException &error)
    {
        jvo->init(MedicineBatchResultDTO::createEmpty(),
                  medicineV2Status(medicineV2ServiceCode(error.getError())));
    }
    catch (const std::exception &)
    {
        jvo->init(MedicineBatchResultDTO::createEmpty(),
                  medicineV2Status(MedicineV2BusinessCode::SERVER_ERROR));
    }
    return jvo;
}

// 将统一业务码映射为 HTTP 状态码。
oatpp::web::protocol::http::Status DrugController::medicineV2HttpStatus(int code)
{
    switch (code)
    {
    case MedicineV2BusinessCode::PARAM_INVALID:
        return Status::CODE_400;
    case MedicineV2BusinessCode::UNAUTHORIZED:
        return Status::CODE_401;
    case MedicineV2BusinessCode::FORBIDDEN:
        return Status::CODE_403;
    case MedicineV2BusinessCode::NOT_FOUND:
        return Status::CODE_404;
    case MedicineV2BusinessCode::VERSION_CONFLICT:
        return Status::CODE_409;
    case MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED:
        return Status::CODE_422;
    case MedicineV2BusinessCode::SERVER_ERROR:
        return Status::CODE_500;
    case MedicineV2BusinessCode::SUCCESS:
    case MedicineV2BusinessCode::PARTIAL_SUCCESS:
    default:
        return Status::CODE_200;
    }
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
