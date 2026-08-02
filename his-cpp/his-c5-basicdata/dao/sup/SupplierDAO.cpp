#include "stdafx.h"
#include "SupplierDAO.h"

#include "SupplierMapper.h"

#include <sstream>

namespace
{
    // All columns of cus_supplier used for row reads.
    const char* SUPPLIER_COLUMNS =
        "id, tenant_id, created_at, updated_at, created_by, updated_by, is_deleted, "
        "deleted_at, deleted_by, version, bus_no, name, type_enum, credit_code, "
        "legal_representative, address, py_str, wb_str, phone, email, bank_name, "
        "bank_account, org_id, display_order, status_enum, note";

    // Appends an assignment to the dynamic SET clause, inserting ", " between items.
    void appendAssignment(
        std::ostringstream& sql,
        bool& first,
        const std::string& column)
    {
        if (!first)
            sql << ", ";
        first = false;
        sql << column;
    }

    // Appends "column = ?" only when the DTO field is non-null (patch semantics).
    void appendStringPatch(
        const std::string& column,
        const oatpp::String& value,
        std::ostringstream& sql,
        SqlParams& params,
        bool& first)
    {
        if (!value)
            return; // null field is not patched
        appendAssignment(sql, first, column + " = ?");
        SQLPARAMS_PUSH(params, "s", std::string, value->c_str());
    }
}

std::string SupplierDAO::selectTenantIdByUserId(const std::string& userId)
{
    // Latest login-session row that carries a non-null tenant_id for the user.
    const std::string sql =
        "SELECT tenant_id FROM whale_user_login_session "
        "WHERE user_id = ? AND tenant_id IS NOT NULL "
        "ORDER BY updated_at DESC LIMIT 1";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, userId);
    return sqlSession->executeQueryOne<std::string>(
        sql, SupplierStringMapper("tenant_id"), params);
}

PtrSupplierDO SupplierDAO::selectByIdForUpdate(const std::string& id)
{
    // FOR UPDATE serializes concurrent writers on the row inside the transaction.
    const std::string sql =
        std::string("SELECT ") + SUPPLIER_COLUMNS +
        " FROM cus_supplier WHERE id = ? AND is_deleted = 0 "
        "LIMIT 1 FOR UPDATE";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    return sqlSession->executeQueryOne<PtrSupplierDO>(
        sql, SupplierMapper(), params);
}

