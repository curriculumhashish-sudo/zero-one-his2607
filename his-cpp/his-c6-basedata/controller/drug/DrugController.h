#pragma once
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
#ifndef _DRUG_CONTROLLER_H_
#define _DRUG_CONTROLLER_H_

#include "domain/GlobalInclude.h"
#include "domain/dto/drug/DrugAddDTO.h"
#include "domain/dto/drug/MedicineV2DTO.h"
#include "domain/query/Drug/DrugQuery.h"
#include "domain/query/Drug/DrugSelectQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/drug/DrugListVO.h"
#include "domain/vo/drug/DrugSelectVO.h"
#include "domain/vo/drug/MedicineV2VO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("drug.tags.tag")
#define API_TAG2 ZH_WORDS_GETTER("drug.tags.tag2")
#define MEDICINE_V2_ERROR_RESPONSES(_TYPE_)                                                                        \
    info->authorization = "Bearer";                                                                               \
    info->addResponse<_TYPE_>(Status::CODE_400, "application/json");                                                \
    info->addResponse<_TYPE_>(Status::CODE_401, "application/json");                                                \
    info->addResponse<_TYPE_>(Status::CODE_403, "application/json");                                                \
    info->addResponse<_TYPE_>(Status::CODE_404, "application/json");                                                \
    info->addResponse<_TYPE_>(Status::CODE_409, "application/json");                                                \
    info->addResponse<_TYPE_>(Status::CODE_422, "application/json");                                                \
    info->addResponse<_TYPE_>(Status::CODE_500, "application/json")
#define MEDICINE_V2_BODY_SCHEMA(_TYPE_, _EXAMPLE_)                                                                 \
    info->body.name = "body";                                                                                       \
    info->body.required = true;                                                                                     \
    info->body.type = _TYPE_::Class::getType();                                                                     \
    if (getDefaultObjectMapper())                                                                                   \
    {                                                                                                               \
        info->bodyContentType = getDefaultObjectMapper()->getInfo().http_content_type;                              \
    }                                                                                                               \
    info->body.addExample("default", _EXAMPLE_)

/** 药品管理控制器。 */
class DrugController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(DrugController);

private:
    enum class MedicineV2Operation
    {
        Modify,
        Status,
        Delete
    };

public:
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("drug.query.summary"), queryDrug, DrugQuery,
        DrugListPageJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/Drug", queryDrug, DrugQuery, execQueryDrug(query));

    API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("drug.select-query.summary"), querySelectDrug, DrugSelectQuery,
        DrugSelectPageJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/DrugSelect", querySelectDrug, DrugSelectQuery,
        execQuerySelectDrug(query));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.add.summary"), addDrug, StringJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/Drug/add", addDrug, BODY_DTO(DrugAddDTO::Wrapper, dto),
        execAddDrug(dto, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.query-one.summary"), queryDrugById, DrugDetailJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("drug.field.id"),
            "ae65c714d48d4f34b52479f5482c0edd", true););
    API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/drug/query-by-id", queryDrugById, QUERY(String, id), execQueryById(id));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.v2.status.summary"), updateMedicineStatusV2,
        MedicineBatchJsonVO::Wrapper, API_TAG,
        MEDICINE_V2_BODY_SCHEMA(MedicineStatusUpdateDTO::Wrapper,
            MedicineStatusUpdateDTO::createExample());
    MEDICINE_V2_ERROR_RESPONSES(MedicineBatchJsonVO::Wrapper);
    info->responses[Status::CODE_400].addExample(
        "invalidBody", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::PARAM_INVALID));
    info->responses[Status::CODE_401].addExample(
        "unauthorized", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::UNAUTHORIZED));
    info->responses[Status::CODE_200].addExample(
        "success", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::SUCCESS));
    info->responses[Status::CODE_200].addExample(
        "partial", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::PARTIAL_SUCCESS));
    info->responses[Status::CODE_409].addExample(
        "versionConflict", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::VERSION_CONFLICT));
    info->responses[Status::CODE_422].addExample(
        "businessValidation", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED)););
    ENDPOINT(API_M_PUT, "/api/v2/medicines/status", updateMedicineStatusV2,
        BODY_DTO(MedicineStatusUpdateDTO::Wrapper, dto),
        HEADER(String, authorization, API_H_TOKEN))
    {
        auto authObject = authenticateMedicineV2(authorization);
        if (!authObject)
        {
            return medicineV2BatchErrorResponse(MedicineV2BusinessCode::UNAUTHORIZED);
        }
        if (!hasMedicineV2Authority(authObject->getPayload(), MedicineV2Operation::Status))
        {
            return medicineV2BatchErrorResponse(MedicineV2BusinessCode::FORBIDDEN);
        }
        if (!dto)
        {
            return medicineV2BatchErrorResponse(MedicineV2BusinessCode::PARAM_INVALID);
        }
        auto response = execUpdateMedicineStatusV2(dto, authObject->getPayload());
        return createDtoResponse(medicineV2HttpStatus(response->code.getValue(10099)), response);
    }

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.v2.batchDelete.summary"), batchDeleteMedicinesV2,
        MedicineBatchJsonVO::Wrapper, API_TAG,
        MEDICINE_V2_BODY_SCHEMA(MedicineBatchDeleteDTO::Wrapper,
            MedicineBatchDeleteDTO::createExample());
    MEDICINE_V2_ERROR_RESPONSES(MedicineBatchJsonVO::Wrapper);
    info->responses[Status::CODE_400].addExample(
        "invalidBody", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::PARAM_INVALID));
    info->responses[Status::CODE_401].addExample(
        "unauthorized", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::UNAUTHORIZED));
    info->responses[Status::CODE_200].addExample(
        "success", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::SUCCESS));
    info->responses[Status::CODE_200].addExample(
        "partial", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::PARTIAL_SUCCESS));
    info->responses[Status::CODE_409].addExample(
        "versionConflict", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::VERSION_CONFLICT));
    info->responses[Status::CODE_422].addExample(
        "businessValidation", MedicineBatchJsonVO::createExample(MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED)););
    ENDPOINT(API_M_DEL, "/api/v2/medicines/batch-delete", batchDeleteMedicinesV2,
        BODY_DTO(MedicineBatchDeleteDTO::Wrapper, dto),
        HEADER(String, authorization, API_H_TOKEN))
    {
        auto authObject = authenticateMedicineV2(authorization);
        if (!authObject)
        {
            return medicineV2BatchErrorResponse(MedicineV2BusinessCode::UNAUTHORIZED);
        }
        if (!hasMedicineV2Authority(authObject->getPayload(), MedicineV2Operation::Delete))
        {
            return medicineV2BatchErrorResponse(MedicineV2BusinessCode::FORBIDDEN);
        }
        if (!dto)
        {
            return medicineV2BatchErrorResponse(MedicineV2BusinessCode::PARAM_INVALID);
        }
        auto response = execBatchDeleteMedicinesV2(dto, authObject->getPayload());
        return createDtoResponse(medicineV2HttpStatus(response->code.getValue(10099)), response);
    }

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.v2.update.summary"), updateMedicineV2,
        MedicineVersionJsonVO::Wrapper, API_TAG,
        API_DEF_ADD_PATH_PARAMS(String, "id", ZH_WORDS_GETTER("drug.field.id"), "10001",
            true);
    MEDICINE_V2_BODY_SCHEMA(MedicineUpdateDTO::Wrapper,
        MedicineUpdateDTO::createExample());
    MEDICINE_V2_ERROR_RESPONSES(MedicineVersionJsonVO::Wrapper);
    info->responses[Status::CODE_400].addExample(
        "invalidBody", MedicineVersionJsonVO::createExample(MedicineV2BusinessCode::PARAM_INVALID));
    info->responses[Status::CODE_401].addExample(
        "unauthorized", MedicineVersionJsonVO::createExample(MedicineV2BusinessCode::UNAUTHORIZED));
    info->responses[Status::CODE_200].addExample(
        "success", MedicineVersionJsonVO::createExample(MedicineV2BusinessCode::SUCCESS));
    info->responses[Status::CODE_409].addExample(
        "versionConflict", MedicineVersionJsonVO::createExample(MedicineV2BusinessCode::VERSION_CONFLICT));
    info->responses[Status::CODE_422].addExample(
        "businessValidation", MedicineVersionJsonVO::createExample(MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED)););
    ENDPOINT(API_M_PUT, "/api/v2/medicines/{id}", updateMedicineV2, PATH(String, id),
        BODY_DTO(MedicineUpdateDTO::Wrapper, dto),
        HEADER(String, authorization, API_H_TOKEN))
    {
        auto authObject = authenticateMedicineV2(authorization);
        if (!authObject)
        {
            return medicineV2VersionErrorResponse(MedicineV2BusinessCode::UNAUTHORIZED);
        }
        if (!hasMedicineV2Authority(authObject->getPayload(), MedicineV2Operation::Modify))
        {
            return medicineV2VersionErrorResponse(MedicineV2BusinessCode::FORBIDDEN);
        }
        if (!dto)
        {
            return medicineV2VersionErrorResponse(MedicineV2BusinessCode::PARAM_INVALID);
        }
        auto response = execUpdateMedicineV2(id, dto, authObject->getPayload());
        return createDtoResponse(medicineV2HttpStatus(response->code.getValue(10099)), response);
    }



private:
    DrugListPageJsonVO::Wrapper execQueryDrug(const DrugQuery::Wrapper& query);
    DrugSelectPageJsonVO::Wrapper execQuerySelectDrug(const DrugSelectQuery::Wrapper& query);
    StringJsonVO::Wrapper execAddDrug(const DrugAddDTO::Wrapper& dto, const PayloadDTO& payload);
    DrugDetailJsonVO::Wrapper execQueryById(const String& id);

    // 部分修改药品。
    MedicineVersionJsonVO::Wrapper execUpdateMedicineV2(const String& id,
        const MedicineUpdateDTO::Wrapper& dto,
        const PayloadDTO& payload);

    // 批量启停药品。
    MedicineBatchJsonVO::Wrapper execUpdateMedicineStatusV2(const MedicineStatusUpdateDTO::Wrapper& dto,
        const PayloadDTO& payload);

    // 批量删除药品。
    MedicineBatchJsonVO::Wrapper execBatchDeleteMedicinesV2(const MedicineBatchDeleteDTO::Wrapper& dto,
        const PayloadDTO& payload);

    std::shared_ptr<CustomerAuthorizeObject> authenticateMedicineV2(
        const String& authorization);
    static bool hasMedicineV2Authority(const PayloadDTO& payload, MedicineV2Operation operation);
    std::shared_ptr<OutgoingResponse> medicineV2VersionErrorResponse(int code);
    std::shared_ptr<OutgoingResponse> medicineV2BatchErrorResponse(int code);

    static oatpp::web::protocol::http::Status medicineV2HttpStatus(int code);

};

#undef API_TAG
#undef API_TAG2
#undef MEDICINE_V2_ERROR_RESPONSES
#undef MEDICINE_V2_BODY_SCHEMA

#include OATPP_CODEGEN_END(ApiController)

#endif // !_DRUG_CONTROLLER_H_
