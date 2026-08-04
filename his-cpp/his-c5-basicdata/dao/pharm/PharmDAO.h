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
#ifndef _PHARMDAO_H_
#define _PHARMDAO_H_

#include "BaseDAO.h"
#include "domain/do/pharm/PharmDO.h"
#include "domain/query/pharm/PharmQuery.h"
#include <list>
#include <string>

/**
 * PharmDAO - Data access for pharmacy/warehouse (adm_location table).
 */
class PharmDAO : public BaseDAO
{
public:
    // Paginated query for pharmacy/warehouse list
    std::list<PtrPharmDO> selectPage(const PharmQuery::Wrapper& query);
    // Count records matching filter conditions
    uint64_t count(const PharmQuery::Wrapper& query);
    // Name list for dropdown: query id + name, optional form_enum filter
    std::list<PtrPharmDO> nameList(const std::string& formEnum);
    // Check if a pharmacy/warehouse name already exists (for duplicate validation)
    bool existsName(const std::string& name, const std::string& excludeId = "");
    // Insert a new pharmacy/warehouse (delegates to BaseDAO::insert)
    int insert(const PharmDO& data);
    // Update pharmacy/warehouse info
    int update(const PharmDO& data);
    // Soft-delete pharmacy/warehouse (logical delete: is_deleted = 1)
    int remove(const std::string& id);
    // Update pharmacy/warehouse status_enum
    int updateStatus(const std::string& id, const std::string& statusEnum);

private:
    // Build dynamic WHERE clause from query parameters
    std::string queryConditionBuilder(const PharmQuery::Wrapper& query, SqlParams& params);
};

#endif // !_PHARMDAO_H_
