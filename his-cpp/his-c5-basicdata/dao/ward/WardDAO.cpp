


#include"WardDAO.h"

std::list<WardTreeDO> WardDAO::selectTreeRows(
    const std::string& organizationId)
{
    const std::string sql = R"(
        SELECT
            id,
            name,
            form_enum,
            parent_id,
            status_enum,
            operational_enum,
            display_order
        FROM adm_location
        WHERE organization_id = ?
          AND is_deleted = 0
          AND form_enum IN ('WARD', 'HOUSE', 'BED')
        ORDER BY
          COALESCE(display_order, 2147483647),
          name,
          id
    )";

    SqlParams params;

    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        organizationId
    );

    return sqlSession->executeQuery<WardTreeDO>(
        sql,
        WardTreeMapper(),
        params
    );
}
std::string WardDAO::
selectOrganizationTenantIdForUpdate(
    const std::string& organizationId)
{
    const std::string sql = R"(
        SELECT tenant_id
        FROM adm_organization
        WHERE id = ?
          AND is_deleted = 0
        LIMIT 1
        FOR UPDATE
    )";

    SqlParams params;
    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        organizationId
    );

    return sqlSession
        ->executeQueryOne<std::string>(
            sql,
            OrganizationTenantIdMapper(),
            params
        );
}
// 查询父级完整记录PtrWardLocationDO
PtrWardLocationDO WardDAO::
selectLocationByIdForUpdate(
    const std::string& id)
{
    const std::string sql = R"(
        SELECT
            id,
            tenant_id,
            name,
            bus_no,
            status_enum,
            operational_enum,
            form_enum,
            mode_enum,
            organization_id,
            display_order,
            parent_id
        FROM adm_location
        WHERE id = ?
          AND is_deleted = 0
        LIMIT 1
        FOR UPDATE
    )";

    SqlParams params;
    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        id
    );

    return sqlSession
        ->executeQueryOne<PtrWardLocationDO>(
            sql,
            WardLocationMapper(),
            params
        );
}

bool WardDAO::existsSiblingName(
    const std::string& organizationId,
    const std::string& formEnum,
    const std::string& parentId,
    const std::string& name)
{
    SqlParams params;

    std::string sql = R"(
        SELECT COUNT(1)
        FROM adm_location
        WHERE organization_id = ?
          AND form_enum = ?
          AND name = ?
          AND is_deleted = 0
    )";

    SQLPARAMS_PUSH(
        params, "s", std::string, organizationId);
    SQLPARAMS_PUSH(
        params, "s", std::string, formEnum);
    SQLPARAMS_PUSH(
        params, "s", std::string, name);

    if (parentId.empty())
    {
        sql += " AND parent_id IS NULL";
    }
    else
    {
        sql += " AND parent_id = ?";
        SQLPARAMS_PUSH(
            params, "s", std::string, parentId);
    }

    return sqlSession->executeQueryNumerical(
        sql,
        params
    ) > 0;
}

int WardDAO::insertLocation(
    const WardLocationDO& data)
{
    return insert(data);
}