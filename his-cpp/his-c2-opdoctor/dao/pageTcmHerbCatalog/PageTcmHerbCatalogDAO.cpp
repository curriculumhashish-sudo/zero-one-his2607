#include "stdafx.h"
#include "PageTcmHerbCatalogDAO.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace
{
sql::Connection& requireConnection(PageTcmHerbCatalogDAO& dao)
{
    auto* session = dao.getSqlSession();
    if (session == nullptr || session->getConnection() == nullptr)
    {
        throw std::runtime_error("Database connection is unavailable");
    }
    return *session->getConnection();
}

std::string nullableString(
    sql::ResultSet& result,
    const std::string& column)
{
    const auto value = result.getString(column);
    return result.wasNull() ? std::string() : value.asStdString();
}

std::string buildFromAndWhere(const PageTcmHerbCatalogDTO& query)
{
    const bool hasOrganization =
        !query.hospitalOrganizationId.empty();
    const bool hasKeyword = !query.keyword.empty();

    std::string sqlText =
        " FROM med_medication_definition d "
        "INNER JOIN ("
        "SELECT i.tenant_id, i.item_id, MIN(i.unit_code) AS unit_code, "
        "SUM(i.quantity) AS quantity "
        "FROM wkf_inventory_item i "
        "INNER JOIN adm_location l ON l.id = i.location_id "
        "AND l.tenant_id = i.tenant_id "
        "AND l.is_deleted = 0 AND l.status_enum = 'ACTIVE' "
        "WHERE i.item_table = 'med_medication_definition' "
        "AND i.is_deleted = 0 "
        "AND i.inventory_status_enum = 'ACTIVE' "
        "AND (i.expiration_date IS NULL OR i.expiration_date >= CURRENT_DATE) "
        "AND l.form_enum = 'PHARMACY' AND l.name = ? ";

    if (hasOrganization)
    {
        sqlText += "AND l.organization_id = ? ";
    }

    sqlText +=
        "GROUP BY i.tenant_id, i.item_id "
        "HAVING SUM(i.quantity) > 0"
        ") inventory ON inventory.tenant_id = d.tenant_id "
        "AND inventory.item_id = d.id "
        "INNER JOIN ("
        "SELECT p.tenant_id, p.instance_id, ";

    if (hasOrganization)
    {
        sqlText +=
            "CAST(SUBSTRING_INDEX(GROUP_CONCAT(p.price_value ORDER BY "
            "CASE WHEN p.org_id = ? THEN 0 ELSE 1 END, "
            "COALESCE(p.priority, 2147483647), p.id SEPARATOR ','), "
            "',', 1) AS DECIMAL(19,6)) AS retail_price ";
    }
    else
    {
        sqlText +=
            "CAST(SUBSTRING_INDEX(GROUP_CONCAT(p.price_value ORDER BY "
            "COALESCE(p.priority, 2147483647), p.id SEPARATOR ','), "
            "',', 1) AS DECIMAL(19,6)) AS retail_price ";
    }

    sqlText +=
        "FROM adm_charge_item_definition p "
        "WHERE p.instance_table = 'med_medication_definition' "
        "AND p.is_deleted = 0 AND p.status_enum = 'ACTIVE' "
        "AND p.price_type_code = 'SALE' ";

    if (hasOrganization)
    {
        sqlText += "AND (p.org_id = ? OR p.org_id IS NULL) ";
    }
    else
    {
        sqlText += "AND p.org_id IS NULL ";
    }

    sqlText +=
        "GROUP BY p.tenant_id, p.instance_id"
        ") price ON price.tenant_id = d.tenant_id "
        "AND price.instance_id = d.id "
        "WHERE d.tenant_id = ? "
        "AND d.is_deleted = 0 AND d.status_enum = 'ACTIVE' "
        "AND d.active_flag = 1 AND d.category_code = '4' ";

    if (hasOrganization)
    {
        sqlText += "AND d.org_id = ? ";
    }

    if (hasKeyword)
    {
        sqlText +=
            "AND (d.bus_no LIKE ? OR d.name LIKE ? "
            "OR COALESCE(d.total_volume, '') LIKE ?) ";
    }

    return sqlText;
}

int bindFromAndWhere(
    sql::PreparedStatement& statement,
    const PageTcmHerbCatalogDTO& query)
{
    int position = 1;
    const bool hasOrganization =
        !query.hospitalOrganizationId.empty();

    statement.setString(position++, query.inventoryLocationName);
    if (hasOrganization)
    {
        statement.setString(position++, query.hospitalOrganizationId);
        statement.setString(position++, query.hospitalOrganizationId);
        statement.setString(position++, query.hospitalOrganizationId);
    }

    statement.setString(position++, query.tenantId);
    if (hasOrganization)
    {
        statement.setString(position++, query.hospitalOrganizationId);
    }

    if (!query.keyword.empty())
    {
        const std::string pattern = "%" + query.keyword + "%";
        statement.setString(position++, pattern);
        statement.setString(position++, pattern);
        statement.setString(position++, pattern);
    }
    return position;
}

PageTcmHerbCatalogDO mapCatalogItem(sql::ResultSet& result)
{
    PageTcmHerbCatalogDO item;
    item.id = nullableString(result, "id");
    item.code = nullableString(result, "code");
    item.name = nullableString(result, "name");
    item.specification = nullableString(result, "specification");
    item.defaultDose = result.getDouble("default_dose");
    item.doseUnitCode = nullableString(result, "dose_unit_code");
    item.retailPrice = result.getDouble("retail_price");
    item.categoryCode = nullableString(result, "category_code");
    item.inventoryQuantity = result.getDouble("inventory_quantity");
    item.inventoryUnitCode =
        nullableString(result, "inventory_unit_code");
    return item;
}
}

