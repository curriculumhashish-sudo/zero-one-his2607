#pragma once

#include "Mapper.h"
#include "../../domain/do/enroll/EnrollDetailDO.h"

/**
 * @brief 住院登记详情查询结果映射器。
 *
 * 将 SQL 查询结果中的一行数据转换为住院登记详情对象。
 * SQL 的列别名必须与 getString、getInt 中的名称一致。
 */
class EnrollDetailMapper : public Mapper<PtrEnrollDetailDO>
{
public:
    /**
     * @brief 将当前数据库结果行转换为住院登记详情对象。
     *
     * @param resultSet 当前查询结果行。
     * @return 保存住院登记详情的共享指针。
     *
     * @details
     * 本方法仅负责数据库字段到 DO 字段的数据搬运。
     * 状态中文、性别中文、年龄等业务数据由 Service 层处理。
     */
    PtrEnrollDetailDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<EnrollDetailDO>();

        data->setEncounterId(resultSet->getString("encounter_id"));
        data->setEncounterBusNo(resultSet->getString("encounter_bus_no"));
        data->setPatientId(resultSet->getString("patient_id"));
        data->setPatientName(resultSet->getString("patient_name"));
        data->setIdCard(resultSet->getString("id_card"));
        data->setGenderEnum(resultSet->getString("gender_enum"));
        data->setBirthDate(resultSet->getString("birth_date"));
        data->setPhone(resultSet->getString("phone"));

        data->setStatusEnum(resultSet->getString("status_enum"));
        data->setOrganizationId(resultSet->getString("organization_id"));
        data->setOrganizationName(resultSet->getString("organization_name"));
        data->setWardId(resultSet->getString("ward_id"));
        data->setWardName(resultSet->getString("ward_name"));
        data->setBedId(resultSet->getString("bed_id"));
        data->setBedName(resultSet->getString("bed_name"));
        data->setStartTime(resultSet->getString("start_time"));

        data->setConditionDefinitionId(
            resultSet->getString("condition_definition_id")
        );
        data->setConditionDefinitionName(
            resultSet->getString("condition_definition_name")
        );
        data->setDiagnosisDescription(
            resultSet->getString("diagnosis_description")
        );

        data->setAdmitSourceCode(
            resultSet->getString("admit_source_code")
        );
        data->setAdmitSourceCodeText(
            resultSet->getString("admit_source_code_text")
        );
        data->setInWayCode(resultSet->getString("in_way_code"));
        data->setInWayCodeText(resultSet->getString("in_way_code_text"));
        data->setPriorityEnum(resultSet->getString("priority_enum"));
        data->setOutpatientDoctorName(
            resultSet->getString("outpatient_doctor_name")
        );
        data->setAttendingDoctorId(
            resultSet->getString("attending_doctor_id")
        );
        data->setAttendingDoctorName(
            resultSet->getString("attending_doctor_name")
        );
        data->setAdmissionCount(resultSet->getInt("admission_count"));

        return data;
    }
};
