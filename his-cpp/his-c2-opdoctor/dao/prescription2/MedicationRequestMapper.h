#pragma once

#ifndef _MEDICATIONREQUESTMAPPER_H_
#define _MEDICATIONREQUESTMAPPER_H_

#include "Mapper.h"
#include "domain/do/prescription2/MedicationRequestDO.h"

// 将完整医嘱查询结果映射为领域对象。
class PtrMedicationRequestMapper : public Mapper<PtrMedicationRequestDO>
{
public:
    PtrMedicationRequestDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<MedicationRequestDO>();
        data->setId(resultSet->getString(1));
        data->setStatusEnum(resultSet->getString(2));
        // 数据库列值为 NULL 时，可选字段必须保持未设置。
        if (!resultSet->isNull(3)) data->setStatusChangedTime(resultSet->getString(3));
        if (!resultSet->isNull(4)) data->setPerformerCheckId(resultSet->getString(4));
        if (!resultSet->isNull(5)) data->setCheckTime(resultSet->getString(5));
        if (!resultSet->isNull(6)) data->setUpdatedBy(resultSet->getString(6));
        if (!resultSet->isNull(7)) data->setUpdatedAt(resultSet->getString(7));
        return data;
    }
};

// 映射 SELECT ... FOR UPDATE 状态校验所需的最小结果集。
class PtrMedicationRequestStatusMapper : public Mapper<PtrMedicationRequestDO>
{
public:
    PtrMedicationRequestDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<MedicationRequestDO>();
        data->setId(resultSet->getString(1));
        data->setStatusEnum(resultSet->getString(2));
        return data;
    }
};

#endif // !_MEDICATIONREQUESTMAPPER_H_
