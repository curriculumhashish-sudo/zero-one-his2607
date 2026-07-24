#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/19 10:00:00

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
#ifndef _NURSINGCONTROLLER_H_
    #define _NURSINGCONTROLLER_H_

    #include "domain/vo/BaseJsonVO.h"
    #include "domain/query/nursing/NursingQuery.h"
    #include "domain/dto/nursing/NursingAddDTO.h"
    #include "domain/dto/nursing/NursingDetailDTO.h"
    #include "domain/vo/nursing/NursingVO.h"

    #include OATPP_CODEGEN_BEGIN(ApiController)
    #define API_TAG ZH_WORDS_GETTER("nursing.tags.tag")

/**
 * Nursing record API controller
 */
class NursingController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(NursingController);

public:
    // Page query nursing records
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("nursing.query.summary"), queryAllNursing, NursingQuery, NursingPageJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/nursing/query-all", queryAllNursing, NursingQuery, execQueryAll(query));

    // Query single nursing record by ID
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("nursing.query-one.summary"),
                               queryOneNursing,
                               NursingJsonVO::Wrapper,
                               API_TAG,
                               API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("nursing.field.id"), "", true););
    API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/nursing/query-one", queryOneNursing, QUERY(String, id), execQueryOne(id));

    // Add nursing record
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("nursing.add.summary"), addNursing, StringJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_POST,
                              "/nursing/add-nursing",
                              addNursing,
                              BODY_DTO(NursingAddDTO::Wrapper, dto),
                              execAddNursing(dto, authObject->getPayload()));

private:
    NursingPageJsonVO::Wrapper execQueryAll(const NursingQuery::Wrapper& query);
    NursingJsonVO::Wrapper execQueryOne(const String& id);
    StringJsonVO::Wrapper execAddNursing(const NursingAddDTO::Wrapper& dto, const PayloadDTO& payload);
};

    #undef API_TAG
    #include OATPP_CODEGEN_END(ApiController)
#endif  // !_NURSINGCONTROLLER_H_
