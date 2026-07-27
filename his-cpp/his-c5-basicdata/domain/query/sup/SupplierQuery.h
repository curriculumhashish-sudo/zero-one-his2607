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
#ifndef _SUPPLIER_QUERY_
#define _SUPPLIER_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * SupplierDetailQuery - Query parameters for getting a single supplier by ID or code.
 */
class SupplierDetailQuery : public oatpp::DTO
{
    DTO_INIT(SupplierDetailQuery, DTO);

    // Supplier ID
    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) {
        info->description = ZH_WORDS_GETTER("sup.field.id");
    }

    // Supplier code (alternative to ID)
    DTO_FIELD(String, code);
    DTO_FIELD_INFO(code) {
        info->description = ZH_WORDS_GETTER("sup.field.code");
    }
};

/**
 * SupplierQuery - Supplier paginated query parameters.
 *
 * Supports filtering by code, name, type, keyword, and statusEnum.
 * Inherits pageIndex and pageSize from PageQuery.
 */
class SupplierQuery : public PageQuery
{
    DTO_INIT(SupplierQuery, PageQuery);

    // Supplier code (fuzzy match)
    DTO_FIELD(String, code);
    DTO_FIELD_INFO(code) {
        info->description = ZH_WORDS_GETTER("sup.field.code");
    }

    // Supplier name (fuzzy match)
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("sup.field.name");
    }

    // Supplier type filter (e.g. manufacturer/supplier)
    DTO_FIELD(String, type);
    DTO_FIELD_INFO(type) {
        info->description = ZH_WORDS_GETTER("sup.field.typeEnum");
    }

    // Search keyword (matches supplier name, fuzzy)
    DTO_FIELD(String, keyword);
    DTO_FIELD_INFO(keyword) {
        info->description = ZH_WORDS_GETTER("sup.field.keyword");
    }

    // Status filter (e.g. "NORMAL" / "DISABLED")
    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = ZH_WORDS_GETTER("sup.field.statusEnum");
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif
