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
#ifndef _SUP_SERVICE_H_
#define _SUP_SERVICE_H_

#include "domain/vo/sup/SupplierVO.h"
#include "domain/vo/sup/SupVO.h"
#include "domain/query/sup/SupplierQuery.h"
#include "domain/dto/sup/SupDTO.h"

/**
 * SupService - Business logic for supplier query operations.
 *
 * Provides query-one (detail), query-all (paginated list),
 * and query-sup-name (name search for dropdown) methods.
 * Write operations (add/update/delete/batch) are in SupplierService.
 */
class SupService
{
public:
    // Query a single supplier by ID or code.
    // Returns SupplierJsonVO with SupplierDTO data on success,
    // or fail status when the supplier is not found.
    SupplierJsonVO::Wrapper queryOne(const SupplierDetailQuery::Wrapper& query);

    // Paginated query with dynamic filter conditions.
    // Returns SupplierPageJsonVO with page metadata and row list.
    SupplierPageJsonVO::Wrapper queryAll(const SupplierQuery::Wrapper& query);

    // Query supplier names by name keyword or id.
    // Returns SupPageJsonVO with SupDTO list for dropdown/selector use.
    SupPageJsonVO::Wrapper querySupNames(const SupNameDTO::Wrapper& dto);
};

#endif // !_SUP_SERVICE_H_