PageTcmHerbCatalogContextDO
PageTcmHerbCatalogDAO::selectUserContext(const std::string& userId)
{
    auto& connection = requireConnection(*this);
    PageTcmHerbCatalogContextDO context;

    const std::string sessionSql =
        "SELECT tenant_id, hospital_organization_id "
        "FROM whale_user_login_session WHERE user_id = ? LIMIT 1";
    std::unique_ptr<sql::PreparedStatement> sessionStatement(
        connection.prepareStatement(sessionSql));
    sessionStatement->setString(1, userId);
    std::unique_ptr<sql::ResultSet> sessionRows(
        sessionStatement->executeQuery());
    if (sessionRows->next())
    {
        context.tenantId = nullableString(*sessionRows, "tenant_id");
        context.hospitalOrganizationId =
            nullableString(*sessionRows, "hospital_organization_id");
        context.found = !context.tenantId.empty();
    }

    if (context.found)
    {
        return context;
    }

    const std::string tenantSql =
        "SELECT tenant_id FROM whale_user_tenants "
        "WHERE user_id = ? AND is_deleted = 0 "
        "ORDER BY updated_at DESC, created_at DESC, id LIMIT 1";
    std::unique_ptr<sql::PreparedStatement> tenantStatement(
        connection.prepareStatement(tenantSql));
    tenantStatement->setString(1, userId);
    std::unique_ptr<sql::ResultSet> tenantRows(
        tenantStatement->executeQuery());
    if (tenantRows->next())
    {
        context.tenantId = nullableString(*tenantRows, "tenant_id");
        context.found = !context.tenantId.empty();
    }
    return context;
}

std::uint64_t PageTcmHerbCatalogDAO::countPageTcmHerbCatalog(
    const PageTcmHerbCatalogDTO& query)
{
    auto& connection = requireConnection(*this);
    const std::string sqlText =
        "SELECT COUNT(*) AS total" + buildFromAndWhere(query);
    std::unique_ptr<sql::PreparedStatement> statement(
        connection.prepareStatement(sqlText));
    bindFromAndWhere(*statement, query);
    std::unique_ptr<sql::ResultSet> rows(statement->executeQuery());
    return rows->next() ? rows->getUInt64("total") : 0;
}

std::vector<PageTcmHerbCatalogDO>
PageTcmHerbCatalogDAO::selectPageTcmHerbCatalog(
    const PageTcmHerbCatalogDTO& query)
{
    auto& connection = requireConnection(*this);
    const std::string unitColumn = query.scope == "inpatient"
        ? "d.def_inhospital_unit_code"
        : "d.def_encounter_unit_code";
    const std::string sqlText =
        "SELECT d.id AS id, d.bus_no AS code, d.name AS name, "
        "COALESCE(d.total_volume, '') AS specification, "
        "COALESCE(d.dose, 10.0) AS default_dose, "
        "COALESCE(" + unitColumn + ", d.dose_unit_code, '') "
        "AS dose_unit_code, price.retail_price AS retail_price, "
        "d.category_code AS category_code, "
        "inventory.quantity AS inventory_quantity, "
        "inventory.unit_code AS inventory_unit_code" +
        buildFromAndWhere(query) +
        "ORDER BY d.bus_no ASC, d.id ASC LIMIT ? OFFSET ?";

    std::unique_ptr<sql::PreparedStatement> statement(
        connection.prepareStatement(sqlText));
    int position = bindFromAndWhere(*statement, query);
    statement->setUInt64(position++, query.pageSize);
    statement->setUInt64(position, query.offset);

    std::unique_ptr<sql::ResultSet> rows(statement->executeQuery());
    std::vector<PageTcmHerbCatalogDO> result;
    result.reserve(static_cast<std::size_t>(query.pageSize));
    while (rows->next())
    {
        result.push_back(mapCatalogItem(*rows));
    }
    return result;
}
