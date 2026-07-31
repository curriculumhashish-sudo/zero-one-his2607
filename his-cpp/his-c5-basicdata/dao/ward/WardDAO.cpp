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

    // 加载床位列表
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