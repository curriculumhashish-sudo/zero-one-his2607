#pragma once
#ifndef _ADM_ORGANIZATIONDO_H_
#define _ADM_ORGANIZATIONDO_H_
#include"../DoInclude.h"


class AdmOrganizationDO : public BaseDO
{
    // Fields required by tree, list, and detail responses.
    MYSQL_SYNTHESIZE(std::string, id, Id);
    MYSQL_SYNTHESIZE(std::string, name, Name);
    MYSQL_SYNTHESIZE(std::string, parent_id, Parent_Id);
    MYSQL_SYNTHESIZE(std::string, type_enum, Type_Enum);
    MYSQL_SYNTHESIZE(std::string, class_enum, Class_Enum);
    MYSQL_SYNTHESIZE(std::string, status_enum, Status_Enum);
    MYSQL_SYNTHESIZE(std::string, yb_no, Yb_No);
    MYSQL_SYNTHESIZE(std::string, yb_name, Yb_Name);
    MYSQL_SYNTHESIZE(std::string, medins_id, Medins_Id);
    MYSQL_SYNTHESIZE(std::string, medins_admdvs, Medins_Admdvs);
    MYSQL_SYNTHESIZE(std::string, medins_type, Medins_Type);
    MYSQL_SYNTHESIZE(std::string, medins_lv, Medins_Lv);
    MYSQL_SYNTHESIZE(std::string, py_str, Py_Str);
    MYSQL_SYNTHESIZE(std::string, wb_str, Wb_Str);
    MYSQL_SYNTHESIZE(int, display_order, Display_Order);

public:
    AdmOrganizationDO() : BaseDO("adm_organization")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD_NULLABLE("parent_id", "s", parent_id, true);
        MYSQL_ADD_FIELD("type_enum", "s", type_enum);
        MYSQL_ADD_FIELD_NULLABLE("class_enum", "s", class_enum, true);
        MYSQL_ADD_FIELD("status_enum", "s", status_enum);
        MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", yb_no, true);
        MYSQL_ADD_FIELD_NULLABLE("yb_name", "s", yb_name, true);
        MYSQL_ADD_FIELD_NULLABLE("medins_id", "s", medins_id, true);
        MYSQL_ADD_FIELD_NULLABLE("medins_admdvs", "s", medins_admdvs, true);
        MYSQL_ADD_FIELD_NULLABLE("medins_type", "s", medins_type, true);
        MYSQL_ADD_FIELD_NULLABLE("medins_lv", "s", medins_lv, true);
        MYSQL_ADD_FIELD_NULLABLE("py_str", "s", py_str, true);
        MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wb_str, true);
        MYSQL_ADD_FIELD_NULLABLE("display_order", "i", display_order, true);
    }
};

typedef std::shared_ptr<AdmOrganizationDO> PtrAdmOrganizationDO;


#endif /* _ADM_ORGANIZATIONDO_H_ */