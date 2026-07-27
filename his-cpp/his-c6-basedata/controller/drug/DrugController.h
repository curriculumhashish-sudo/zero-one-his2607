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
#include "domain/query/Drug/DrugQuery.h"
#include "domain/query/Drug/DrugSelectQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/NoDataJsonVO.h"
#include "domain/vo/drug/DrugListVO.h"
#include "domain/vo/drug/DrugModifyVO.h"
#include "domain/vo/drug/DrugSelectVO.h"
#define API_TAG ZH_WORDS_GETTER("drug.tags.tag")
#define API_TAG2 ZH_WORDS_GETTER("drug.tags.tag2")

class DrugSwagger final
{
  public:
    static void addModifyExamples(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info> &info);
    static void addRemoveExamples(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info> &info);
    static void addEnableDisableExamples(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info> &info);

  private:
    static oatpp::List<oatpp::String> sampleIds();
    static DrugModifyDTO::Wrapper sampleModify();
    static DrugRemoveDTO::Wrapper sampleRemove();
    static DrugEnableDisableDTO::Wrapper sampleEnableDisable(bool activeFlag);
    static DrugModifyJsonVO::Wrapper successModify();
    static NoDataJsonVO::Wrapper successNoData();
    static void fillModifyExample(const DrugModifyDTO::Wrapper &dto);
};

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("drug.tag")
#define DRUG_PATH "/drug"

class DrugController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(DrugController);

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

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.query-one.summary"), queryOneDrug, DrugDetailJsonVO::Wrapper,
                               API_TAG,
                               API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("drug.field.id"),
                                                        "ae65c714d48d4f34b52479f5482c0edd", true););
    API_HANDLER_ENDPOINT_AUTH(API_M_GET, DRUG_PATH "/query-one", queryOneDrug, QUERY(String, id), execQueryById(id));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.modify.summary"), modifyDrug, DrugModifyJsonVO::Wrapper, API_TAG,
                               DrugSwagger::addModifyExamples(info););
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, DRUG_PATH "/modify-drug", modifyDrug, BODY_DTO(DrugModifyDTO::Wrapper, dto),
                              execModifyDrug(dto, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.remove.summary"), removeDrug, NoDataJsonVO::Wrapper, API_TAG,
                               DrugSwagger::addRemoveExamples(info););
    API_HANDLER_ENDPOINT_AUTH(API_M_DEL, DRUG_PATH "/remove-drug", removeDrug, BODY_DTO(DrugRemoveDTO::Wrapper, dto),
                              execRemoveDrug(dto, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.enable-disable.summary"), enableDisableDrug,
                               NoDataJsonVO::Wrapper, API_TAG, DrugSwagger::addEnableDisableExamples(info););
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, DRUG_PATH "/enable-disable-drug", enableDisableDrug,
                              BODY_DTO(DrugEnableDisableDTO::Wrapper, dto),
                              execEnableDisableDrug(dto, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("drug.query-one.summary"), queryOne, DrugDetailJsonVO::Wrapper, API_TAG2,
        API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("drug.field.id"), "d934050a8bb373e8f8eed0bf7507ec17", true);
    );
    // 3.2 定义测试声明式服务调用的接口1处理
    API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/drug/query-one", queryOne, QUERY(String, id), execQueryOne(id, authObject->getPayload()));

    // 3.1 定义测试声明式服务调用的接口2描述
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("drug.query-all.summary"), queryAll, DrugQuery, DrugListPageJsonVO::Wrapper, API_TAG2);
    // 3.2 定义测试声明式服务调用的接口2处理
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/drug/query-all", queryAll, DrugQuery, execQueryAll(query, authObject->getPayload()));

  private:
    DrugListPageJsonVO::Wrapper execQueryDrug(const DrugQuery::Wrapper &query);
    DrugSelectPageJsonVO::Wrapper execQuerySelectDrug(const DrugSelectQuery::Wrapper &query);
    StringJsonVO::Wrapper execAddDrug(const DrugAddDTO::Wrapper &dto, const PayloadDTO &payload);
    DrugDetailJsonVO::Wrapper execQueryById(const String &id);
    DrugModifyJsonVO::Wrapper execModifyDrug(const DrugModifyDTO::Wrapper &dto, const PayloadDTO &payload);
    NoDataJsonVO::Wrapper execRemoveDrug(const DrugRemoveDTO::Wrapper &dto, const PayloadDTO &payload);
    NoDataJsonVO::Wrapper execEnableDisableDrug(const DrugEnableDisableDTO::Wrapper &dto, const PayloadDTO &payload);
    DrugDetailJsonVO::Wrapper execQueryOne(const String& id, const PayloadDTO& payload);
    // 3.3 测试声明式服务调用2
     DrugListPageJsonVO::Wrapper execQueryAll(const DrugQuery::Wrapper& query, const PayloadDTO& payload);
};

#undef DRUG_PATH
#undef API_TAG
#undef API_TAG2

#include OATPP_CODEGEN_END(ApiController)

#endif // !_DRUG_CONTROLLER_H_
