#include "stdafx.h"
#include "WardDAO.h"

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
// ============================================================
// 只读按ID查询地理位置记录
// SQL: SELECT * FROM adm_location WHERE id=? AND is_deleted=0 LIMIT 1
// 不加FOR UPDATE锁，仅用于查询展示场景（如病房/病床ID查询接口）
// 写操作场景（修改/删除/状态变更）请使用selectLocationByIdForUpdate加锁查询
// ============================================================
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
            type_json,
            py_str,
            wb_str,
            version,
            parent_id
        FROM adm_location
        WHERE id = ?
          AND is_deleted = 0
        LIMIT 1
    )";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);

    return sqlSession
        ->executeQueryOne<PtrWardLocationDO>(
            sql,
            WardLocationMapper(),
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
// Lock and return the complete parent location record.
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
            type_json,
            py_str,
            wb_str,
            version,
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
    const std::string& name,
    const std::string& excludeId)
{
    SqlParams params;

    std::string sql = R"(
        SELECT EXISTS (
            SELECT 1
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

    if (!excludeId.empty())
    {
        sql += " AND id <> ?";
        SQLPARAMS_PUSH(
            params, "s", std::string, excludeId);
    }

    sql += " LIMIT 1)";

    return sqlSession->executeQueryNumerical(
        sql,
        params
    ) > 0;
}

bool WardDAO::hasChildren(
    const std::string& parentId,
    bool activeOnly)
{
    std::string sql = R"(
        SELECT EXISTS (
            SELECT 1
            FROM adm_location
            WHERE parent_id = ?
              AND is_deleted = 0
    )";

    if (activeOnly)
    {
        sql += " AND status_enum = 'ACTIVE'";
    }

    sql += " LIMIT 1)";

    SqlParams params;
    SQLPARAMS_PUSH(
        params, "s", std::string, parentId);

    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

int WardDAO::updateLocation(
    const WardLocationDO& data,
    int expectedVersion)
{
    std::string sql;
    SqlParams params;

    if (data.getFormEnum() == "WARD")
    {
        sql = R"(
            UPDATE adm_location
            SET name = ?,
                organization_id = ?,
                display_order = ?,
                py_str = ?,
                wb_str = ?,
                updated_at = ?,
                updated_by = ?,
                version = version + 1
        )";

        SQLPARAMS_PUSH(params, "s", std::string, data.getName());
        SQLPARAMS_PUSH(params, "s", std::string, data.getOrganizationId());
        SQLPARAMS_PUSH(params, "i", int, data.getDisplayOrder());
        SQLPARAMS_PUSH(params, "s", std::string, data.getPyStr());
        SQLPARAMS_PUSH(params, "s", std::string, data.getWbStr());
    }
    else if (data.getFormEnum() == "HOUSE")
    {
        sql = R"(
            UPDATE adm_location
            SET name = ?,
                parent_id = ?,
                organization_id = ?,
                display_order = ?,
                py_str = ?,
                wb_str = ?,
                updated_at = ?,
                updated_by = ?,
                version = version + 1
        )";

        SQLPARAMS_PUSH(params, "s", std::string, data.getName());
        SQLPARAMS_PUSH(params, "s", std::string, data.getParentId());
        SQLPARAMS_PUSH(params, "s", std::string, data.getOrganizationId());
        SQLPARAMS_PUSH(params, "i", int, data.getDisplayOrder());
        SQLPARAMS_PUSH(params, "s", std::string, data.getPyStr());
        SQLPARAMS_PUSH(params, "s", std::string, data.getWbStr());
    }
    else
    {
        sql = R"(
            UPDATE adm_location
            SET name = ?,
                parent_id = ?,
                organization_id = ?,
                display_order = ?,
                type_json = ?,
                updated_at = ?,
                updated_by = ?,
                version = version + 1
        )";

        SQLPARAMS_PUSH(params, "s", std::string, data.getName());
        SQLPARAMS_PUSH(params, "s", std::string, data.getParentId());
        SQLPARAMS_PUSH(params, "s", std::string, data.getOrganizationId());
        SQLPARAMS_PUSH(params, "i", int, data.getDisplayOrder());
        SQLPARAMS_PUSH(params, "s", std::string, data.getTypeJson());
    }

    SQLPARAMS_PUSH(params, "dt", std::string, data.getUpdatedAt());
    SQLPARAMS_PUSH(params, "s", std::string, data.getUpdatedBy());

    sql += R"(
        WHERE id = ?
          AND form_enum = ?
          AND version = ?
          AND is_deleted = 0
    )";

    SQLPARAMS_PUSH(params, "s", std::string, data.getId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getFormEnum());
    SQLPARAMS_PUSH(params, "i", int, expectedVersion);

    return sqlSession->executeUpdate(sql, params);
}

int WardDAO::softDeleteLocation(
    const std::string& id,
    const std::string& formEnum,
    int expectedVersion,
    const std::string& deletedAt,
    const std::string& deletedBy)
{
    const std::string sql = R"(
        UPDATE adm_location
        SET is_deleted = 1,
            deleted_at = ?,
            deleted_by = ?,
            updated_at = ?,
            updated_by = ?,
            version = version + 1
        WHERE id = ?
          AND form_enum = ?
          AND version = ?
          AND is_deleted = 0
    )";

    SqlParams params;
    SQLPARAMS_PUSH(params, "dt", std::string, deletedAt);
    SQLPARAMS_PUSH(params, "s", std::string, deletedBy);
    SQLPARAMS_PUSH(params, "dt", std::string, deletedAt);
    SQLPARAMS_PUSH(params, "s", std::string, deletedBy);
    SQLPARAMS_PUSH(params, "s", std::string, id);
    SQLPARAMS_PUSH(params, "s", std::string, formEnum);
    SQLPARAMS_PUSH(params, "i", int, expectedVersion);

    return sqlSession->executeUpdate(sql, params);
}

int WardDAO::updateLocationStatus(
    const std::string& id,
    const std::string& formEnum,
    const std::string& statusEnum,
    int expectedVersion,
    const std::string& updatedAt,
    const std::string& updatedBy)
{
    const std::string sql = R"(
        UPDATE adm_location
        SET status_enum = ?,
            updated_at = ?,
            updated_by = ?,
            version = version + 1
        WHERE id = ?
          AND form_enum = ?
          AND version = ?
          AND is_deleted = 0
    )";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
    SQLPARAMS_PUSH(params, "dt", std::string, updatedAt);
    SQLPARAMS_PUSH(params, "s", std::string, updatedBy);
    SQLPARAMS_PUSH(params, "s", std::string, id);
    SQLPARAMS_PUSH(params, "s", std::string, formEnum);
    SQLPARAMS_PUSH(params, "i", int, expectedVersion);

    return sqlSession->executeUpdate(sql, params);
}

int WardDAO::insertLocation(
    const WardLocationDO& data)
{
    return insert(data);
}
