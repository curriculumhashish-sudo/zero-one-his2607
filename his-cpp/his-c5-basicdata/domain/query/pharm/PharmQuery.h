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
 * Used by GET /pharm/list endpoint.
 */
class PharmQuery : public PageQuery
{
    DTO_INIT(PharmQuery, PageQuery);

    // Keyword search (fuzzy match on name)
    API_DTO_FIELD_DEFAULT(oatpp::String, keyword, ZH_WORDS_GETTER("pharm.queries.keyword"));

    // Filter by type
    API_DTO_FIELD_DEFAULT(oatpp::String, type, ZH_WORDS_GETTER("pharm.queries.type"));

    // Filter by status
    API_DTO_FIELD_DEFAULT(oatpp::Int32, status, ZH_WORDS_GETTER("pharm.queries.status"));

    // Filter by name (c5 mengsuibizhi)
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("pharm.field.name");
    }

    // Filter by type enum (c5 mengsuibizhi)
    DTO_FIELD(String, typeEnum);
    DTO_FIELD_INFO(typeEnum) {
        info->description = ZH_WORDS_GETTER("pharm.field.typeEnum");
    }

    // Filter by status enum (c5 mengsuibizhi)
    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = ZH_WORDS_GETTER("pharm.field.statusEnum");
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARM_QUERY_
