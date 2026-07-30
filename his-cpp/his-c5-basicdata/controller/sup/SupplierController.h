#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: shelly
 @Date: 2026/7/20

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

#ifndef _SUPLIER_CONTROLLER_
#define _SUPLIER_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/sup/SupplierQuery.h"
#include "domain/dto/sup/SupplierDTO.h"
#include "domain/vo/sup/SupplierVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("sup.tag")

/**
 * SupplierController - Supplier management API.
 *
 * Provides 4 endpoints: query, batch enable/disable, and batch remove.
 */
class SupplierController : public oatpp::web::server::api::ApiController {
    API_ACCESS_DECLARE(SupplierController);

public:
    // ============================================================
    // 1. GET /sup/query-one — Query single supplier by ID or code
    // ============================================================
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

    // ============================================================
    // 2. GET /sup/query-all — Paginated query with filters
    //    Supports: code, name, type, keyword, statusEnum
    // ============================================================
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

    // ============================================================
    // 3. PUT /sup/batch-enable-disable — Batch enable/disable
    // ============================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.batch-enable-disable.summary"),
        batchEnableDisable,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/sup/batch-enable-disable",
        batchEnableDisable,
        BODY_DTO(SupplierBatchEnableDisableDTO::Wrapper, dto),
        execBatchEnableDisable(dto)
    );

    // ============================================================
    // 4. DELETE /sup/batch-remove — Batch delete suppliers
    // ============================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.batch-remove.summary"),
        batchRemove,
        ListJsonVO<String>::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_DEL,
        "/sup/batch-remove",
        batchRemove,
        BODY_DTO(List<String>, ids),
        execBatchRemove(ids)
    );

private:
    // Query single supplier by ID or code (stub — TODO: connect Service)
    SupplierJsonVO::Wrapper execQueryOne(const SupplierDetailQuery::Wrapper& query);
    // Paginated query with filters (stub — TODO: connect Service)
    SupplierPageJsonVO::Wrapper execQueryAll(const SupplierQuery::Wrapper& query);
    // Batch enable/disable (stub — TODO: connect Service)
    StringJsonVO::Wrapper execBatchEnableDisable(const SupplierBatchEnableDisableDTO::Wrapper& dto);
    // Batch delete (stub — TODO: connect Service)
    ListJsonVO<String>::Wrapper execBatchRemove(const List<String>& ids);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_SUPPLIER_CONTROLLER_
