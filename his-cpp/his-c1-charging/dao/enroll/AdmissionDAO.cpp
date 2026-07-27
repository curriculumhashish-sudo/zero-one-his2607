#include"AdmissionDAO.h"

bool AdmissionDAO::insertadmission(
    const oatpp::String& name,
    const oatpp::String& idCard,
    const oatpp::String& birthTime,
    const oatpp::Int32& age,
    const oatpp::String& sex,
    const oatpp::String& phone,
    const oatpp::String& admissionDept,
    const oatpp::String& admissionWard,
    const oatpp::String& attendingDoctor,
    const oatpp::String& admissionDiagnosis,
    const oatpp::String& diagnosisDesc,
    const oatpp::String& patientCondition,
    const oatpp::String& admissionType,
    const oatpp::String& admissionMode,
    const oatpp::String& feeNature,
    const oatpp::String& admissionDate,
    const oatpp::String& prepayAmount,
    const oatpp::String& payWay,
    const oatpp::String& tenantId,
    const oatpp::String& orgId,
    const oatpp::String& operUserId,
    oatpp::String& outPatientId,
    oatpp::String& outEncounterId,
    oatpp::String& outBusNo

)
{

    SqlParams params;

    // 简单入参校验
    if (!name || name->empty()) return false;
    if (!sex || sex->empty()) return false;
    if (age && age < 0) return false;

    // 事务SQL
    string sql = R"(
START TRANSACTION;

-- 1.插入患者主表 adm_patient
INSERT INTO adm_patient
(
    id, tenant_id, created_at, updated_at, created_by, updated_by,
    is_deleted, deleted_at, deleted_by, version, bus_no, name,
    gender_enum, birth_date, status_enum, active_flag, temp_flag,
    id_card, phone, address, nationality_code, organization_id
)
VALUES
(
    :patientId, :tenantId, NOW(6), NOW(6), :operUserId, :operUserId,
    0, NULL, NULL, 1, CONCAT('BR',DATE_FORMAT(NOW(),'%Y%m%d'),FLOOR(RAND()*1000)), :name,
    :sex, STR_TO_DATE(:birthTime,'%Y-%m-%d'), 'NORMAL', 1, 0,
    :idCard, :phone, '', 'CN', :orgId
);

-- 2.插入患者证件表 adm_patient_identifier（身份证）
INSERT INTO adm_patient_identifier
(
    id, tenant_id, created_at, updated_at, created_by, updated_by,
    is_deleted, deleted_at, deleted_by, version,
    type_code, identifier_no, state_enum, start_time, end_time, patient_id
)
VALUES
(
    UUID(), :tenantId, NOW(6), NOW(6), :operUserId, :operUserId,
    0, NULL, NULL, 1,
    'IDCARD', :idCard, 'VALID', NOW(6), NULL, :patientId
);

-- 3.插入住院就诊主表 adm_encounter 核心住院单
INSERT INTO adm_encounter
(
    id, tenant_id, created_at, updated_at, created_by, updated_by,
    is_deleted, deleted_at, deleted_by, version,
    patient_id, group_id, bus_no, status_enum, class_enum,
    admit_source_code, in_way_code, type_enum, start_time, end_time,
    organization_id, priority_enum, reception_time
)
VALUES
(
    :encounterId, :tenantId, NOW(6), NOW(6), :operUserId, :operUserId,
    0, NULL, NULL, 1,
    :patientId, NULL, :encounterBusNo, 'INHOSPITAL', 'COMMON',
    :admissionType, :admissionMode, :feeNature, STR_TO_DATE(:admissionDate,'%Y-%m-%d'), NULL,
    :orgId, :patientCondition, NOW(6)
);

-- 4.插入入院诊断表 adm_encounter_diagnosis
INSERT INTO adm_encounter_diagnosis
(
    id, tenant_id, created_at, updated_at, created_by, updated_by,
    is_deleted, deleted_at, deleted_by, version,
    encounter_id, condition_id, primary_flag, rank, description,
    diag_type_code, med_type, highest_diag_flag, tcm_flag
)
VALUES
(
    UUID(), :tenantId, NOW(6), NOW(6), :operUserId, :operUserId,
    0, NULL, NULL, 1,
    :encounterId, NULL, 1, 1, :diagnosisDesc,
    'ADMIT_DIAG', 'WEST', 1, 0
);

-- 5.插入就诊参与人员（主治医生）adm_encounter_participant
INSERT INTO adm_encounter_participant
(
    id, tenant_id, created_at, updated_at, created_by, updated_by,
    is_deleted, deleted_at, deleted_by, version,
    encounter_id, role_code, practitioner_id, status_enum, start_time, end_time
)
VALUES
(
    UUID(), :tenantId, NOW(6), NOW(6), :operUserId, :operUserId,
    0, NULL, NULL, 1,
    :encounterId, 'ATTENDING_DOCTOR', :attendingDoctor, 'ON_DUTY', NOW(6), NULL
);

-- 6.插入入院病区床位记录 adm_encounter_location
INSERT INTO adm_encounter_location
(
    id, tenant_id, created_at, updated_at, created_by, updated_by,
    is_deleted, deleted_at, deleted_by, version,
    encounter_id, location_id, form_enum, status_enum, start_time, end_time
)
VALUES
(
    UUID(), :tenantId, NOW(6), NOW(6), :operUserId, :operUserId,
    0, NULL, NULL, 1,
    :encounterId, :admissionWard, 'WARD_BED', 'OCCUPY', NOW(6), NULL
);

-- 7.插入住院预交金账户 adm_account
INSERT INTO adm_account
(
    id, tenant_id, created_at, updated_at, created_by, updated_by,
    is_deleted, deleted_at, deleted_by, version,
    patient_id, encounter_id, type_code, bus_no, name, balance,
    billing_status_enum, encounter_flag, insutype, default_flag, status_enum
)
VALUES
(
    UUID(), :tenantId, NOW(6), NOW(6), :operUserId, :operUserId,
    0, NULL, NULL, 1,
    :patientId, :encounterId, 'PREPAY', CONCAT('AC',DATE_FORMAT(NOW(),'%Y%m%d'),FLOOR(RAND()*1000)),
    CONCAT(:name,'住院预交账户'), :prepayAmount,
    'UNSETTLE', 1, :feeNature, 1, 'NORMAL'
);

COMMIT;
)";

    sqlSession->executeInsert(sql, params);
    return true;
}


bool AdmissionDAO::deleteadmission(
    const oatpp::String& encounterId,
    const oatpp::String& cancelReason,
    const oatpp::String& operUserId,
    const oatpp::String& tenantId
)
{
    string sql = R"(
START TRANSACTION;

DELETE FROM adm_account WHERE encounter_id = :encounterId AND tenant_id = :tenantId;
DELETE FROM adm_encounter_location WHERE encounter_id = :encounterId AND tenant_id = :tenantId;
DELETE FROM adm_encounter_participant WHERE encounter_id = :encounterId AND tenant_id = :tenantId;
DELETE FROM adm_encounter_diagnosis WHERE encounter_id = :encounterId AND tenant_id = :tenantId;
DELETE FROM adm_encounter WHERE id = :encounterId AND tenant_id = :tenantId;

COMMIT;
)";
    sqlSession->executeUpdate(sql);
    return true;
}