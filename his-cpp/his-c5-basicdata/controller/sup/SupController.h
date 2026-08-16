#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Lfff
 @Date: 2026/08/02

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
#ifndef _SUP_CONTROLLER_
#define _SUP_CONTROLLER_

// Project unified helper headers
#include "ApiHelper.h"
#include "Macros.h"

#include "domain/vo/sup/SupVO.h"
#include "domain/vo/sup/SupplierVO.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/sup/SupQuery.h"
#include "domain/query/sup/SupplierQuery.h"
#include "domain/dto/sup/SupDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("sup.tag")

/**
 * SupController - Supplier query endpoints.
 *
 * Provides endpoints for querying supplier detail by ID/code,
 * paginated supplier list with dynamic filters, and supplier name
 * search for dropdown/selector components.
 * Add/update/delete operations are handled by SupplierController
 * and SupplierAddUpdateController.
 */
class SupController : public oatpp::web::server::api::ApiController
{

    API_ACCESS_DECLARE(SupController);

public:
    SupController()
        : oatpp::web::server::api::ApiController(oatpp::parser::json::mapping::ObjectMapper::createShared()) {};

    // GET /sup/query-one — Query single supplier by ID or code
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("sup.query-one.summary"),
        queryOne,
        SupplierDetailQuery,
        SupplierJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/sup/query-one",
        queryOne,
        SupplierDetailQuery,
        execQueryOne(query)
    );

    // GET /sup/query-all — Paginated query with filters
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("sup.query-all.summary"),
        queryAll,
        SupplierQuery,
        SupplierPageJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/sup/query-all",
        queryAll,
        SupplierQuery,
        execQueryAll(query)
    );

    // POST /sup/statusEnum/modify — Query supplier names by keyword (dropdown/selector data source)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.name.query"),
        querySupName,
        SupPageJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/sup/statusEnum/modify",
        querySupName,
        BODY_DTO(SupNameDTO::Wrapper, dto),
        execQuerySupName(dto)
    );

private:
    // Query single supplier by ID or code
    SupplierJsonVO::Wrapper execQueryOne(const SupplierDetailQuery::Wrapper& query);
    // Paginated query with filters
    SupplierPageJsonVO::Wrapper execQueryAll(const SupplierQuery::Wrapper& query);
    // Query supplier names by keyword (dropdown/selector data source)
    SupPageJsonVO::Wrapper execQuerySupName(const SupNameDTO::Wrapper& dto);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_SUP_CONTROLLER_