bool SupplierDAO::existsName(
    const std::string& tenantId,
    const std::string& name,
    const std::string& excludeId)
{
    std::string sql =
        "SELECT COUNT(1) FROM cus_supplier "
        "WHERE tenant_id = ? AND name = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, name);
    if (!excludeId.empty())
    {
        sql += " AND id <> ?";
        SQLPARAMS_PUSH(params, "s", std::string, excludeId);
    }
    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

int SupplierDAO::insertSupplier(const SupplierDO& data)
{
    // is_deleted and version are hard-coded to their initial values (0, 0);
    // genuinely nullable columns are bound as "nul" when empty, NOT NULL
    // columns (bus_no/name/type_enum/credit_code/py_str/wb_str/org_id/status_enum)
    // are always bound as "s" because an empty string is still non-NULL.
    const std::string sql =
        "INSERT INTO cus_supplier ("
        "id, tenant_id, created_at, created_by, is_deleted, version, "
        "bus_no, name, type_enum, credit_code, legal_representative, address, "
        "py_str, wb_str, phone, email, bank_name, bank_account, "
        "org_id, display_order, status_enum, note) "
        "VALUES (?, ?, ?, ?, 0, 0, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, data.getId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getTenantId());
    SQLPARAMS_PUSH(params, "dt", std::string, data.getCreatedAt());
    SQLPARAMS_PUSH(params, "s", std::string, data.getCreatedBy());
    SQLPARAMS_PUSH(params, "s", std::string, data.getBusNo());
    SQLPARAMS_PUSH(params, "s", std::string, data.getName());
    SQLPARAMS_PUSH(params, "s", std::string, data.getTypeEnum());
    SQLPARAMS_PUSH(params, "s", std::string, data.getCreditCode());

    if (data.getLegalRepresentative().empty())
        SQLPARAMS_PUSH(params, "nul", int, 1);
    else
        SQLPARAMS_PUSH(params, "s", std::string, data.getLegalRepresentative());
    if (data.getAddress().empty())
        SQLPARAMS_PUSH(params, "nul", int, 1);
    else
        SQLPARAMS_PUSH(params, "s", std::string, data.getAddress());

    SQLPARAMS_PUSH(params, "s", std::string, data.getPyStr());
    SQLPARAMS_PUSH(params, "s", std::string, data.getWbStr());

    if (data.getPhone().empty())
        SQLPARAMS_PUSH(params, "nul", int, 1);
    else
        SQLPARAMS_PUSH(params, "s", std::string, data.getPhone());
    if (data.getEmail().empty())
        SQLPARAMS_PUSH(params, "nul", int, 1);
    else
        SQLPARAMS_PUSH(params, "s", std::string, data.getEmail());
    if (data.getBankName().empty())
        SQLPARAMS_PUSH(params, "nul", int, 1);
    else
        SQLPARAMS_PUSH(params, "s", std::string, data.getBankName());
    if (data.getBankAccount().empty())
        SQLPARAMS_PUSH(params, "nul", int, 1);
    else
        SQLPARAMS_PUSH(params, "s", std::string, data.getBankAccount());

    SQLPARAMS_PUSH(params, "s", std::string, data.getOrgId());
    SQLPARAMS_PUSH(params, "i", int, data.getDisplayOrder());
    SQLPARAMS_PUSH(params, "s", std::string, data.getStatusEnum());
    if (data.getNote().empty())
        SQLPARAMS_PUSH(params, "nul", int, 1);
    else
        SQLPARAMS_PUSH(params, "s", std::string, data.getNote());

    return sqlSession->executeUpdate(sql, params);
}

int SupplierDAO::updatePatch(
    const SupplierUpdateDTO& dto,
    const std::string& userId,
    int expectedVersion)
{
    // Only non-null DTO fields are SET (patch semantics); status_enum is not
    // part of the update DTO, so the status can never be changed here.
    std::ostringstream sql;
    sql << "UPDATE cus_supplier SET ";
    SqlParams params;
    bool first = true;

    appendStringPatch("name", dto.name, sql, params, first);
    appendStringPatch("type_enum", dto.typeEnum, sql, params, first);
    appendStringPatch("credit_code", dto.creditCode, sql, params, first);
    appendStringPatch("legal_representative", dto.legalRepresentative, sql, params, first);
    appendStringPatch("address", dto.address, sql, params, first);
    appendStringPatch("py_str", dto.pyStr, sql, params, first);
    appendStringPatch("wb_str", dto.wbStr, sql, params, first);
    appendStringPatch("phone", dto.phone, sql, params, first);
    appendStringPatch("email", dto.email, sql, params, first);
    appendStringPatch("bank_name", dto.bankName, sql, params, first);
    appendStringPatch("bank_account", dto.bankAccount, sql, params, first);
    appendStringPatch("org_id", dto.orgId, sql, params, first);
    appendStringPatch("note", dto.note, sql, params, first);

    if (dto.displayOrder)
    {
        appendAssignment(sql, first, "display_order = ?");
        SQLPARAMS_PUSH(params, "i", int, dto.displayOrder.getValue(0));
    }

    // Audit fields are always refreshed on every update.
    appendAssignment(sql, first, "updated_at = NOW(6)");
    appendAssignment(sql, first, "updated_by = ?");
    SQLPARAMS_PUSH(params, "s", std::string, userId);
    // Optimistic lock: bump the version and require the caller's expected version.
    appendAssignment(sql, first, "version = version + 1");
    sql << " WHERE id = ? AND version = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "s", std::string, dto.id.getValue(""));
    SQLPARAMS_PUSH(params, "i", int, expectedVersion);
    return sqlSession->executeUpdate(sql.str(), params);
}
