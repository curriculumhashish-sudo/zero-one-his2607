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
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * PharmDTO - Full pharmacy/warehouse data transfer object.
 *
 * Used for paginated list display and form data binding.
 * Fields map to the storage/warehouse management table.
 */
class PharmDTO : public oatpp::DTO {
    DTO_INIT(PharmDTO, DTO);

    // Unique identifier
    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) {
        info->description = ZH_WORDS_GETTER("pharm.field.id");
    }

    // Pharmacy/warehouse name
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("pharm.field.name");
    }

    // Type: WAREHOUSE / PHARMACY
    DTO_FIELD(String, type);
    DTO_FIELD_INFO(type) {
        info->description = ZH_WORDS_GETTER("pharm.field.type");
    }

    // Status: ENABLED / DISABLED
    DTO_FIELD(String, status);
    DTO_FIELD_INFO(status) {
        info->description = ZH_WORDS_GETTER("pharm.field.status");
    }

    // Pinyin code for quick search
    DTO_FIELD(String, pyStr);
    DTO_FIELD_INFO(pyStr) {
        info->description = ZH_WORDS_GETTER("pharm.field.pyStr");
    }

    // Wubi code for quick search
    DTO_FIELD(String, wbStr);
    DTO_FIELD_INFO(wbStr) {
        info->description = ZH_WORDS_GETTER("pharm.field.wbStr");
    }

    // Display order for list sorting
    DTO_FIELD(Int32, displayOrder);
    DTO_FIELD_INFO(displayOrder) {
        info->description = ZH_WORDS_GETTER("pharm.field.displayOrder");
    }

    // Remark / note
    DTO_FIELD(String, note);
    DTO_FIELD_INFO(note) {
        info->description = ZH_WORDS_GETTER("pharm.field.note");
    }
};

/**
 * PharmSelectDTO - Lightweight DTO for dropdown selection lists.
 *
 * Only contains id and name, used by the frontend
 * dropdown/select component for pharmacy/warehouse selection.
 */
class PharmSelectDTO : public oatpp::DTO {
    DTO_INIT(PharmSelectDTO, DTO);

    // Unique identifier
    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) {
        info->description = ZH_WORDS_GETTER("pharm.field.id");
    }

    // Pharmacy/warehouse name
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("pharm.field.name");
    }
};

/**
 * PharmPageDTO - Paginated wrapper for PharmDTO list.
 *
 * Wraps a list of PharmDTO with pagination metadata
 * (pageIndex, pageSize, total, pages) for frontend table rendering.
 */
class PharmPageDTO : public PageDTO<PharmDTO::Wrapper>
{
    DTO_INIT(PharmPageDTO, PageDTO<PharmDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARM_DTO_
