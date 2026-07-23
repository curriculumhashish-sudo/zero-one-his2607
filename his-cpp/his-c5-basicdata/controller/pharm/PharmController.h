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

#ifndef _PHARM_CONTROLLER_
#define _PHARM_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pharm/PharmQuery.h"
#include "domain/dto/pharm/PharmDTO.h"
#include "domain/vo/pharm/PharmVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#ifdef API_TAG
#undef API_TAG
#endif
#define API_TAG ZH_WORDS_GETTER("pharm.tags.name")

/**
 * PharmController - Pharmacy/warehouse management API controller.
 *
 * Provides 3 core endpoints:
 * 1. GET  /pharm/select-list - Dropdown selection list (lightweight, id + name)
 * 2. GET  /pharm/query-all    - Paginated list query with keyword/type/status filters
 * 3. POST /pharm/add          - Add a new pharmacy/warehouse record
 */
class PharmController : public oatpp::web::server::api::ApiController {
    API_ACCESS_DECLARE(PharmController);

public:
    // ============================================================
    // 1. GET /pharm/select-list — Dropdown selection list
    //    Supports optional type filter parameter
    // ============================================================
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

    // ============================================================
    // 2. GET /pharm/query-all — Paginated query list
    //    Supports keyword (name/pinyin/wubi), type, status filters
    // ============================================================
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

    // ============================================================
    // 3. POST /pharm/add — Add a new pharmacy/warehouse
    //    Request body: type (WAREHOUSE/PHARMACY) and name
    // ============================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.add.summary"),
        addPharm,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/pharm/add",
        addPharm,
        BODY_DTO(PharmDTO::Wrapper, dto),
        execAddPharm(dto)
    );

private:
    // Dropdown selection list execution
    PharmSelectListJsonVO::Wrapper execSelectList(const String& type);
    // Paginated query execution
    PharmPageJsonVO::Wrapper execQueryAll(const PharmQuery::Wrapper& query);
    // Add pharmacy/warehouse execution
    StringJsonVO::Wrapper execAddPharm(const PharmDTO::Wrapper& dto);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_PHARM_CONTROLLER_
