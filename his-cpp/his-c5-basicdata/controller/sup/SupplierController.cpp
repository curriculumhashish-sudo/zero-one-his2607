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
#include "stdafx.h"
#include "SupplierController.h"

// Query single supplier by ID (stub - returns empty)
SupplierJsonVO::Wrapper SupplierController::execQueryOne(const String& id)
{
    return {};
}

// Paginated query (stub - returns empty)
SupplierPageJsonVO::Wrapper SupplierController::execQueryAll(const SupplierQuery::Wrapper& query)
{
    return {};
}

// Add supplier (stub - returns empty)
StringJsonVO::Wrapper SupplierController::execAddSupplier(const SupplierDTO::Wrapper& dto)
{
    return {};
}

// Modify supplier (stub - returns empty)
StringJsonVO::Wrapper SupplierController::execModifySupplier(const SupplierDTO::Wrapper& dto)
{
    return {};
}

// Batch enable/disable suppliers (stub - returns empty)
StringJsonVO::Wrapper SupplierController::execBatchEnableDisable(const SupplierBatchEnableDisableDTO::Wrapper& dto)
{
    return {};
}

// Batch delete suppliers (stub - returns empty)
ListJsonVO<String>::Wrapper SupplierController::execBatchRemove(const List<String>& ids)
{
    return {};
}