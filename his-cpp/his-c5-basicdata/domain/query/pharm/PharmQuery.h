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
#ifndef _PHARM_QUERY_
#define _PHARM_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * PharmQuery - Pharmacy/warehouse paginated query parameters.
 *
 * Inherits pageIndex and pageSize from PageQuery.
 * Supports filtering by keyword (name/pinyin/wubi), type, and status.
 * Also supports name, typeEnum, and statusEnum for POST-based queries.
 */
class PharmQuery : public PageQuery
{
    DTO_INIT(PharmQuery, PageQuery);

    // Search keyword (matches name, pinyin code, or wubi code)
    DTO_FIELD(String, keyword);
    DTO_FIELD_INFO(keyword) {
        info->description = ZH_WORDS_GETTER("pharm.field.keyword");
    }

    // Filter by name
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("pharm.field.name");
    }

    // Filter by type: WAREHOUSE / PHARMACY
    DTO_FIELD(String, type);
    DTO_FIELD_INFO(type) {
        info->description = ZH_WORDS_GETTER("pharm.field.type");
    }

    // Filter by type enum: REGISTRY (warehouse) / DRUG (pharmacy)
    DTO_FIELD(String, typeEnum);
    DTO_FIELD_INFO(typeEnum) {
        info->description = ZH_WORDS_GETTER("pharm.field.typeEnum");
    }

    // Filter by status: ENABLED / DISABLED
    DTO_FIELD(String, status);
    DTO_FIELD_INFO(status) {
        info->description = ZH_WORDS_GETTER("pharm.field.status");
    }

    // Filter by status enum: NORMAL / DISABLED
    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = ZH_WORDS_GETTER("pharm.field.statusEnum");
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARM_QUERY_
