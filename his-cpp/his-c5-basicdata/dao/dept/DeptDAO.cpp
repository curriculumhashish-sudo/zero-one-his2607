#include "stdafx.h"
#include "DeptDAO.h"

#include <sstream>
#include <utility>
#include <vector>

#include "DeptMapper.h"

namespace
{
    const char* DEPT_COLUMNS =
        "id, tenant_id, name, bus_no, parent_id, type_enum, class_enum, "
        "status_enum, display_order, def_doctor_id, py_str, "
        "wb_str, yb_no, yb_name, medins_id, medins_admdvs, medins_type, "
        "medins_lv, caty, version";

    const std::vector<std::pair<std::string, std::string>> BUSINESS_REFERENCES = {
        {"adm_charge_item", "org_id"},
        {"adm_charge_item_definition", "org_id"},
        {"adm_device_definition", "org_id"},
        {"adm_encounter", "organization_id"},
        {"adm_episode_of_care", "organization_id"},
        {"adm_location", "organization_id"},
        {"adm_patient", "organization_id"},
        {"adm_pr_assign", "org_id"},
        {"adm_practitioner", "org_id"},
        {"bkg_calendar_exception", "organization_id"},
        {"bkg_channel_config", "organization_id"},
        {"bkg_diagnostic_resource_map", "organization_id"},
        {"bkg_group_checkup_batch", "org_id"},
        {"bkg_health_check_package", "organization_id"},
        {"bkg_queue_display_config", "organization_id"},
        {"bkg_registration_policy", "organization_id"},
        {"bkg_release_policy", "organization_id"},
        {"bkg_resource_profile", "organization_id"},
        {"bkg_schedule_change_policy", "organization_id"},
        {"bkg_schedule_template", "organization_id"},
        {"bkg_slot_provision_template", "organization_id"},
        {"bkg_stop_clinic_slot_freeze", "organization_id"},
        {"bkg_waitlist_entry", "organization_id"},
        {"bkg_week_schedule_template", "organization_id"},
        {"cln_diagnostic_report", "org_id"},
        {"cln_diagnostic_report_index", "org_id"},
        {"cln_observation", "org_id"},
        {"cln_observation_definition", "org_id"},
        {"cln_procedure", "org_id"},
        {"cus_activity_observation_link", "org_id"},
        {"cus_encounter_history", "original_organization_id"},
        {"cus_encounter_history", "target_organization_id"},
        {"cus_price_adjustment", "org_id"},
        {"cus_service_location_template", "organization_id"},
        {"cus_supplier", "org_id"},
        {"emr_template", "organization_id"},
        {"fin_cashier_settlement", "org_id"},
        {"fin_contract", "org_id"},
        {"med_medication_definition", "org_id"},
        {"med_medication_request", "org_id"},
        {"peis_auto_express", "org_id"},
        {"peis_common_result", "org_id"},
        {"peis_config", "org_id"},
        {"peis_encounter_ext", "org_id"},
        {"peis_report_grant_ext", "org_id"},
        {"peis_service_request_ext", "org_id"},
        {"prt_print_binding", "organization_id"},
        {"prt_print_event_log", "organization_id"},
        {"prt_template", "organization_id"},
        {"whale_user_login_session", "hospital_organization_id"},
        {"whale_user_login_session", "organization_unit_id"},
        {"whale_user_organization_units", "organization_unit_id"},
        {"wkf_activity_definition", "org_id"},
        {"wkf_appointment_queue", "organization_id"},
        {"wkf_device_request", "org_id"},
        {"wkf_schedule", "organization_id"},
        {"wkf_service_request", "org_id"},
        {"wkf_slot", "organization_id"}
    };

    void appendAssignment(
        std::ostringstream& sql,
        bool& first,
        const std::string& column)
    {
        if (!first)
            sql << ", ";
        first = false;
        sql << column;
    }

    void appendStringPatch(
        const std::string& column,
        const oatpp::String& value,
        std::ostringstream& sql,
        SqlParams& params,
        bool& first)
    {
        if (!value)
            return;

        appendAssignment(sql, first, column + " = ?");
        SQLPARAMS_PUSH(params, "s", std::string, value->c_str());
    }
}

