#pragma once
#ifndef _OUTPATIENTRECORD_MAPPER_
#define _OUTPATIENTRECORD_MAPPER_

#include "Mapper.h"
#include "../../domain/do/outpatientrecord2/OutpatientRecordMainDO.h"
#include "../../domain/do/outpatientrecord2/OutpatientRecordViewDO.h"

class OutpatientRecordViewMapper : public Mapper<PtrOutpatientRecordViewDO>
{
public:
    PtrOutpatientRecordViewDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<OutpatientRecordViewDO>();

        // 主表字段
        data->setId(resultSet->getString("id"));                       // 就诊ID
        data->setPatientId(resultSet->getString("patient_id"));
        data->setOrganizationId(resultSet->getString("organization_id"));
        data->setStartTime(resultSet->getString("start_time"));        // 就诊日期
        data->setCreatedAt(resultSet->getString("created_at"));
        data->setStatusEnum(resultSet->getString("status_enum"));
        data->setClassEnum(resultSet->getString("class_enum"));
        data->setBusNo(resultSet->getString("bus_no"));
        data->setIsDeleted(resultSet->getInt("is_deleted"));

        // 扩展字段（用于列表展示）
        data->setPatientName(resultSet->getString("patient_name"));    // 患者姓名
        data->setGender(resultSet->getString("gender"));               // 性别
        data->setDiagnosis(resultSet->getString("diagnosis"));         // 诊断
        data->setDoctorName(resultSet->getString("doctor_name"));      // 医生姓名
        data->setDeptName(resultSet->getString("dept_name"));          // 科室名称
        data->setRemark(resultSet->getString("remark"));               // 备注

        return data;
    }
};

#endif // !_OUTPATIENTRECORD_MAPPER_