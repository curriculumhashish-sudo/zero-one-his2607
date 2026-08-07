#pragma once

#ifndef _GET_PATIENT_QUEUE_MAPPER_H_
#define _GET_PATIENT_QUEUE_MAPPER_H_

#include "Mapper.h"
#include "domain/do/charge/GetPatientQueueDO.h"

/**
 * GetPatientQueueDO 的结果集映射器
 * 将主 JOIN + 聚合查询的每一行映射为 GetPatientQueueDO 实例
 * SQL 列别名必须与 getString() 的参数名严格一致
 */
class GetPatientQueueMapper : public Mapper<GetPatientQueueDO>
{
public:
    GetPatientQueueDO mapper(ResultSet* resultSet) const override
    {
        GetPatientQueueDO data;

        // 就诊字段
        data.setEncounterId(resultSet->getString("encounter_id"));
        data.setEncounterBusNo(resultSet->getString("encounter_bus_no"));
        data.setStartTime(resultSet->getString("start_time"));

        // 患者字段
        data.setPatientId(resultSet->getString("patient_id"));
        data.setPatientName(resultSet->getString("patient_name"));
        data.setGenderCode(resultSet->getString("gender_code"));
        data.setBirthDate(resultSet->getString("birth_date"));

        // 科室字段
        data.setOrganizationName(resultSet->getString("organization_name"));

        // 医生字段
        data.setDoctorName(resultSet->getString("doctor_name"));

        // 聚合字段
        data.setTotalAmount(resultSet->getString("total_amount"));
        data.setPaidAmount(resultSet->getString("paid_amount"));
        data.setPaymentId(resultSet->getString("payment_id"));
        data.setPaymentType(resultSet->getString("payment_type"));

        return data;
    }
};

#endif // !_GET_PATIENT_QUEUE_MAPPER_H_
