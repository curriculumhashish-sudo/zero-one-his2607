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
#ifndef _PHARM_DTO_
#define _PHARM_DTO_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * PharmDTO - Full pharmacy/warehouse data transfer object (c5-pharm).
 * Used for list display and detail responses.
 */
class PharmDTO : public oatpp::DTO
{
    DTO_INIT(PharmDTO, DTO);

    // Required fields
    API_DTO_FIELD_REQUIRE(oatpp::UInt64, id, ZH_WORDS_GETTER("pharm.field.id"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("pharm.field.name"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, type, ZH_WORDS_GETTER("pharm.field.type"), true);
    API_DTO_FIELD_REQUIRE(oatpp::Int32, status, ZH_WORDS_GETTER("pharm.field.status"), true);

    // Optional fields
    API_DTO_FIELD_DEFAULT(oatpp::Int32, sort, ZH_WORDS_GETTER("pharm.field.sort"));
    API_DTO_FIELD_DEFAULT(oatpp::String, remark, ZH_WORDS_GETTER("pharm.field.remark"));
    API_DTO_FIELD_DEFAULT(oatpp::String, create_by, ZH_WORDS_GETTER("pharm.field.createBy"));
    API_DTO_FIELD_DEFAULT(oatpp::String, update_by, ZH_WORDS_GETTER("pharm.field.updateBy"));
    API_DTO_FIELD_DEFAULT(oatpp::String, create_time, ZH_WORDS_GETTER("pharm.field.createTime"));
    API_DTO_FIELD_DEFAULT(oatpp::String, update_time, ZH_WORDS_GETTER("pharm.field.updateTime"));
};

/**
 * PharmModifyStatusEnumDTO - Status modification DTO (c5 mengsuibizhi).
 * Used for the /pharm/statusEnum/modify endpoint.
 */
class PharmModifyStatusEnumDTO : public oatpp::DTO
{
    DTO_INIT(PharmModifyStatusEnumDTO, DTO);
    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = ZH_WORDS_GETTER("pharm.field.statusEnum");
    }
};

/**
 * PharmSelectDTO - Lightweight DTO for dropdown selection lists (c5 mengsuibizhi).
 */
class PharmSelectDTO : public oatpp::DTO
{
    DTO_INIT(PharmSelectDTO, DTO);
    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) {
        info->description = ZH_WORDS_GETTER("pharm.field.id");
    }
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("pharm.field.name");
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARM_DTO_
