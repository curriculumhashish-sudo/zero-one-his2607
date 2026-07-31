#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Lfff
 @Date: 2026/07/23 10:00:00

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
 * c5-pharm endpoints (real implementation via PharmService):
 *   1. GET    /pharm/list       - Paginated query list
 *   2. POST   /pharm            - Add pharmacy/warehouse
 *   3. PUT    /pharm            - Modify pharmacy/warehouse
 *   4. DELETE /pharm/{id}       - Remove pharmacy/warehouse
 *   5. PUT    /pharm/status     - Toggle pharmacy/warehouse status
 *
 * c5 (mengsuibizhi) additional endpoints (stubs):
 *   6. GET    /pharm/select-list       - Dropdown selection list
 *   7. GET    /pharm/query-all         - Paginated query (GET params)
 *   8. POST   /pharm/statusEnum/modify - Status modify via enum
 *   9. POST   /pharm/list/query        - Paginated query (POST body)
 */
class PharmController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(PharmController);

public:
    PharmController()
        : oatpp::web::server::api::ApiController(oatpp::parser::json::mapping::ObjectMapper::createShared())
    {
    }

    // ============================================================
    // c5-pharm endpoints (real implementation)
    // ============================================================

    // 1. GET /pharm/list — Paginated query list
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

    // 2. POST /pharm — Add pharmacy/warehouse
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

    // 3. PUT /pharm — Modify pharmacy/warehouse
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

    // 4. DELETE /pharm/{id} — Remove pharmacy/warehouse
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

    // 5. PUT /pharm/status — Toggle status
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
        execUpdatePharmStatus(dto->id.getValue(0), dto->status.getValue(0))
    );

    // ============================================================
    // c5 (mengsuibizhi) additional endpoints (stubs)
    // ============================================================

    // 6. GET /pharm/select-list — Dropdown selection list
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.select-list.summary"),
        selectList,
        PharmSelectListJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_QUERY_PARAMS(String, "type", ZH_WORDS_GETTER("pharm.field.type"), "", false);
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_GET,
        "/pharm/select-list",
        selectList,
        QUERY(String, type),
        execSelectList(type)
    );

    // 7. GET /pharm/query-all — Paginated query (GET params)
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("pharm.query-all.summary"),
        queryAll,
        PharmQuery,
        PharmPageJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/pharm/query-all",
        queryAll,
        PharmQuery,
        execQueryAll(query)
    );

    // 8. POST /pharm/statusEnum/modify — Status modify via enum
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.statusEnum.modify"),
        modifyPharmStatusEnum,
        PharmPageJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/pharm/statusEnum/modify",
        modifyPharmStatusEnum,
        BODY_DTO(PharmModifyStatusEnumDTO::Wrapper, dto),
        execModifyPharmStatusEnum(dto)
    );

    // 9. POST /pharm/list/query — Paginated query (POST body)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.list.query"),
        queryPharmList,
        PharmPageJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/pharm/list/query",
        queryPharmList,
        BODY_DTO(PharmQuery::Wrapper, dto),
        execQueryPharmList(dto)
    );

private:
    // ---- c5-pharm real implementations ----
    PharmPageJsonVO::Wrapper execGetPharmList(const PharmQuery::Wrapper& query);
    StringJsonVO::Wrapper execAddPharm(const PharmAddDTO::Wrapper& dto);
    StringJsonVO::Wrapper execModifyPharm(const PharmModifyDTO::Wrapper& dto);
    StringJsonVO::Wrapper execRemovePharm(const String& id);
    StringJsonVO::Wrapper execUpdatePharmStatus(uint64_t id, int status);

    // ---- c5 (mengsuibizhi) stub implementations ----
    PharmSelectListJsonVO::Wrapper execSelectList(const String& type);
    PharmPageJsonVO::Wrapper execQueryAll(const PharmQuery::Wrapper& query);
    StringJsonVO::Wrapper execModifyPharmStatusEnum(const PharmModifyStatusEnumDTO::Wrapper& dto);
    PharmPageJsonVO::Wrapper execQueryPharmList(const PharmQuery::Wrapper& query);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_PHARM_CONTROLLER_H_
