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

// ============================================================
// c5-pharm real implementations (via PharmService)
// ============================================================

// GET /pharm/list — Paginated query
PharmPageJsonVO::Wrapper PharmController::execGetPharmList(const PharmQuery::Wrapper& query)
{
    PharmService service;
    return service.getPage(query);
}

// POST /pharm — Add pharmacy/warehouse
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

// PUT /pharm — Modify pharmacy/warehouse
StringJsonVO::Wrapper PharmController::execModifyPharm(const PharmModifyDTO::Wrapper& dto)
{
    PharmService service;
    return service.modify(dto);
}

// DELETE /pharm/{id} — Remove pharmacy/warehouse
StringJsonVO::Wrapper PharmController::execRemovePharm(const String& id)
{
    uint64_t idNum = std::stoull(id->c_str());
    PharmService service;
    return service.remove(idNum);
}

// PUT /pharm/status — Toggle status
StringJsonVO::Wrapper PharmController::execUpdatePharmStatus(uint64_t id, int status)
{
    PharmService service;
    return service.updateStatus(id, status);
}

// ============================================================
// c5 (mengsuibizhi) stub implementations
// ============================================================

// GET /pharm/select-list — Dropdown selection list (stub)
PharmSelectListJsonVO::Wrapper PharmController::execSelectList(const String& type)
{
    auto jvo = PharmSelectListJsonVO::createShared();
    // TODO: Replace with Service layer call
    jvo->fail({});
    return jvo;
}

// GET /pharm/query-all — Paginated query via GET (stub)
PharmPageJsonVO::Wrapper PharmController::execQueryAll(const PharmQuery::Wrapper& query)
{
    auto jvo = PharmPageJsonVO::createShared();
    // TODO: Replace with Service layer call
    jvo->fail({});
    return jvo;
}

// POST /pharm/statusEnum/modify — Status modify via enum (stub)
StringJsonVO::Wrapper PharmController::execModifyPharmStatusEnum(const PharmModifyStatusEnumDTO::Wrapper& dto)
{
    auto jvo = StringJsonVO::createShared();
    // TODO: Replace with Service layer call
    jvo->fail({});
    return jvo;
}

// POST /pharm/list/query — Paginated query via POST (stub)
PharmPageJsonVO::Wrapper PharmController::execQueryPharmList(const PharmQuery::Wrapper& query)
{
    auto jvo = PharmPageJsonVO::createShared();
    // TODO: Replace with Service layer call
    jvo->fail({});
    return jvo;
}
