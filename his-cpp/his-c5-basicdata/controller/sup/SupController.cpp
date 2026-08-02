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
#include "SupController.h"
#include "service/sup/SupService.h"

// GET /sup/query-one — Query single supplier by ID or code
SupplierJsonVO::Wrapper SupController::execQueryOne(const SupplierDetailQuery::Wrapper& query)
{
    // Validate: query object must be present and contain at least one lookup key.
    if (!query)
    {
        auto vo = SupplierJsonVO::createShared();
        vo->init(SupplierDTO::Wrapper(nullptr), ResultStatus("query parameter is required, provide id or code", 400));
        return vo;
    }

    SupService service;
    return service.queryOne(query);
}

// GET /sup/query-all — Paginated query with dynamic filter conditions
SupplierPageJsonVO::Wrapper SupController::execQueryAll(const SupplierQuery::Wrapper& query)
{
    if (!query)
    {
        auto vo = SupplierPageJsonVO::createShared();
        vo->init(SupplierPageDTO::Wrapper(nullptr), ResultStatus("query parameter is required", 400));
        return vo;
    }

    SupService service;
    return service.queryAll(query);
}

// POST /sup/statusEnum/modify — Modify supplier name/status (existing stub)
StringJsonVO::Wrapper SupController::execQuerySupName(const SupNameDTO::Wrapper& dto)
{
    return {};
}
