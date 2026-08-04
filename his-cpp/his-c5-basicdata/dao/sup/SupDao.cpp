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
#include "SupDao.h"
#include "SupplierMapper.h"

#include <sstream>

namespace
{
    // All columns of cus_supplier used for SELECT queries.
    const char* SUPPLIER_COLUMNS =
        "id, tenant_id, created_at, updated_at, created_by, updated_by, is_deleted, "
        "deleted_at, deleted_by, version, bus_no, name, type_enum, credit_code, "
        "legal_representative, address, py_str, wb_str, phone, email, bank_name, "
        "bank_account, org_id, display_order, status_enum, note";

    // Append a dynamic WHERE clause fragment and its parameter for fuzzy matching.
    // Wraps the column in LIKE %?% and adds the parameter to params.
    void appendFuzzyCondition(
        const std::string& column,
        const std::string& value,
        std::ostringstream& sql,
        SqlParams& params)
    {
        sql << " AND " << column << " LIKE ?";
        std::string likeValue = "%" + value + "%";
        SQLPARAMS_PUSH(params, "s", std::string, likeValue);
    }

    // Append a dynamic WHERE clause fragment for exact matching.
    void appendExactCondition(
        const std::string& column,
        const std::string& value,
        std::ostringstream& sql,
        SqlParams& params)
    {
        sql << " AND " << column << " = ?";
        SQLPARAMS_PUSH(params, "s", std::string, value);
    }

    // Build the common WHERE clause with dynamic filters.
    // Returns the populated SqlParams for reuse across count and select queries.
    void buildWhereClause(
        const std::string& code,
        const std::string& name,
        const std::string& type,
        const std::string& keyword,
        const std::string& statusEnum,
        std::ostringstream& sql,
        SqlParams& params)
    {
        sql << " WHERE is_deleted = 0";

        if (!code.empty())
            appendFuzzyCondition("bus_no", code, sql, params);

        if (!name.empty())
            appendFuzzyCondition("name", name, sql, params);

        if (!type.empty())
            appendExactCondition("type_enum", type, sql, params);

        if (!keyword.empty())
        {
            // Keyword searches across name and bus_no (OR relationship).
            sql << " AND (name LIKE ? OR bus_no LIKE ?)";
            std::string likeKeyword = "%" + keyword + "%";
            SQLPARAMS_PUSH(params, "s", std::string, likeKeyword);
            SQLPARAMS_PUSH(params, "s", std::string, likeKeyword);
        }

        if (!statusEnum.empty())
            appendExactCondition("status_enum", statusEnum, sql, params);
    }
}

PtrSupplierDO SupDAO::selectById(const std::string& id)
{
    const std::string sql =
        std::string("SELECT ") + SUPPLIER_COLUMNS +
        " FROM cus_supplier WHERE id = ? AND is_deleted = 0 LIMIT 1";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    return sqlSession->executeQueryOne<PtrSupplierDO>(
        sql, SupplierMapper(), params);
}

uint64_t SupDAO::countByCondition(
    const std::string& code,
    const std::string& name,
    const std::string& type,
    const std::string& keyword,
    const std::string& statusEnum)
{
    std::ostringstream sql;
    sql << "SELECT COUNT(1) FROM cus_supplier";
    SqlParams params;
    buildWhereClause(code, name, type, keyword, statusEnum, sql, params);
    return sqlSession->executeQueryNumerical(sql.str(), params);
}

std::list<PtrSupplierDO> SupDAO::selectByCondition(
    const std::string& code,
    const std::string& name,
    const std::string& type,
    const std::string& keyword,
    const std::string& statusEnum,
    uint64_t offset,
    uint64_t limit)
{
    std::ostringstream sql;
    sql << "SELECT " << SUPPLIER_COLUMNS << " FROM cus_supplier";
    SqlParams params;
    buildWhereClause(code, name, type, keyword, statusEnum, sql, params);
    sql << " ORDER BY COALESCE(display_order, 2147483647) ASC, name ASC, id ASC";
    sql << " LIMIT ? OFFSET ?";
    SQLPARAMS_PUSH(params, "ull", uint64_t, limit);
    SQLPARAMS_PUSH(params, "ull", uint64_t, offset);

    return sqlSession->executeQuery<PtrSupplierDO>(
        sql.str(), SupplierMapper(), params);
}
