#pragma once

#include "Mapper.h"
#include "../../domain/do/enroll/EnrollListDO.h"

// 将 SQL 查询结果的一行数据映射为一条住院登记列表数据。
class EnrollListMapper : public Mapper<EnrollListDO>
{
public:
    // 框架会对查询结果的每一行调用一次这个方法。
    EnrollListDO mapper(ResultSet* resultSet) const override {
        // 创建当前结果行对应的列表数据对象。
        EnrollListDO data;

        // SQL 中的列名或别名必须与这里完全一致。
        data.setEncounterId(resultSet->getString("encounter_id"));
        data.setEncounterBusNo(resultSet->getString("encounter_bus_no"));
        data.setPatientName(resultSet->getString("patient_name"));
        data.setStatusEnum(resultSet->getString("status_enum"));
        data.setOrganizationName(resultSet->getString("organization_name"));
        data.setWardName(resultSet->getString("ward_name"));
        data.setBedName(resultSet->getString("bed_name"));
        data.setStartTime(resultSet->getString("start_time"));

        return data;
    }
};