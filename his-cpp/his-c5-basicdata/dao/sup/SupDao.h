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
#ifndef _SUP_DAO_H_
#define _SUP_DAO_H_

#include "BaseDAO.h"
#include "domain/do/sup/SupplierDO.h"
#include <list>
#include <string>

/**
 * SupDAO - Data access layer for supplier query operations.
 *
 * Handles read-only queries against the cus_supplier table.
 * Write operations (add/update/delete/batch-status) are in SupplierDAO.
 */
class SupDAO : public BaseDAO
{
public:
    // Query a single supplier by primary key (read-only, no FOR UPDATE lock).
    // Returns nullptr when the row does not exist or is soft-deleted.
    PtrSupplierDO selectById(const std::string& id);

    // Count rows matching the dynamic filter conditions.
    // Used before the paginated query to compute total/pages.
    uint64_t countByCondition(
        const std::string& code,
        const std::string& name,
        const std::string& type,
        const std::string& keyword,
        const std::string& statusEnum);

    // Paginated query with dynamic filter conditions.
    // Returns rows ordered by display_order ASC, name ASC, id ASC.
    std::list<PtrSupplierDO> selectByCondition(
        const std::string& code,
        const std::string& name,
        const std::string& type,
        const std::string& keyword,
        const std::string& statusEnum,
        uint64_t offset,
        uint64_t limit);
};

#endif // !_SUP_DAO_H_
