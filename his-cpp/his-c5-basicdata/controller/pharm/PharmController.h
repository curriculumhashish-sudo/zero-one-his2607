#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Lfff
 @Date: 2026/08/01 10:00:00

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
#ifndef _PHARM_CONTROLLER_H_
#define _PHARM_CONTROLLER_H_

#include "ApiHelper.h"
#include "Macros.h"

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pharm/PharmQuery.h"
#include "domain/dto/pharm/PharmDTO.h"
#include "domain/dto/pharm/PharmAddDTO.h"
#include "domain/dto/pharm/PharmModifyDTO.h"
#include "domain/vo/pharm/PharmPageVO.h"
#include "domain/vo/pharm/PharmDetailVO.h"
#include "domain/vo/pharm/PharmVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("pharm.tag")

/**
 * PharmController - Pharmacy/warehouse management API controller.
 *
 * Endpoints (6 total):
 *   1. GET    /pharm/name-list  - Name list for dropdown (id + name), optional form_enum filter
 *   2. GET    /pharm/list       - Paginated query list with conditions (full data for form fill)
 *   3. POST   /pharm            - Add pharmacy/warehouse
 *   4. PUT    /pharm            - Modify pharmacy/warehouse
 *   5. DELETE /pharm/{id}       - Soft-delete pharmacy/warehouse
 *   6. PUT    /pharm/status     - Toggle pharmacy/warehouse status
 */
class PharmController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(PharmController);

public:
    PharmController()
        : oatpp::web::server::api::ApiController(oatpp::parser::json::mapping::ObjectMapper::createShared())
    {
    }

    // 1. GET /pharm/name-list — Pharmacy/warehouse name list (id + name, optional form_enum filter)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.nameList"),
        nameList,
        PharmNameListJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_QUERY_PARAMS(String, "formEnum", ZH_WORDS_GETTER("pharm.field.formEnum"), "", false);
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_GET,
        "/pharm/name-list",
        nameList,
        QUERY(String, formEnum),
        execNameList(formEnum)
    );

    // 2. GET /pharm/list — Paginated query list with conditions
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.list"),
        getPharmList,
        PharmQuery,
        PharmPageJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/pharm/list",
        getPharmList,
        PharmQuery,
        execGetPharmList(query)
    );

    // 3. POST /pharm — Add pharmacy/warehouse
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.add"),
        addPharm,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/pharm",
        addPharm,
        BODY_DTO(PharmAddDTO::Wrapper, dto),
        execAddPharm(dto)
    );

    // 4. PUT /pharm — Modify pharmacy/warehouse
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.modify"),
        modifyPharm,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/pharm",
        modifyPharm,
        BODY_DTO(PharmModifyDTO::Wrapper, dto),
        execModifyPharm(dto)
    );

    // 5. DELETE /pharm/{id} — Soft-delete pharmacy/warehouse
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.remove"),
        removePharm,
        StringJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_QUERY_PARAMS(
            String, "id",
            ZH_WORDS_GETTER("pharm.field.id"), "", true
        );
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_DEL,
        "/pharm/{id}",
        removePharm,
        PATH(String, id),
        execRemovePharm(id)
    );

    // 6. PUT /pharm/status — Toggle pharmacy/warehouse status
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.status"),
        updatePharmStatus,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/pharm/status",
        updatePharmStatus,
        BODY_DTO(PharmModifyDTO::Wrapper, dto),
        execUpdatePharmStatus(dto->id.getValue(""), dto->statusEnum.getValue(""))
    );

private:
    // Name list for dropdown
    PharmNameListJsonVO::Wrapper execNameList(const String& formEnum);
    // Paginated query list
    PharmPageJsonVO::Wrapper execGetPharmList(const PharmQuery::Wrapper& query);
    // Add pharmacy/warehouse
    StringJsonVO::Wrapper execAddPharm(const PharmAddDTO::Wrapper& dto);
    // Modify pharmacy/warehouse
    StringJsonVO::Wrapper execModifyPharm(const PharmModifyDTO::Wrapper& dto);
    // Soft-delete pharmacy/warehouse
    StringJsonVO::Wrapper execRemovePharm(const String& id);
    // Toggle status
    StringJsonVO::Wrapper execUpdatePharmStatus(const std::string& id, const std::string& statusEnum);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_PHARM_CONTROLLER_H_
