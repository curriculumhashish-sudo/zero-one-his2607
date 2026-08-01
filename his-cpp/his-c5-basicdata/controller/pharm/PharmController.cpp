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
#include "stdafx.h"
#include "PharmController.h"
#include "../../service/pharm/PharmService.h"

// 1. GET /pharm/name-list — Pharmacy/warehouse name list (id + name, optional type filter)
PharmNameListJsonVO::Wrapper PharmController::execNameList(const String& type)
{
    PharmService service;
    return service.nameList(type);
}

// 2. GET /pharm/list — Paginated query list
PharmPageJsonVO::Wrapper PharmController::execGetPharmList(const PharmQuery::Wrapper& query)
{
    PharmService service;
    return service.getPage(query);
}

// 3. POST /pharm — Add pharmacy/warehouse
StringJsonVO::Wrapper PharmController::execAddPharm(const PharmAddDTO::Wrapper& dto)
{
    std::string err = dto->validate();
    if (!err.empty()) {
        auto result = StringJsonVO::createShared();
        result->fail(oatpp::String(err.c_str()));
        return result;
    }

    PharmService service;
    return service.add(dto);
}

// 4. PUT /pharm — Modify pharmacy/warehouse
StringJsonVO::Wrapper PharmController::execModifyPharm(const PharmModifyDTO::Wrapper& dto)
{
    PharmService service;
    return service.modify(dto);
}

// 5. DELETE /pharm/{id} — Remove pharmacy/warehouse
StringJsonVO::Wrapper PharmController::execRemovePharm(const String& id)
{
    uint64_t idNum = std::stoull(id->c_str());
    PharmService service;
    return service.remove(idNum);
}

// 6. PUT /pharm/status — Toggle enable/disable status
StringJsonVO::Wrapper PharmController::execUpdatePharmStatus(uint64_t id, int status)
{
    PharmService service;
    return service.updateStatus(id, status);
}