std::string DeptDAO::selectTenantIdByUserId(const std::string& userId)
{
    const std::string sql =
        "SELECT tenant_id FROM whale_user_login_session "
        "WHERE user_id = ? AND tenant_id IS NOT NULL "
        "ORDER BY updated_at DESC LIMIT 1";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, userId);
    return sqlSession->executeQueryOne<std::string>(
        sql, DeptStringMapper("tenant_id"), params);
}

PtrDeptDO DeptDAO::selectByIdForUpdate(const std::string& id)
{
    const std::string sql =
        std::string("SELECT ") + DEPT_COLUMNS +
        " FROM adm_organization WHERE id = ? AND is_deleted = 0 "
        "LIMIT 1 FOR UPDATE";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    return sqlSession->executeQueryOne<PtrDeptDO>(sql, DeptMapper(), params);
}

bool DeptDAO::existsName(
    const std::string& tenantId,
    const std::string& name,
    const std::string& excludeId)
{
    std::string sql =
        "SELECT COUNT(1) FROM adm_organization "
        "WHERE tenant_id = ? AND name = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, name);
    if (!excludeId.empty())
    {
        sql += " AND id <> ?";
        SQLPARAMS_PUSH(params, "s", std::string, excludeId);
    }
    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

bool DeptDAO::existsDoctor(
    const std::string& tenantId,
    const std::string& doctorId)
{
    const std::string sql =
        "SELECT COUNT(1) FROM adm_practitioner "
        "WHERE id = ? AND tenant_id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, doctorId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

bool DeptDAO::hasChildren(const std::string& id)
{
    const std::string sql =
        "SELECT COUNT(1) FROM adm_organization "
        "WHERE parent_id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

bool DeptDAO::hasBusinessReferences(const std::string& id)
{
    for (const auto& reference : BUSINESS_REFERENCES)
    {
        const std::string sql =
            "SELECT COUNT(1) FROM " + reference.first +
            " WHERE " + reference.second + " = ?";
        SqlParams params;
        SQLPARAMS_PUSH(params, "s", std::string, id);
        if (sqlSession->executeQueryNumerical(sql, params) > 0)
            return true;
    }
    return false;
}

int DeptDAO::insertDept(const DeptDO& data)
{
    const std::string sql =
        "INSERT INTO adm_organization ("
        "id, tenant_id, created_at, created_by, is_deleted, version, name, "
        "bus_no, parent_id, type_enum, class_enum, status_enum, "
        "display_order, def_doctor_id, py_str, wb_str, yb_no, yb_name, "
        "medins_id, medins_admdvs, medins_type, medins_lv, caty) "
        "VALUES (?, ?, ?, ?, 0, 0, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, data.getId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getTenantId());
    SQLPARAMS_PUSH(params, "dt", std::string, data.getCreatedAt());
    SQLPARAMS_PUSH(params, "s", std::string, data.getCreatedBy());
    SQLPARAMS_PUSH(params, "s", std::string, data.getName());
    SQLPARAMS_PUSH(params, "s", std::string, data.getBusNo());

    if (data.getParentId().empty()) SQLPARAMS_PUSH(params, "nul", int, 1);
    else SQLPARAMS_PUSH(params, "s", std::string, data.getParentId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getTypeEnum());
    if (data.getClassEnum().empty()) SQLPARAMS_PUSH(params, "nul", int, 1);
    else SQLPARAMS_PUSH(params, "s", std::string, data.getClassEnum());
    SQLPARAMS_PUSH(params, "s", std::string, data.getStatusEnum());
    SQLPARAMS_PUSH(params, "i", int, data.getDisplayOrder());

    const std::vector<std::string> nullableValues = {
        data.getDefDoctorId(), data.getPyStr(), data.getWbStr(), data.getYbNo(),
        data.getYbName(), data.getMedinsId(), data.getMedinsAdmdvs(),
        data.getMedinsType(), data.getMedinsLv(), data.getCaty()
    };
    for (const auto& value : nullableValues)
    {
        if (value.empty()) SQLPARAMS_PUSH(params, "nul", int, 1);
        else SQLPARAMS_PUSH(params, "s", std::string, value);
    }
    return sqlSession->executeUpdate(sql, params);
}

int DeptDAO::updatePatch(
    const DeptModifyDTO& dto,
    const std::string& userId,
    bool clearOrganizationFields)
{
    std::ostringstream sql;
    sql << "UPDATE adm_organization SET ";
    SqlParams params;
    bool first = true;

    appendStringPatch("name", dto.name, sql, params, first);
    appendStringPatch("type_enum", dto.typeEnum, sql, params, first);
    if (clearOrganizationFields)
    {
        appendAssignment(sql, first, "class_enum = NULL");
        appendAssignment(sql, first, "parent_id = NULL");
    }
    else
    {
        appendStringPatch("class_enum", dto.classEnum, sql, params, first);
        appendStringPatch("parent_id", dto.parentId, sql, params, first);
    }
    appendStringPatch("yb_no", dto.ybNo, sql, params, first);
    appendStringPatch("yb_name", dto.ybName, sql, params, first);
    appendStringPatch("medins_id", dto.medinsId, sql, params, first);
    appendStringPatch("medins_admdvs", dto.medinsAdmdvs, sql, params, first);
    appendStringPatch("medins_type", dto.medinsType, sql, params, first);
    appendStringPatch("medins_lv", dto.medinsLv, sql, params, first);
    appendStringPatch("py_str", dto.pyStr, sql, params, first);
    appendStringPatch("wb_str", dto.wbStr, sql, params, first);
    appendStringPatch("def_doctor_id", dto.defDoctorId, sql, params, first);
    appendStringPatch("caty", dto.caty, sql, params, first);

    if (dto.displayOrder)
    {
        appendAssignment(sql, first, "display_order = ?");
        SQLPARAMS_PUSH(params, "i", int, dto.displayOrder.getValue(0));
    }
    if (dto.statusEnum)
    {
        appendAssignment(sql, first, "status_enum = ?");
        SQLPARAMS_PUSH(params, "s", std::string, dto.statusEnum.getValue(""));
    }

    appendAssignment(sql, first, "updated_at = NOW(6)");
    appendAssignment(sql, first, "updated_by = ?");
    SQLPARAMS_PUSH(params, "s", std::string, userId);
    appendAssignment(sql, first, "version = version + 1");
    sql << " WHERE id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "s", std::string, dto.id.getValue(""));
    return sqlSession->executeUpdate(sql.str(), params);
}

int DeptDAO::logicalDelete(
    const std::string& id,
    const std::string& tenantId,
    const std::string& userId)
{
    const std::string sql =
        "UPDATE adm_organization SET is_deleted = 1, deleted_at = NOW(6), "
        "deleted_by = ?, updated_at = NOW(6), updated_by = ?, version = version + 1 "
        "WHERE id = ? AND tenant_id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, userId);
    SQLPARAMS_PUSH(params, "s", std::string, userId);
    SQLPARAMS_PUSH(params, "s", std::string, id);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeUpdate(sql, params);
}

int DeptDAO::updateBatchStatus(
    const std::vector<std::string>& ids,
    const std::string& tenantId,
    const std::string& userId,
    const std::string& statusEnum)
{
    if (ids.empty())
        return 0;

    std::ostringstream sql;
    sql << "UPDATE adm_organization SET status_enum = ?, "
        "updated_at = NOW(6), updated_by = ?, version = version + 1 "
        "WHERE tenant_id = ? AND is_deleted = 0 AND id IN (";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
    SQLPARAMS_PUSH(params, "s", std::string, userId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    for (size_t index = 0; index < ids.size(); ++index)
    {
        if (index > 0) sql << ",";
        sql << "?";
        SQLPARAMS_PUSH(params, "s", std::string, ids[index]);
    }
    sql << ")";
    return sqlSession->executeUpdate(sql.str(), params);
}
