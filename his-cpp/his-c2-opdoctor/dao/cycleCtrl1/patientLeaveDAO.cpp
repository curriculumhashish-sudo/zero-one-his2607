#include "stdafx.h"
#include "patientLeaveDAO.h"

std::shared_ptr<AdmEncounterDO> PatientLeaveDAO::selectEncounterById(const oatpp::String& tenantId, const oatpp::String& encounterId)
{
    std::string sql = "SELECT id, tenant_id, status_enum, is_deleted "
                      "FROM adm_encounter "
                      "WHERE amb_encounter_id = ? AND tenant_id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId->c_str());
    SQLPARAMS_PUSH(params, "s", std::string, tenantId ? tenantId->c_str() : "");
    return getSqlSession()->executeQueryOne<std::shared_ptr<AdmEncounterDO>>(sql, PatientLeaveMapper(), params);
}

bool PatientLeaveDAO::updateEncounterToLeave(const oatpp::String& tenantId, const oatpp::String& encounterId)
{
    // 暂离仅修改就诊状态，按 amb_encounter_id 匹配（与 select 保持一致）
    std::string sql = "UPDATE adm_encounter SET status_enum = 'LEFT' WHERE amb_encounter_id = ? AND tenant_id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId->c_str());
    SQLPARAMS_PUSH(params, "s", std::string, tenantId ? tenantId->c_str() : "");
    return getSqlSession()->executeUpdate(sql, params);
}