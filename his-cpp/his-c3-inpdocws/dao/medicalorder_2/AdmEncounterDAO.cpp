#include "stdafx.h"
#include "AdmEncounterDAO.h"
#include "AdmEncounterMapper.h"
#include "domain/do/medicalorder_2/AdmEncounterDO.h"


//selectById �� �����������
PtrAdmEncounterDO AdmEncounterDAO::selectById(const std::string& encounterId)
{
    if (encounterId.empty()) return nullptr;

    std::string sql =
        "SELECT id, patient_id, status_enum, org_id "
        "FROM adm_encounter "
        "WHERE id = ? AND is_deleted = 0 "
        "LIMIT 1";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);

    AdmEncounterMapper mapper;
    return sqlSession->executeQueryOne<PtrAdmEncounterDO>(sql, mapper, params);
}