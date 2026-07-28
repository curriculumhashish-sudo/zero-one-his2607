#include "stdafx.h"
#include "patientFinishDAO.h"
#include "patientFinishMapper.h"

std::shared_ptr<AdmEncounterDO> PatientFinishDAO::selectEncounterById(const oatpp::String& tenantId, const oatpp::String& encounterId)
{
    // 使用 amb_encounter_id 作为外部传入的 encounterId 匹配列
    std::string sql = "SELECT id, status_enum, is_deleted FROM adm_encounter WHERE amb_encounter_id = ? AND tenant_id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId->c_str());
    SQLPARAMS_PUSH(params, "s", std::string, tenantId->c_str());
    return sqlSession->executeQueryOne<std::shared_ptr<AdmEncounterDO>>(sql, PatientFinishMapper(), params);
}

bool PatientFinishDAO::updateEncounterToFinish(const oatpp::String& tenantId, const oatpp::String& encounterId)
{
    // 同样按 amb_encounter_id 更新
    std::string sql = "UPDATE adm_encounter SET status_enum = 'FINISHED', finish_time = NOW() WHERE amb_encounter_id = ? AND tenant_id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId->c_str());
    SQLPARAMS_PUSH(params, "s", std::string, tenantId->c_str());
    uint64_t affectRows = sqlSession->executeUpdate(sql, params);
    return affectRows > 0;
}