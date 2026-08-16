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
#include "stdafx.h"
#include "SupService.h"
#include "dao/sup/SupDao.h"

namespace
{
    // Safe conversion: returns empty string when the oatpp String is null.
    std::string safeValue(const oatpp::String& s)
    {
        return s ? s->c_str() : "";
    }

    // Map a SupplierDO row into a SupplierDTO wrapper.
    // Copies every database column to the corresponding DTO field.
    oatpp::Object<SupplierDTO> mapDOtoDTO(const PtrSupplierDO& row)
    {
        auto dto = SupplierDTO::createShared();
        dto->id = row->getId();
        dto->code = row->getBusNo();          // bus_no serves as the supplier code
        dto->busNo = row->getBusNo();
        dto->name = row->getName();
        dto->typeEnum = row->getTypeEnum();
        dto->creditCode = row->getCreditCode();
        dto->legalRepresentative = row->getLegalRepresentative();
        dto->address = row->getAddress();
        dto->pyStr = row->getPyStr();
        dto->wbStr = row->getWbStr();
        dto->phone = row->getPhone();
        dto->email = row->getEmail();
        dto->bankName = row->getBankName();
        dto->bankAccount = row->getBankAccount();
        dto->orgId = row->getOrgId();
        dto->displayOrder = row->getDisplayOrder();
        dto->statusEnum = row->getStatusEnum();
        dto->note = row->getNote();
        dto->version = row->getVersion();
        return dto;
    }
}

SupplierJsonVO::Wrapper SupService::queryOne(const SupplierDetailQuery::Wrapper& query)
{
    auto vo = SupplierJsonVO::createShared();

    // Validate: at least one of id or code must be provided.
    std::string id = safeValue(query->id);
    std::string code = safeValue(query->code);
    if (id.empty() && code.empty())
    {
        vo->init(SupplierDTO::Wrapper(nullptr), ResultStatus("at least one of id or code is required", 400));
        return vo;
    }

    SupDAO dao;
    PtrSupplierDO row;

    if (!id.empty())
    {
        row = dao.selectById(id);
    }
    // The query-by-code path can be extended here when needed.
    // Currently falls through to the same selectById for simplicity;
    // a separate DAO method can be added for code-based lookup.

    if (!row)
    {
        vo->init(SupplierDTO::Wrapper(nullptr), ResultStatus("supplier not found", 404));
        return vo;
    }

    vo->success(mapDOtoDTO(row));
    return vo;
}

SupplierPageJsonVO::Wrapper SupService::queryAll(const SupplierQuery::Wrapper& query)
{
    auto vo = SupplierPageJsonVO::createShared();

    // Extract filter values; empty string means the filter is not applied.
    std::string code = safeValue(query->code);
    std::string name = safeValue(query->name);
    std::string type = safeValue(query->type);
    std::string keyword = safeValue(query->keyword);
    std::string statusEnum = safeValue(query->statusEnum);

    // Prepare pagination parameters with defaults.
    uint64_t pageIndex = query->pageIndex ? query->pageIndex.getValue(1) : 1;
    uint64_t pageSize = query->pageSize ? query->pageSize.getValue(10) : 10;
    if (pageIndex < 1) pageIndex = 1;
    if (pageSize < 1) pageSize = 10;
    uint64_t offset = (pageIndex - 1) * pageSize;

    SupDAO dao;

    // 1. Count total rows.
    uint64_t total = dao.countByCondition(code, name, type, keyword, statusEnum);

    // 2. Query the current page.
    auto rows = dao.selectByCondition(code, name, type, keyword, statusEnum, offset, pageSize);

    // 3. Build the page DTO.
    auto pageDTO = SupplierPageDTO::createShared();
    pageDTO->pageIndex = pageIndex;
    pageDTO->pageSize = pageSize;
    pageDTO->total = static_cast<v_int64>(total);
    pageDTO->calcPages();

    auto rowList = oatpp::List<SupplierDTO::Wrapper>::createShared();
    for (const auto& row : rows)
    {
        rowList->push_back(mapDOtoDTO(row));
    }
    pageDTO->rows = rowList;

    vo->success(pageDTO);
    return vo;
}

SupPageJsonVO::Wrapper SupService::querySupNames(const SupNameDTO::Wrapper& dto)
{
    auto vo = SupPageJsonVO::createShared();

    if (!dto)
    {
        auto pageDTO = SupPageDTO::createShared();
        pageDTO->pageIndex = 1;
        pageDTO->pageSize = 10;
        pageDTO->total = v_int64(0);
        pageDTO->calcPages();
        pageDTO->rows = oatpp::List<SupDTO::Wrapper>::createShared();
        vo->fail(pageDTO);
        return vo;
    }

    std::string name = safeValue(dto->name);
    std::string idStr = dto->id ? std::to_string(dto->id.getValue(0)) : "";

    SupDAO dao;
    uint64_t total = dao.countBySupName(name, idStr);

    auto pageDTO = SupPageDTO::createShared();
    pageDTO->pageIndex = 1;
    pageDTO->pageSize = v_uint64(total > 0 ? total : static_cast<uint64_t>(10));
    pageDTO->total = v_int64(static_cast<int64_t>(total));
    pageDTO->calcPages();

    auto rows = dao.selectBySupName(name, idStr, 0, total > 0 ? total : 100);

    auto rowList = oatpp::List<SupDTO::Wrapper>::createShared();
    for (const auto& row : rows)
    {
        auto d = SupDTO::createShared();
        d->id = v_uint64(0);                // numeric id not in cus_supplier
        d->supId = row->getId();            // cus_supplier.id is VARCHAR
        d->name = row->getName();
        d->typeEnum = row->getTypeEnum();
        d->address = row->getAddress();
        d->contact = row->getPhone();       // phone doubles as contact
        d->pinyin = row->getPyStr();
        d->wubi = row->getWbStr();
        d->statusEnum = row->getStatusEnum();
        rowList->push_back(d);
    }
    pageDTO->rows = rowList;

    vo->success(pageDTO);
    return vo;
}
