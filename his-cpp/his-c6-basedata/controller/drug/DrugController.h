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
#include "domain/dto/drug/DrugEnableDisableDTO.h"
#include "domain/dto/drug/DrugModifyDTO.h"
#include "domain/dto/drug/DrugRemoveDTO.h"
#include "domain/dto/drug/MedicineV2DTO.h"
#include "domain/query/Drug/DrugQuery.h"
#include "domain/query/Drug/DrugSelectQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/NoDataJsonVO.h"
#include "domain/vo/drug/DrugListVO.h"
#include "domain/vo/drug/DrugModifyVO.h"
#include "domain/vo/drug/DrugSelectVO.h"
#include "domain/vo/drug/MedicineV2VO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#ifndef API_M_PATCH
#define API_M_PATCH "PATCH"
#endif

#define API_TAG ZH_WORDS_GETTER("drug.tag")
#define API_TAG2 ZH_WORDS_GETTER("drug.tags.tag2")
#define MEDICINE_V2_ERROR_RESPONSES(_TYPE_)                                                                        \
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

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.modify.summary"), modifyDrug, DrugModifyJsonVO::Wrapper, API_TAG,
                               info->body.description = ZH_WORDS_GETTER("drug.body.modify");
                               info->body.addExample("default", DrugModifyDTO::createExample());
                               auto responseExample = DrugModifyJsonVO::createShared();
                               responseExample->success(DrugModifyResultDTO::createExample());
                               info->responses[Status::CODE_200].addExample("default", responseExample););
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/drug/modify-drug", modifyDrug, BODY_DTO(DrugModifyDTO::Wrapper, dto),
                              execModifyDrug(dto, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.remove.summary"), removeDrug, NoDataJsonVO::Wrapper, API_TAG,
                               info->body.description = ZH_WORDS_GETTER("drug.body.remove");
                               info->body.addExample("default", DrugRemoveDTO::createExample());
                               auto responseExample = NoDataJsonVO::createShared();
                               responseExample->init(RS_SUCCESS);
                               info->responses[Status::CODE_200].addExample("default", responseExample););
    API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/drug/remove-drug", removeDrug, BODY_DTO(DrugRemoveDTO::Wrapper, dto),
                              execRemoveDrug(dto, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.enable-disable.summary"), enableDisableDrug,
                               NoDataJsonVO::Wrapper, API_TAG,
                               info->body.description = ZH_WORDS_GETTER("drug.body.enableDisable");
                               info->body.addExample("default", DrugEnableDisableDTO::createExample(true));
                               info->body.addExample("disable", DrugEnableDisableDTO::createExample(false));
                               auto responseExample = NoDataJsonVO::createShared();
                               responseExample->init(RS_SUCCESS);
                               info->responses[Status::CODE_200].addExample("default", responseExample););
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/drug/enable-disable-drug", enableDisableDrug,
                               BODY_DTO(DrugEnableDisableDTO::Wrapper, dto),
                               execEnableDisableDrug(dto, authObject->getPayload()));

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
    ENDPOINT(API_M_PATCH, "/api/v2/medicines/status", updateMedicineStatusV2,
             REQUEST(std::shared_ptr<IncomingRequest>, request))
    {
        auto authObject = authenticateMedicineV2(request);
        if (!authObject)
        {
            return medicineV2BatchErrorResponse(MedicineV2BusinessCode::UNAUTHORIZED);
        }
        if (!hasMedicineV2Authority(authObject->getPayload(), MedicineV2Operation::Status))
        {
            return medicineV2BatchErrorResponse(MedicineV2BusinessCode::FORBIDDEN);
        }
        auto dto = readMedicineV2Body<MedicineStatusUpdateDTO>(request);
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
    ENDPOINT(API_M_POST, "/api/v2/medicines/batch-delete", batchDeleteMedicinesV2,
             REQUEST(std::shared_ptr<IncomingRequest>, request))
    {
        auto authObject = authenticateMedicineV2(request);
        if (!authObject)
        {
            return medicineV2BatchErrorResponse(MedicineV2BusinessCode::UNAUTHORIZED);
        }
        if (!hasMedicineV2Authority(authObject->getPayload(), MedicineV2Operation::Delete))
        {
            return medicineV2BatchErrorResponse(MedicineV2BusinessCode::FORBIDDEN);
        }
        auto dto = readMedicineV2Body<MedicineBatchDeleteDTO>(request);
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
    ENDPOINT(API_M_PATCH, "/api/v2/medicines/{id}", updateMedicineV2, PATH(String, id),
             REQUEST(std::shared_ptr<IncomingRequest>, request))
    {
        auto authObject = authenticateMedicineV2(request);
        if (!authObject)
        {
            return medicineV2VersionErrorResponse(MedicineV2BusinessCode::UNAUTHORIZED);
        }
        if (!hasMedicineV2Authority(authObject->getPayload(), MedicineV2Operation::Modify))
        {
            return medicineV2VersionErrorResponse(MedicineV2BusinessCode::FORBIDDEN);
        }
        auto dto = readMedicineV2Body<MedicineUpdateDTO>(request);
        if (!dto)
        {
            return medicineV2VersionErrorResponse(MedicineV2BusinessCode::PARAM_INVALID);
        }
        auto response = execUpdateMedicineV2(id, dto, authObject->getPayload());
        return createDtoResponse(medicineV2HttpStatus(response->code.getValue(10099)), response);
    }

   

  private:
    DrugListPageJsonVO::Wrapper execQueryDrug(const DrugQuery::Wrapper &query);
    DrugSelectPageJsonVO::Wrapper execQuerySelectDrug(const DrugSelectQuery::Wrapper &query);
    StringJsonVO::Wrapper execAddDrug(const DrugAddDTO::Wrapper &dto, const PayloadDTO &payload);
    DrugDetailJsonVO::Wrapper execQueryById(const String &id);

    // 修改药品。
    DrugModifyJsonVO::Wrapper execModifyDrug(const DrugModifyDTO::Wrapper &dto, const PayloadDTO &payload);

    // 批量删除药品。
    NoDataJsonVO::Wrapper execRemoveDrug(const DrugRemoveDTO::Wrapper &dto, const PayloadDTO &payload);

    // 批量启用或停用药品。
    NoDataJsonVO::Wrapper execEnableDisableDrug(const DrugEnableDisableDTO::Wrapper &dto, const PayloadDTO &payload);

    // 新版部分修改药品。
    MedicineVersionJsonVO::Wrapper execUpdateMedicineV2(const String &id,
                                                         const MedicineUpdateDTO::Wrapper &dto,
                                                         const PayloadDTO &payload);

    // 新版批量启停药品。
    MedicineBatchJsonVO::Wrapper execUpdateMedicineStatusV2(const MedicineStatusUpdateDTO::Wrapper &dto,
                                                             const PayloadDTO &payload);

    // 新版批量删除药品。
    MedicineBatchJsonVO::Wrapper execBatchDeleteMedicinesV2(const MedicineBatchDeleteDTO::Wrapper &dto,
                                                             const PayloadDTO &payload);

    std::shared_ptr<CustomerAuthorizeObject> authenticateMedicineV2(
        const std::shared_ptr<IncomingRequest> &request);
    static bool hasMedicineV2Authority(const PayloadDTO &payload, MedicineV2Operation operation);
    std::shared_ptr<OutgoingResponse> medicineV2VersionErrorResponse(int code);
    std::shared_ptr<OutgoingResponse> medicineV2BatchErrorResponse(int code);

    template <class T>
    typename T::Wrapper readMedicineV2Body(const std::shared_ptr<IncomingRequest> &request)
    {
        try
        {
            if (!request || !getDefaultObjectMapper())
            {
                return nullptr;
            }
            const auto body = request->readBodyToString();
            if (!body || body.getValue("").empty())
            {
                return nullptr;
            }
            return getDefaultObjectMapper()->readFromString<typename T::Wrapper>(body);
        }
        catch (...)
        {
            return nullptr;
        }
    }

    static oatpp::web::protocol::http::Status medicineV2HttpStatus(int code);

};

#undef API_TAG
#undef API_TAG2
#undef MEDICINE_V2_ERROR_RESPONSES
#undef MEDICINE_V2_BODY_SCHEMA

#include OATPP_CODEGEN_END(ApiController)

#endif // !_DRUG_CONTROLLER_H_
