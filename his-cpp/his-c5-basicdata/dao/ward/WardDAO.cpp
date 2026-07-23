#include <iomanip>
#include <iostream>
#include "stdafx.h"
#include "WardDAO.h"
#include "WardMapper.h"






std::list<WardDO> WardDAO::selectWardList(
    const WardListQuery::Wrapper& query)
{
    SqlParams params;

    std::string sql = R"(
        SELECT id, name, status_enum, display_order
        FROM adm_location
        WHERE organization_id = ?
          AND form_enum = 'WARD'
          AND is_deleted = 0
    )";

    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        query->organizationId.getValue("")
    );

    if (query->keyword &&
        !query->keyword.getValue("").empty())
    {
        sql += " AND name LIKE CONCAT('%', ?, '%')";
        SQLPARAMS_PUSH(
            params,
            "s",
            std::string,
            query->keyword.getValue("")
        );
    }

    if (query->statusEnum &&
        !query->statusEnum.getValue("").empty())
    {
        sql += " AND status_enum = ?";
        SQLPARAMS_PUSH(
            params,
            "s",
            std::string,
            query->statusEnum.getValue("")
        );
    }

    sql += " ORDER BY display_order, name";

    return sqlSession->executeQuery<WardDO>(
        sql,
        WardMapper(),
        params
    );
}
std::list<WardDO> WardDAO::selectNameList(
    const std::string& organizationId)
{
    SqlParams params;

    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        organizationId
    );

    const std::string sql = R"(
    const std::string sql = R"(
    SELECT id, name
    FROM adm_location
    WHERE organization_id = ?
      AND form_enum = 'WARD'
      AND is_deleted = 0
      AND status_enum = 'ACTIVE'
    ORDER BY display_order, name
)";

    return sqlSession->executeQuery<WardDO>(
        sql,
        WardNameMapper(),
        params
    );
}



int WardDAO::insertLocation(
    const WardLocationDO& data)
{
    return insert(data);
}

std::string WardDAO::selectTenantIdByOrganizationId(
    const std::string& organizationId)
{
    const std::string sql = R"(
        SELECT tenant_id
        FROM adm_organization
        WHERE id = ?
          AND is_deleted = 0
        LIMIT 1
    )";

    SqlParams params;

    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        organizationId
    );

    return sqlSession->executeQueryOne<std::string>(
        sql,
        OrganizationTenantIdMapper(),
        params
    );
}
//--------------------------
PtrWardLocationDO WardDAO::selectLocationById(
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
            PtrWardLocationMapper(),
            params
        );
}
PtrWardLocationDO WardDAO::selectLocationById(
    const std::string& id,
    const std::string& organizationId)
{
    const std::string sql = R"(
        SELECT
            id,
            tenant_id,
            bus_no,
            form_enum,
            organization_id
        FROM adm_location
        WHERE id = ?
          AND organization_id = ?
          AND is_deleted = 0
        LIMIT 1
    )";

    SqlParams params;

    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        id
    );

    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        organizationId
    );

    return sqlSession
        ->executeQueryOne<PtrWardLocationDO>(
            sql,
            WardLocationMapper(),
            params
        );
}
std::string
WardDAO::selectOrganizationTenantIdForUpdate(
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

    return sqlSession->executeQueryOne<std::string>(
        sql,
        OrganizationTenantIdMapper(),
        params
    );
}
PtrWardLocationDO
WardDAO::selectLocationByIdForUpdate(
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
            PtrWardLocationMapper(),
            params
        );
}
bool WardDAO::existsSiblingName(
    const std::string& organizationId,
    const std::string& formEnum,
    const std::string& parentId,
    const std::string& name)
{
    std::string sql = R"(
        SELECT COUNT(*)
        FROM adm_location
        WHERE organization_id = ?
          AND form_enum = ?
          AND name = ?
          AND is_deleted = 0
    )";

    SqlParams params;

    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        organizationId
    );

    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        formEnum
    );

    SQLPARAMS_PUSH(
        params,
        "s",
        std::string,
        name
    );

    if (formEnum == "WARD")
    {
        sql += " AND parent_id IS NULL";
    }
    else
    {
        sql += " AND parent_id = ?";

        SQLPARAMS_PUSH(
            params,
            "s",
            std::string,
            parentId
        );
    }

    return sqlSession->executeQueryNumerical(
        sql,
        params
    ) > 0;
}