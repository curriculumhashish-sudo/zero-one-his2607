#pragma once
#ifndef _WARD_LOCATION_DO_H_
#define _WARD_LOCATION_DO_H_

#include "../DoInclude.h"

class WardLocationDO : public BaseDO
{
    MYSQL_SYNTHESIZE(std::string, id, Id);
    MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
    MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
    MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);

    MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
    MYSQL_SYNTHESIZE(int, version, Version);

    MYSQL_SYNTHESIZE(std::string, name, Name);
    MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(std::string, operationalEnum, OperationalEnum);
    MYSQL_SYNTHESIZE(std::string, formEnum, FormEnum);
    MYSQL_SYNTHESIZE(std::string, modeEnum, ModeEnum);
    MYSQL_SYNTHESIZE(std::string, typeJson, TypeJson);
    MYSQL_SYNTHESIZE(std::string, organizationId, OrganizationId);
    MYSQL_SYNTHESIZE(int, displayOrder, DisplayOrder);
    MYSQL_SYNTHESIZE(std::string, parentId, ParentId);

public:
    WardLocationDO() : BaseDO("adm_location")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);

        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD("created_at", "dt", createdAt);
        MYSQL_ADD_FIELD("created_by", "s", createdBy);
        MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
        MYSQL_ADD_FIELD("version", "i", version);

        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD(
            "operational_enum",
            "s",
            operationalEnum
        );
        MYSQL_ADD_FIELD("form_enum", "s", formEnum);
        MYSQL_ADD_FIELD("mode_enum", "s", modeEnum);
        MYSQL_ADD_FIELD("type_json", "s", typeJson);
        MYSQL_ADD_FIELD(
            "organization_id",
            "s",
            organizationId
        );
        MYSQL_ADD_FIELD(
            "display_order",
            "i",
            displayOrder
        );

        // 不设置时，插入 SQL 将不包含 parent_id，
        // 因而 WARD 可以保存为 NULL。
        MYSQL_ADD_FIELD_NULLABLE(
            "parent_id",
            "s",
            parentId,
            true
        );
    }
};

typedef std::shared_ptr<WardLocationDO>
PtrWardLocationDO;

#endif