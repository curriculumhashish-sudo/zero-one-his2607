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
#include "domain/dto/drug/DrugModifyDTO.h"
#include "domain/dto/drug/DrugRemoveDTO.h"
#include "domain/dto/drug/DrugStartDTO.h"
#include "domain/dto/drug/DrugStopDTO.h"
#include "domain/query/Drug/DrugQuery.h"
#include "domain/query/Drug/DrugSelectQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/NoDataJsonVO.h"
#include "domain/vo/drug/DrugListVO.h"
#include "domain/vo/drug/DrugModifyVO.h"
#include "domain/vo/drug/DrugSelectVO.h"

class DrugSwagger final
{
  public:
    static void addModifyExamples(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info> &info);
    static void addRemoveExamples(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info> &info);
    static void addStartExamples(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info> &info);
    static void addStopExamples(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info> &info);

  private:
    static oatpp::List<oatpp::String> sampleIds();
    static DrugModifyDTO::Wrapper sampleModify();
    static DrugRemoveDTO::Wrapper sampleRemove();
    static DrugStartDTO::Wrapper sampleStart();
    static DrugStopDTO::Wrapper sampleStop();
    static DrugModifyJsonVO::Wrapper successModify();
    static NoDataJsonVO::Wrapper successNoData();
    static void fillRequiredModifyFields(const DrugModifyDTO::Wrapper &dto);
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
    API_HANDLER_ENDPOINT_AUTH(API_M_GET, DRUG_PATH "/query-one", queryOneDrug, QUERY(String, id), execQueryOneDrug(id));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.modify.summary"), modifyDrug, DrugModifyJsonVO::Wrapper, API_TAG,
                               DrugSwagger::addModifyExamples(info););
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, DRUG_PATH "/modify-drug", modifyDrug, BODY_DTO(DrugModifyDTO::Wrapper, dto),
                              execModifyDrug(dto));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.remove.summary"), removeDrug, NoDataJsonVO::Wrapper, API_TAG,
                               DrugSwagger::addRemoveExamples(info););
    API_HANDLER_ENDPOINT_AUTH(API_M_DEL, DRUG_PATH "/remove-drug", removeDrug, BODY_DTO(DrugRemoveDTO::Wrapper, dto),
                              execRemoveDrug(dto));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.start.summary"), startDrug, NoDataJsonVO::Wrapper, API_TAG,
                               DrugSwagger::addStartExamples(info););
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, DRUG_PATH "/start-drug", startDrug, BODY_DTO(DrugStartDTO::Wrapper, dto),
                              execStartDrug(dto));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("drug.stop.summary"), stopDrug, NoDataJsonVO::Wrapper, API_TAG,
                               DrugSwagger::addStopExamples(info););
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, DRUG_PATH "/stop-drug", stopDrug, BODY_DTO(DrugStopDTO::Wrapper, dto),
                              execStopDrug(dto));

  private:
    DrugListPageJsonVO::Wrapper execQueryDrug(const DrugQuery::Wrapper &query);
    DrugSelectPageJsonVO::Wrapper execQuerySelectDrug(const DrugSelectQuery::Wrapper &query);
    StringJsonVO::Wrapper execAddDrug(const DrugAddDTO::Wrapper &dto, const PayloadDTO &payload);
    DrugDetailJsonVO::Wrapper execQueryOneDrug(const String &id);
    DrugModifyJsonVO::Wrapper execModifyDrug(const DrugModifyDTO::Wrapper &dto);
    NoDataJsonVO::Wrapper execRemoveDrug(const DrugRemoveDTO::Wrapper &dto);
    NoDataJsonVO::Wrapper execStartDrug(const DrugStartDTO::Wrapper &dto);
    NoDataJsonVO::Wrapper execStopDrug(const DrugStopDTO::Wrapper &dto);
};

#undef DRUG_PATH
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_DRUG_CONTROLLER_H_
