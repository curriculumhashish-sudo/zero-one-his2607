#pragma once
#ifndef WARD_TREE_DO_H
#define WARD_TREE_DO_H

#include "../DoInclude.h"

class WardTreeDO : public BaseDO
{
    MYSQL_SYNTHESIZE(std::string, id, Id);
    MYSQL_SYNTHESIZE(std::string, name, Name);
    MYSQL_SYNTHESIZE(std::string, formEnum, FormEnum);
    MYSQL_SYNTHESIZE(std::string, parentId, ParentId);
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(
        std::string,
        operationalEnum,
        OperationalEnum
    );
    MYSQL_SYNTHESIZE(int, displayOrder, DisplayOrder);

   
    MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
    MYSQL_SYNTHESIZE(std::string, organizationId, OrganizationId);
    MYSQL_SYNTHESIZE(std::string, pyStr, PyStr);
    MYSQL_SYNTHESIZE(std::string, wbStr, WbStr);
    MYSQL_SYNTHESIZE(std::string, typeJson, TypeJson);
    MYSQL_SYNTHESIZE(int, version, Version);

public:
    WardTreeDO()
        : BaseDO("adm_location")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("form_enum", "s", formEnum);
        MYSQL_ADD_FIELD("parent_id", "s", parentId);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD(
            "operational_enum",
            "s",
            operationalEnum
        );
        MYSQL_ADD_FIELD(
            "display_order",
            "i",
            displayOrder
        );

       
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("organization_id", "s", organizationId);
        MYSQL_ADD_FIELD("py_str", "s", pyStr);
        MYSQL_ADD_FIELD("wb_str", "s", wbStr);
        MYSQL_ADD_FIELD("type_json", "s", typeJson);
        MYSQL_ADD_FIELD("version", "i", version);
    }
};

typedef std::shared_ptr<WardTreeDO> PtrWardTreeDO;

#endif