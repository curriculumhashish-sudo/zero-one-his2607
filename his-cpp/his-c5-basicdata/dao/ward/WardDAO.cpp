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

// ============================================================
// Ward/Bed mappers and query methods (c5-ward-pb)
// ============================================================

// Ward mapper
class WardMapper : public Mapper<WardDO>
{
public:
    WardDO mapper(ResultSet* rs) const override
    {
        WardDO d;
        d.setId(rs->getString(1));
        d.setName(rs->getString(2));
        d.setOrganizationId(rs->getString(3));
        d.setTotalBeds(rs->getInt(4));
        d.setAvailableBeds(rs->getInt(5));
        d.setOccupiedBeds(rs->getInt(6));
        d.setReservedBeds(rs->getInt(7));
        d.setFloor(rs->getString(8));
        d.setWardCategory(rs->getString(9));
        d.setDefaultNurseStationId(rs->getString(10));
        d.setNursingLevel(rs->getString(11));
        d.setPyStr(rs->getString(12));
        d.setWbStr(rs->getString(13));
        d.setDisplayOrder(rs->getInt(14));
        d.setVersion(rs->getInt(15));
        d.setCreatedAt(rs->getString(16));
        d.setUpdatedAt(rs->getString(17));
        return d;
    }
};

class PtrWardMapper : public Mapper<PtrWardDO>
{
public:
    PtrWardDO mapper(ResultSet* rs) const override
    {
        auto p = std::make_shared<WardDO>();
        p->setId(rs->getString(1));
        p->setName(rs->getString(2));
        p->setOrganizationId(rs->getString(3));
        p->setTotalBeds(rs->getInt(4));
        p->setAvailableBeds(rs->getInt(5));
        p->setOccupiedBeds(rs->getInt(6));
        p->setReservedBeds(rs->getInt(7));
        p->setFloor(rs->getString(8));
        p->setWardCategory(rs->getString(9));
        p->setDefaultNurseStationId(rs->getString(10));
        p->setNursingLevel(rs->getString(11));
        p->setPyStr(rs->getString(12));
        p->setWbStr(rs->getString(13));
        p->setDisplayOrder(rs->getInt(14));
        p->setVersion(rs->getInt(15));
        p->setCreatedAt(rs->getString(16));
        p->setUpdatedAt(rs->getString(17));
        return p;
    }
};

// Bed mapper
class BedMapper : public Mapper<BedDO>
{
public:
    BedDO mapper(ResultSet* rs) const override
    {
        BedDO d;
        d.setId(rs->getString(1));
        d.setName(rs->getString(2));
        d.setParentId(rs->getString(3));
        d.setOrganizationId(rs->getString(4));
        d.setStatusEnum(rs->getString(5));
        d.setTypeJson(rs->getString(6));
        d.setDisplayOrder(rs->getString(7));
        d.setVersion(rs->getString(8));
        d.setCreatedAt(rs->getString(9));
        d.setUpdatedAt(rs->getString(10));
        return d;
    }
};

class PtrBedMapper : public Mapper<PtrBedDO>
{
public:
    PtrBedDO mapper(ResultSet* rs) const override
    {
        auto p = std::make_shared<BedDO>();
        p->setId(rs->getString(1));
        p->setName(rs->getString(2));
        p->setParentId(rs->getString(3));
        p->setOrganizationId(rs->getString(4));
        p->setStatusEnum(rs->getString(5));
        p->setTypeJson(rs->getString(6));
        p->setDisplayOrder(rs->getString(7));
        p->setVersion(rs->getString(8));
        p->setCreatedAt(rs->getString(9));
        p->setUpdatedAt(rs->getString(10));
        return p;
    }
};

PtrWardDO WardDAO::selectRoomById(const std::string& id)
{
    std::string sql = "SELECT id, name, organization_id, total_beds, available_beds, occupied_beds, reserved_beds, floor, ward_category, default_nurse_station_id, nursing_level, py_str, wb_str, display_order, version, created_at, updated_at FROM adm_location WHERE id=?";
    auto ward = sqlSession->executeQueryOne<PtrWardDO>(sql, PtrWardMapper(), "%s", id);
    if (!ward) return nullptr;

    // Load bed list
    ward->beds.clear();
    std::list<PtrBedDO> beds = selectBedsByParentId(id);
    for (auto& b : beds) ward->beds.push_back(b);
    return ward;
}

PtrBedDO WardDAO::selectBedById(const std::string& id)
{
    std::string sql = "SELECT id, name, parent_id, organization_id, status_enum, type_json, display_order, version, created_at, updated_at FROM adm_location WHERE id=?";
    return sqlSession->executeQueryOne<PtrBedDO>(sql, PtrBedMapper(), "%s", id);
}

std::list<PtrBedDO> WardDAO::selectBedsByParentId(const std::string& parentId)
{
    std::string sql = "SELECT id, name, parent_id, organization_id, status_enum, type_json, display_order, version, created_at, updated_at FROM adm_location WHERE parent_id=? ORDER BY display_order ASC, id ASC";
    return sqlSession->executeQuery<PtrBedDO>(sql, PtrBedMapper(), "%s", parentId);
}
