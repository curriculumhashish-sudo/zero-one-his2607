#include "stdafx.h"
#include "ReceptionPatientDAO.h"
#include <iostream>

PtrCurrentReceptionCombineDO ReceptionPatientDAO::queryReceptionPatient(
    const RecePatientQuery::Wrapper& query,
    const std::string& tenantId,
    const std::string& practitionerId)
{
    std::string sql = R"(
SELECT DISTINCT
    e.id AS enc_id,
    e.tenant_id,
    e.status_enum,
    e.reception_time,
    e.service_type_id,

    p.id AS patient_id,
    p.name AS patient_name,
    p.gender_enum,
    p.birth_date,
    p.phone,

    org.name AS dept_name,
    ai.description AS allergy_text,
    serv.name AS service_name,
    acc.type_code AS fee_type_code

FROM adm_encounter e
INNER JOIN adm_patient p
    ON e.patient_id = p.id
    AND p.tenant_id = e.tenant_id
    AND p.is_deleted = 0

LEFT JOIN adm_encounter_location enc_loc
    ON e.id = enc_loc.encounter_id
    AND enc_loc.tenant_id = e.tenant_id
    AND enc_loc.is_deleted = 0

LEFT JOIN adm_organization org
    ON enc_loc.location_id = org.id
    AND org.tenant_id = e.tenant_id
    AND org.is_deleted = 0

LEFT JOIN adm_encounter_participant part
    ON e.id = part.encounter_id
    AND part.tenant_id = e.tenant_id
    AND part.is_deleted = 0
    AND (? = '' OR part.practitioner_id = ?)

LEFT JOIN cln_allergy_intolerance ai
    ON ai.patient_id = p.id
    AND ai.tenant_id = e.tenant_id
    AND ai.is_deleted = 0

LEFT JOIN adm_account acc
    ON e.id = acc.encounter_id
    AND acc.tenant_id = e.tenant_id
    AND acc.default_flag = 1
    AND acc.is_deleted = 0

LEFT JOIN adm_healthcare_service serv
    ON e.service_type_id = serv.id
    AND serv.tenant_id = e.tenant_id
    AND serv.is_deleted = 0

WHERE e.is_deleted = 0
  AND e.tenant_id = ?
  AND DATE(e.reception_time) = ?
)";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, practitionerId);
    SQLPARAMS_PUSH(params, "s", std::string, practitionerId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, query->date->c_str());

    sql += " ORDER BY e.reception_time ASC; ";

    ReceptionPatientMapper mapper;
    auto result = sqlSession->executeQuery<PtrCurrentReceptionCombineDO>(sql, mapper, params);

    std::cout << "查询结果数量：" << result.size() << std::endl;
    return result.empty() ? nullptr : result.front();
}

PtrCurrentReceptionCombineDO ReceptionPatientDAO::queryReceptionByEncounterId(const std::string& encounterId, const std::string& tenantId)
{
    std::string sql = R"(
SELECT DISTINCT
    e.id AS enc_id,
    e.tenant_id,
    e.status_enum,
    e.reception_time,
    e.service_type_id,

    p.id AS patient_id,
    p.name AS patient_name,
    p.gender_enum,
    p.birth_date,
    p.phone,

    org.name AS dept_name,
    ai.description AS allergy_text,
    serv.name AS service_name,
    acc.type_code AS fee_type_code

FROM adm_encounter e
INNER JOIN adm_patient p
    ON e.patient_id = p.id
    AND p.tenant_id = e.tenant_id
    AND p.is_deleted = 0

LEFT JOIN adm_encounter_location enc_loc
    ON e.id = enc_loc.encounter_id
    AND enc_loc.tenant_id = e.tenant_id
    AND enc_loc.is_deleted = 0

LEFT JOIN adm_organization org
    ON enc_loc.location_id = org.id
    AND org.tenant_id = e.tenant_id
    AND org.is_deleted = 0

LEFT JOIN adm_encounter_participant part
    ON e.id = part.encounter_id
    AND part.tenant_id = e.tenant_id
    AND part.is_deleted = 0

LEFT JOIN cln_allergy_intolerance ai
    ON ai.patient_id = p.id
    AND ai.tenant_id = e.tenant_id
    AND ai.is_deleted = 0

LEFT JOIN adm_account acc
    ON e.id = acc.encounter_id
    AND acc.tenant_id = e.tenant_id
    AND acc.default_flag = 1
    AND acc.is_deleted = 0

LEFT JOIN adm_healthcare_service serv
    ON e.service_type_id = serv.id
    AND serv.tenant_id = e.tenant_id
    AND serv.is_deleted = 0

WHERE e.is_deleted = 0
  AND e.id = ?
  AND e.tenant_id = ?
)";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);

    ReceptionPatientMapper mapper;
    auto result = sqlSession->executeQuery<PtrCurrentReceptionCombineDO>(sql, mapper, params);
    return result.empty() ? nullptr : result.front();
}

bool ReceptionPatientDAO::updateEncounterStatusIfMatch(const std::string& encounterId, const std::string& tenantId,
                                                       const std::string& newStatus, const std::string& expectedCurrentStatus)
{
    std::string sql = R"(
UPDATE adm_encounter
SET status_enum = ?
WHERE id = ? AND tenant_id = ? AND status_enum = ?
)";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, newStatus);
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, expectedCurrentStatus);

    try {
        int affected = sqlSession->executeUpdate(sql, params);
        return affected > 0;
    } catch (...) {
        return false;
    }
}