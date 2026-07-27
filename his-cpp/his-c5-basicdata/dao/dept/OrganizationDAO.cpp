#include "stdafx.h"
#include "OrganizationDAO.h"
#include "OrganizationMapper.h"

std::list<PtrAdmOrganizationDO> OrganizationDAO::selectAllNotDeleted()
{
    std::string sql =
        "SELECT "
        "id, name, bus_no, parent_id, type_enum, class_enum, status_enum, "
        "def_doctor_id, caty, "
        "yb_no, yb_name, medins_id, medins_admdvs, medins_type, medins_lv, "
        "py_str, wb_str, display_order "
        "FROM adm_organization "
        "WHERE is_deleted = 0 "
        "ORDER BY display_order ASC, id ASC";

    return sqlSession->executeQuery<PtrAdmOrganizationDO>(
        sql,
        OrganizationMapper()
    );
}

PtrAdmOrganizationDO OrganizationDAO::selectById(const std::string& id)
{
    std::string sql =
        "SELECT "
        "id, name, bus_no, parent_id, type_enum, class_enum, status_enum, "
        "def_doctor_id, caty, "
        "yb_no, yb_name, medins_id, medins_admdvs, medins_type, medins_lv, "
        "py_str, wb_str, display_order "
        "FROM adm_organization "
        "WHERE id = ? "
        "AND is_deleted = 0";

    return sqlSession->executeQueryOne<PtrAdmOrganizationDO>(
        sql,
        OrganizationMapper(),
        "%s",
        id
    );
}
