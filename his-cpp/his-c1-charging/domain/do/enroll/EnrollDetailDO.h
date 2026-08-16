#pragma once

#include "../DoInclude.h"

/**
 * @brief 住院登记详情数据对象。
 *
 * 用于承接 DAO 查询住院登记详情时返回的一行数据库结果。
 * 字段名与 SQL 查询中的列别名保持一致，不承担状态翻译、
 * 年龄计算等业务处理。
 */
class EnrollDetailDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    MYSQL_SYNTHESIZE(string, encounterBusNo, EncounterBusNo);
    MYSQL_SYNTHESIZE(string, patientId, PatientId);
    MYSQL_SYNTHESIZE(string, patientName, PatientName);
    MYSQL_SYNTHESIZE(string, idCard, IdCard);
    MYSQL_SYNTHESIZE(string, genderEnum, GenderEnum);
    MYSQL_SYNTHESIZE(string, birthDate, BirthDate);
    MYSQL_SYNTHESIZE(string, phone, Phone);

    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(string, organizationId, OrganizationId);
    MYSQL_SYNTHESIZE(string, organizationName, OrganizationName);
    MYSQL_SYNTHESIZE(string, wardId, WardId);
    MYSQL_SYNTHESIZE(string, wardName, WardName);
    MYSQL_SYNTHESIZE(string, bedId, BedId);
    MYSQL_SYNTHESIZE(string, bedName, BedName);
    MYSQL_SYNTHESIZE(string, startTime, StartTime);

    MYSQL_SYNTHESIZE(string, conditionDefinitionId, ConditionDefinitionId);
    MYSQL_SYNTHESIZE(string, conditionDefinitionName, ConditionDefinitionName);
    MYSQL_SYNTHESIZE(string, diagnosisDescription, DiagnosisDescription);

    MYSQL_SYNTHESIZE(string, admitSourceCode, AdmitSourceCode);
    MYSQL_SYNTHESIZE(string, admitSourceCodeText, AdmitSourceCodeText);
    MYSQL_SYNTHESIZE(string, inWayCode, InWayCode);
    MYSQL_SYNTHESIZE(string, inWayCodeText, InWayCodeText);
    MYSQL_SYNTHESIZE(string, priorityEnum, PriorityEnum);
    MYSQL_SYNTHESIZE(string, outpatientDoctorName, OutpatientDoctorName);
    MYSQL_SYNTHESIZE(string, attendingDoctorId, AttendingDoctorId);
    MYSQL_SYNTHESIZE(string, attendingDoctorName, AttendingDoctorName);
    MYSQL_SYNTHESIZE(int, admissionCount, AdmissionCount);

public:
    /**
     * @brief 初始化住院登记详情字段与数据库列名的映射关系。
     *
     * @param 无。
     * @return 无。
     *
     * @details
     * MYSQL_ADD_FIELD 的第一个参数必须与详情 SQL 中的 AS 别名一致。
     * 例如 SQL 写 AS patient_name，这里必须写 patient_name。
     */
    EnrollDetailDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD_PK("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("encounter_bus_no", "s", encounterBusNo);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("patient_name", "s", patientName);
        MYSQL_ADD_FIELD("id_card", "s", idCard);
        MYSQL_ADD_FIELD("gender_enum", "s", genderEnum);
        MYSQL_ADD_FIELD("birth_date", "s", birthDate);
        MYSQL_ADD_FIELD("phone", "s", phone);

        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("organization_id", "s", organizationId);
        MYSQL_ADD_FIELD("organization_name", "s", organizationName);
        MYSQL_ADD_FIELD("ward_id", "s", wardId);
        MYSQL_ADD_FIELD("ward_name", "s", wardName);
        MYSQL_ADD_FIELD("bed_id", "s", bedId);
        MYSQL_ADD_FIELD("bed_name", "s", bedName);
        MYSQL_ADD_FIELD("start_time", "s", startTime);

        MYSQL_ADD_FIELD("condition_definition_id", "s", conditionDefinitionId);
        MYSQL_ADD_FIELD("condition_definition_name", "s", conditionDefinitionName);
        MYSQL_ADD_FIELD("diagnosis_description", "s", diagnosisDescription);

        MYSQL_ADD_FIELD("admit_source_code", "s", admitSourceCode);
        MYSQL_ADD_FIELD("admit_source_code_text", "s", admitSourceCodeText);
        MYSQL_ADD_FIELD("in_way_code", "s", inWayCode);
        MYSQL_ADD_FIELD("in_way_code_text", "s", inWayCodeText);
        MYSQL_ADD_FIELD("priority_enum", "s", priorityEnum);
        MYSQL_ADD_FIELD("outpatient_doctor_name", "s", outpatientDoctorName);
        MYSQL_ADD_FIELD("attending_doctor_id", "s", attendingDoctorId);
        MYSQL_ADD_FIELD("attending_doctor_name", "s", attendingDoctorName);
        MYSQL_ADD_FIELD("admission_count", "i", admissionCount);
    }
};

/**
 * @brief 住院登记详情数据对象的共享指针类型。
 *
 * 用于表达单条详情查询结果。
 * 当数据库未查询到对应登记时，该指针为 nullptr。
 */
typedef std::shared_ptr<EnrollDetailDO> PtrEnrollDetailDO;
