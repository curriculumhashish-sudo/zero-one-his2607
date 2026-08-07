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
#ifndef _SUPPLIER_DTO_
#define _SUPPLIER_DTO_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * SupplierDTO - Full supplier data transfer object.
 *
 * Used for paginated list display, detail query, and form data binding.
 * Fields map to the cus_supplier database table.
 */
class SupplierDTO : public oatpp::DTO {
    DTO_INIT(SupplierDTO, DTO);

    // Supplier unique identifier
    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) {
        info->description = ZH_WORDS_GETTER("sup.field.id");
    }

    // Supplier code / business number (e.g. "GYS-001")
    DTO_FIELD(String, code);
    DTO_FIELD_INFO(code) {
        info->description = ZH_WORDS_GETTER("sup.field.code");
    }

    // Business number / serial number
    DTO_FIELD(String, busNo);
    DTO_FIELD_INFO(busNo) {
        info->description = ZH_WORDS_GETTER("sup.field.busNo");
    }

    // Supplier name
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("sup.field.name");
    }

    // Type enum value (e.g. "DRUG" = drug supplier, "MATERIAL" = material supplier)
    DTO_FIELD(String, typeEnum);
    DTO_FIELD_INFO(typeEnum) {
        info->description = ZH_WORDS_GETTER("sup.field.typeEnum");
    }

    // Unified social credit code
    DTO_FIELD(String, creditCode);
    DTO_FIELD_INFO(creditCode) {
        info->description = ZH_WORDS_GETTER("sup.field.creditCode");
    }

    // Legal representative name
    DTO_FIELD(String, legalRepresentative);
    DTO_FIELD_INFO(legalRepresentative) {
        info->description = ZH_WORDS_GETTER("sup.field.legalRepresentative");
    }

    // Address
    DTO_FIELD(String, address);
    DTO_FIELD_INFO(address) {
        info->description = ZH_WORDS_GETTER("sup.field.address");
    }

    // Pinyin code for quick search
    DTO_FIELD(String, pyStr);
    DTO_FIELD_INFO(pyStr) {
        info->description = ZH_WORDS_GETTER("sup.field.pyStr");
    }

    // Wubi code for quick search
    DTO_FIELD(String, wbStr);
    DTO_FIELD_INFO(wbStr) {
        info->description = ZH_WORDS_GETTER("sup.field.wbStr");
    }

    // Contact phone number
    DTO_FIELD(String, phone);
    DTO_FIELD_INFO(phone) {
        info->description = ZH_WORDS_GETTER("sup.field.phone");
    }

    // Email address
    DTO_FIELD(String, email);
    DTO_FIELD_INFO(email) {
        info->description = ZH_WORDS_GETTER("sup.field.email");
    }

    // Bank name for payment
    DTO_FIELD(String, bankName);
    DTO_FIELD_INFO(bankName) {
        info->description = ZH_WORDS_GETTER("sup.field.bankName");
    }

    // Bank account number
    DTO_FIELD(String, bankAccount);
    DTO_FIELD_INFO(bankAccount) {
        info->description = ZH_WORDS_GETTER("sup.field.bankAccount");
    }

    // Organization ID (which organization this supplier belongs to)
    DTO_FIELD(String, orgId);
    DTO_FIELD_INFO(orgId) {
        info->description = ZH_WORDS_GETTER("sup.field.orgId");
    }

    // Display order for list sorting
    DTO_FIELD(Int32, displayOrder);
    DTO_FIELD_INFO(displayOrder) {
        info->description = ZH_WORDS_GETTER("sup.field.displayOrder");
    }

    // Status enum ("NORMAL" = enabled, "DISABLED" = disabled)
    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = ZH_WORDS_GETTER("sup.field.statusEnum");
    }

    // Remark / note
    DTO_FIELD(String, note);
    DTO_FIELD_INFO(note) {
        info->description = ZH_WORDS_GETTER("sup.field.note");
    }
    // Version for optimistic locking )
    DTO_FIELD(Int32, version);
    DTO_FIELD_INFO(version) {
        info->description = ZH_WORDS_GETTER("sup.field.version");
    }
};

/**
 * SupplierPageDTO - Paginated wrapper for supplier list.
 *
 * Wraps a list of SupplierDTO with pagination metadata
 * (pageIndex, pageSize, total, pages) for frontend table rendering.
 */
class SupplierPageDTO : public PageDTO<SupplierDTO::Wrapper>
{
    DTO_INIT(SupplierPageDTO, PageDTO<SupplierDTO::Wrapper>);
};

/**
 * SupplierBatchEnableDisableDTO - Batch enable/disable request body.
 *
 * Frontend sends JSON: {"ids":["id1","id2"], "statusEnum":"DISABLED"}
 * Backend updates all matching suppliers to the target status.
 */
class SupplierBatchEnableDisableDTO : public oatpp::DTO {
    DTO_INIT(SupplierBatchEnableDisableDTO, DTO);

    // List of supplier IDs to modify
    DTO_FIELD(List<String>, ids);
    DTO_FIELD_INFO(ids) {
        info->description = ZH_WORDS_GETTER("sup.field.ids");
    }

    // Target status value ("NORMAL" = enable, "DISABLED" = disable)
    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = ZH_WORDS_GETTER("sup.field.statusEnum");
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif
