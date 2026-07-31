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
#include "BaseDAO.h"
#include "SimpleDateTimeFormat.h"

// PUT /sup/batch-enable-disable
StringJsonVO::Wrapper SupplierController::execBatchEnableDisable(
    const SupplierBatchEnableDisableDTO::Wrapper& dto)
{
    auto vo = StringJsonVO::createShared();

    if (!dto->ids || dto->ids->size() == 0)
    {
        vo->fail(oatpp::String("supplier id list is empty"));
        return vo;
    }

    std::string status = dto->statusEnum.getValue("");
    if (status != "NORMAL" && status != "DISABLED")
    {
        vo->fail(oatpp::String("invalid status, use NORMAL or DISABLED"));
        return vo;
    }

    std::stringstream sql;
    sql << "UPDATE cus_supplier SET status_enum = ? WHERE id IN (";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, status);

    bool first = true;
    for (const auto& id : *dto->ids)
    {
        if (first) first = false;
        else sql << ",";
        sql << "?";
        SQLPARAMS_PUSH(params, "s", std::string, id.getValue(""));
    }
    sql << ") AND is_deleted = 0";

    BaseDAO dao;
    int affected = dao.getSqlSession()->executeUpdate(sql.str(), params);

    if (affected > 0)
    {
        std::string msg = std::string("ok, ") + std::to_string(affected) + std::string(" rows updated");
        vo->success(oatpp::String(msg));
    }
    else
    {
        vo->fail(oatpp::String("no matching supplier found"));
    }

    return vo;
}

// GET /sup/query-one — Query single supplier by ID or code (stub)
SupplierJsonVO::Wrapper SupplierController::execQueryOne(const SupplierDetailQuery::Wrapper& query)
{
    // TODO: Build dynamic SQL from query->id or query->code; return single SupplierDTO
    return {};
}

// Paginated query with code/name/type/keyword/statusEnum filters (stub)
SupplierPageJsonVO::Wrapper SupplierController::execQueryAll(const SupplierQuery::Wrapper& query)
{
    // TODO: Build dynamic SQL from query->code, query->name, query->type,
    //       query->keyword, query->statusEnum; paginate with query->pageIndex/pageSize
    return {};
}

// DELETE /sup/batch-remove
ListJsonVO<String>::Wrapper SupplierController::execBatchRemove(const List<String>& ids)
{
    auto vo = ListJsonVO<String>::createShared();

    if (!ids || ids->size() == 0)
    {
        vo->fail(oatpp::List<oatpp::String>::createShared());
        return vo;
    }

    std::stringstream sql;
    sql << "UPDATE cus_supplier SET is_deleted = 1, deleted_at = ?, deleted_by = ? WHERE id IN (";

    std::string now = SimpleDateTimeFormat::format();
    SqlParams params;
    SQLPARAMS_PUSH(params, "dt", std::string, now);
    SQLPARAMS_PUSH(params, "s", std::string, "system");

    std::list<std::string> idList;
    bool first = true;
    for (const auto& id : *ids)
    {
        if (first) first = false;
        else sql << ",";
        sql << "?";
        SQLPARAMS_PUSH(params, "s", std::string, id.getValue(""));
        idList.push_back(id.getValue(""));
    }
    sql << ") AND is_deleted = 0";

    BaseDAO dao;
    int affected = dao.getSqlSession()->executeUpdate(sql.str(), params);

    if (affected > 0)
    {
        auto successIds = oatpp::List<oatpp::String>::createShared();
        for (const auto& id : idList)
            successIds->push_back(oatpp::String(id));
        vo->success(successIds);
    }
    else
    {
        vo->fail(oatpp::List<oatpp::String>::createShared());
    }

    return vo;
}