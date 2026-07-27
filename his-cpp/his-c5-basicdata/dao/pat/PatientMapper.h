// PatientMapper.h
#pragma once
#ifndef _PATIENT_MAPPER_H_
#define _PATIENT_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/pat/PatientDO.h"


/**
 * 患者选择列表查询Mapper 
 */
class PtrPatientSelectMapper : public Mapper<PtrPatientDO>
{
public:
    PtrPatientDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<PatientDO>();
        // 只映射前端需要的7个字段
        data->setBusNo(resultSet->getString("bus_no"));        // 病历号
        data->setName(resultSet->getString("name"));           // 姓名
        data->setGender(resultSet->getString("gender")); // 性别
        data->setBirthDate(resultSet->getString("birth_date"));   // 出生日期
        data->setPhone(resultSet->getString("phone"));         // 联系电话
        data->setIdCard(resultSet->getString("id_card"));      // 身份证号
        return data;
    }
};

/**
 * 统计患者总数Mapper
 */
class PatientCountMapper : public Mapper<int>
{
public:
    int mapper(ResultSet* resultSet) const override
    {
        if (resultSet && resultSet->next())
        {
            return resultSet->getInt(1);
        }
        return 0;
    }
};

#endif // !_PATIENT_MAPPER_H_