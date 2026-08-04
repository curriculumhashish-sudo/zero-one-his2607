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
#include "stdafx.h"
#include "PharmDAO.h"
#include "PharmMapper.h"
#include "PharmNameMapper.h"
#include <sstream>

std::string PharmDAO::queryConditionBuilder(const PharmQuery::Wrapper& query, SqlParams& params)
{
    std::stringstream sqlCondition;
    sqlCondition << " WHERE is_deleted = 0";

    // Only return pharmacy/warehouse form_enum rows
    sqlCondition << " AND form_enum IN ('PHARMACY', 'WAREHOUSE')";

    if (query->keyword)
    {
        sqlCondition << " AND name LIKE CONCAT('%',?,'%')";
        SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
    }

    if (query->formEnum)
    {
        sqlCondition << " AND form_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->formEnum.getValue(""));
    }

    if (query->statusEnum)
    {
        sqlCondition << " AND status_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->statusEnum.getValue(""));
    }

    return sqlCondition.str();
}

uint64_t PharmDAO::count(const PharmQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM adm_location";
    sql += queryConditionBuilder(query, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PtrPharmDO> PharmDAO::selectPage(const PharmQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql =
        "SELECT id, tenant_id, name, bus_no, status_enum, operational_enum, "
        "form_enum, mode_enum, type_json, organization_id, display_order, "
        "py_str, wb_str, parent_id, created_by, updated_by, created_at, updated_at "
        "FROM adm_location";
    sql += queryConditionBuilder(query, params);
    sql += " ORDER BY COALESCE(display_order, 2147483647) ASC, name ASC, id DESC";

    uint64_t offset = (query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10);
    uint64_t size = query->pageSize.getValue(10);
    sql += " LIMIT " + std::to_string(offset) + ", " + std::to_string(size);

    return sqlSession->executeQuery<PtrPharmDO>(sql, PharmMapper(), params);
}

int PharmDAO::insert(const PharmDO& data)
{
    // Delegate to BaseDAO::insert — PK id must be set (SnowFlake generated) before calling
    return BaseDAO::insert(data);
}

int PharmDAO::update(const PharmDO& data)
{
    std::string sql =
        "UPDATE adm_location SET "
        "name = ?, form_enum = ?, organization_id = ?, display_order = ?, "
        "py_str = ?, wb_str = ?, parent_id = ?, updated_at = ?, updated_by = ? "
        "WHERE id = ? AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, data.getName());
    SQLPARAMS_PUSH(params, "s", std::string, data.getFormEnum());
    SQLPARAMS_PUSH(params, "s", std::string, data.getOrganizationId());
    SQLPARAMS_PUSH(params, "i", int, data.getDisplayOrder());
    SQLPARAMS_PUSH(params, "s", std::string, data.getPyStr());
    SQLPARAMS_PUSH(params, "s", std::string, data.getWbStr());
    SQLPARAMS_PUSH(params, "s", std::string, data.getParentId());
    SQLPARAMS_PUSH(params, "dt", std::string, data.getUpdatedAt());
    SQLPARAMS_PUSH(params, "s", std::string, data.getUpdatedBy());
    SQLPARAMS_PUSH(params, "s", std::string, data.getId());

    return sqlSession->executeUpdate(sql, params);
}

int PharmDAO::remove(const std::string& id)
{
    std::string sql = "UPDATE adm_location SET is_deleted = 1 WHERE id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    return sqlSession->executeUpdate(sql, params);
}

std::list<PtrPharmDO> PharmDAO::nameList(const std::string& formEnum)
{
    std::string sql =
        "SELECT id, name FROM adm_location "
        "WHERE is_deleted = 0 AND status_enum = 'ACTIVE' "
        "AND form_enum IN ('PHARMACY', 'WAREHOUSE')";
    SqlParams params;
    if (!formEnum.empty())
    {
        sql += " AND form_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, formEnum);
    }
    sql += " ORDER BY COALESCE(display_order, 2147483647) ASC, name ASC, id DESC";
    return sqlSession->executeQuery<PtrPharmDO>(sql, PharmNameMapper(), params);
}

bool PharmDAO::existsName(const std::string& name, const std::string& excludeId)
{
    std::string sql =
        "SELECT COUNT(1) FROM adm_location "
        "WHERE name = ? AND is_deleted = 0 "
        "AND form_enum IN ('PHARMACY', 'WAREHOUSE')";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, name);
    if (!excludeId.empty())
    {
        sql += " AND id <> ?";
        SQLPARAMS_PUSH(params, "s", std::string, excludeId);
    }
    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

int PharmDAO::updateStatus(const std::string& id, const std::string& statusEnum)
{
    std::string sql = "UPDATE adm_location SET status_enum = ? WHERE id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
    SQLPARAMS_PUSH(params, "s", std::string, id);
    return sqlSession->executeUpdate(sql, params);
}
