#pragma once
#ifndef _ADM_ORGANIZATIONDO_H_
#define _ADM_ORGANIZATIONDO_H_
#include"../DoInclude.h"


class AdmOrganizationDO : public BaseDO
{
    // Fields required by tree, list, and detail responses.
    MYSQL_SYNTHESIZE(std::string, id, Id);
    MYSQL_SYNTHESIZE(std::string, name, Name);
    MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
    MYSQL_SYNTHESIZE(std::string, parentId, ParentId);
    MYSQL_SYNTHESIZE(std::string, typeEnum, TypeEnum);
    MYSQL_SYNTHESIZE(std::string, classEnum, ClassEnum);
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(std::string, defDoctorId, DefDoctorId);
    MYSQL_SYNTHESIZE(std::string, caty, Caty);
    MYSQL_SYNTHESIZE(std::string, ybNo, YbNo);
    MYSQL_SYNTHESIZE(std::string, ybName, YbName);
    MYSQL_SYNTHESIZE(std::string, medinsId, MedinsId);
    MYSQL_SYNTHESIZE(std::string, medinsAdmdvs, MedinsAdmdvs);
    MYSQL_SYNTHESIZE(std::string, medinsType, MedinsType);
    MYSQL_SYNTHESIZE(std::string, medinsLv, MedinsLv);
    MYSQL_SYNTHESIZE(std::string, pyStr, PyStr);
    MYSQL_SYNTHESIZE(std::string, wbStr, WbStr);
    MYSQL_SYNTHESIZE(int, displayOrder, DisplayOrder);

public:
    AdmOrganizationDO() : BaseDO("adm_organization")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD_NULLABLE("parent_id", "s", parentId, true);
        MYSQL_ADD_FIELD("type_enum", "s", typeEnum);
        MYSQL_ADD_FIELD_NULLABLE("class_enum", "s", classEnum, true);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD_NULLABLE("def_doctor_id", "s", defDoctorId, true);
        MYSQL_ADD_FIELD_NULLABLE("caty", "s", caty, true);
        MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", ybNo, true);
        MYSQL_ADD_FIELD_NULLABLE("yb_name", "s", ybName, true);
        MYSQL_ADD_FIELD_NULLABLE("medins_id", "s", medinsId, true);
        MYSQL_ADD_FIELD_NULLABLE("medins_admdvs", "s", medinsAdmdvs, true);
        MYSQL_ADD_FIELD_NULLABLE("medins_type", "s", medinsType, true);
        MYSQL_ADD_FIELD_NULLABLE("medins_lv", "s", medinsLv, true);
        MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
        MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
        MYSQL_ADD_FIELD_NULLABLE("display_order", "i", displayOrder, true);
    }
};

typedef std::shared_ptr<AdmOrganizationDO> PtrAdmOrganizationDO;


#endif /* _ADM_ORGANIZATIONDO_H_ */
