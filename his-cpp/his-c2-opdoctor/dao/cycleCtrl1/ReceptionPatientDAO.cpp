#include "stdafx.h"
#include "ReceptionPatientDAO.h"
#include <iostream>

PtrCurrentReceptionCombineDO ReceptionPatientDAO::queryReceptionPatient(
    const RecePatientQuery::Wrapper& query,
    const std::string& tenantId,
    const std::string& practitionerId)
{
    std::string sql = R"(
        SELECT
            e.id AS enc_id,
            e.tenant_id,
            e.bus_no,
            e.status_enum,
            UNIX_TIMESTAMP(DATE(e.reception_time)) AS reception_time,
            e.service_type_id,

            p.id AS patient_id,
            p.name AS patient_name,
            p.gender_enum,
            p.birth_date,
            p.phone AS phone_num,

            hs.name AS service_name,
            org.name AS org_name,
            a.type_code AS pay_type,
            IFNULL(GROUP_CONCAT(ai.description SEPARATOR ';'), 'none') AS allergy_content

        FROM adm_encounter e
        INNER JOIN adm_patient p ON e.patient_id = p.id AND p.is_deleted = 0
        INNER JOIN adm_encounter_location enc_loc ON e.id = enc_loc.encounter_id AND enc_loc.is_deleted = 0
        INNER JOIN adm_organization org ON enc_loc.location_id = org.id AND org.is_deleted = 0
        INNER JOIN adm_encounter_participant part ON e.id = part.encounter_id AND part.is_deleted = 0
        LEFT JOIN adm_account a ON e.id = a.encounter_id AND a.default_flag = 1 AND a.is_deleted = 0
        LEFT JOIN adm_healthcare_service hs ON e.service_type_id = hs.id AND hs.is_deleted = 0
        LEFT JOIN cln_allergy_intolerance ai ON ai.patient_id = p.id AND ai.is_deleted = 0

        WHERE e.is_deleted = 0
          AND e.tenant_id = ?
          AND part.practitioner_id = ?
          AND DATE(e.reception_time) = ?
          AND e.status_enum = ?
        GROUP BY 
            e.id, e.tenant_id, e.bus_no, e.status_enum, e.reception_time, e.service_type_id,
            p.id, p.name, p.gender_enum, p.birth_date, p.phone,
            hs.name, org.name, a.type_code
        ORDER BY e.reception_time ASC
        LIMIT 1;
    )";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, practitionerId);
    SQLPARAMS_PUSH(params, "s", std::string, query->visitTime->c_str());
    
    SQLPARAMS_PUSH(params, "s", std::string, std::string("TREATING"));

    ReceptionPatientMapper mapper;

    // 打印 SQL 与参数（排查用）
    std::cout << "[DAO] Executing SQL:\n" << sql << std::endl;
    std::cout << "[DAO] Params: tenantId=" << tenantId << " practitionerId=" << practitionerId
        << " visitTime=" << (query->visitTime ? query->visitTime->c_str() : "") << " status=TREATING" << std::endl;

    try {
        auto result = sqlSession->executeQuery<PtrCurrentReceptionCombineDO>(sql, mapper, params);
        std::cout << "[DAO] result size = " << result.size() << std::endl;
        return result.empty() ? nullptr : result.front();
    }
    catch (const std::exception& ex) {
        std::cout << "[DAO] ExecuteQuery Exception: " << ex.what() << std::endl;
        return nullptr;
    }
}