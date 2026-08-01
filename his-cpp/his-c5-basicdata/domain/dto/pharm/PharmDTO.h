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
#ifndef _PHARM_DTO_
#define _PHARM_DTO_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * PharmDTO - Full pharmacy/warehouse data transfer object (adm_location).
 * Used for list display and detail responses.
 */
class PharmDTO : public oatpp::DTO
{
    DTO_INIT(PharmDTO, DTO);

    // Required fields
    API_DTO_FIELD_REQUIRE(oatpp::String, id, ZH_WORDS_GETTER("pharm.field.id"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("pharm.field.name"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, formEnum, ZH_WORDS_GETTER("pharm.field.formEnum"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, statusEnum, ZH_WORDS_GETTER("pharm.field.statusEnum"), true);

    // Optional fields
    API_DTO_FIELD_DEFAULT(oatpp::String, busNo, ZH_WORDS_GETTER("pharm.field.busNo"));
    API_DTO_FIELD_DEFAULT(oatpp::String, organizationId, ZH_WORDS_GETTER("pharm.field.organizationId"));
    API_DTO_FIELD_DEFAULT(oatpp::Int32, displayOrder, ZH_WORDS_GETTER("pharm.field.displayOrder"));
    API_DTO_FIELD_DEFAULT(oatpp::String, pyStr, ZH_WORDS_GETTER("pharm.field.pyStr"));
    API_DTO_FIELD_DEFAULT(oatpp::String, wbStr, ZH_WORDS_GETTER("pharm.field.wbStr"));
    API_DTO_FIELD_DEFAULT(oatpp::String, parentId, ZH_WORDS_GETTER("pharm.field.parentId"));
    API_DTO_FIELD_DEFAULT(oatpp::String, createdBy, ZH_WORDS_GETTER("pharm.field.createBy"));
    API_DTO_FIELD_DEFAULT(oatpp::String, updatedBy, ZH_WORDS_GETTER("pharm.field.updateBy"));
    API_DTO_FIELD_DEFAULT(oatpp::String, createdAt, ZH_WORDS_GETTER("pharm.field.createTime"));
    API_DTO_FIELD_DEFAULT(oatpp::String, updatedAt, ZH_WORDS_GETTER("pharm.field.updateTime"));
};

/**
 * PharmNameDTO - Lightweight DTO for pharmacy/warehouse name list (id + name).
 * Used by GET /pharm/name-list endpoint for dropdown selection.
 */
class PharmNameDTO : public oatpp::DTO
{
    DTO_INIT(PharmNameDTO, DTO);
    API_DTO_FIELD_DEFAULT(oatpp::String, id, ZH_WORDS_GETTER("pharm.field.id"));
    API_DTO_FIELD_DEFAULT(oatpp::String, name, ZH_WORDS_GETTER("pharm.field.name"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARM_DTO_
