#pragma once

#ifndef _SERVICEREQUESTMAPPER_H_
#define _SERVICEREQUESTMAPPER_H_

#include "Mapper.h"
#include "domain/do/prescription2/ServiceRequestDO.h"

class PtrServiceRequestMapper : public Mapper<PtrServiceRequestDO>
{
public:
    // 将服务申请单查询列映射为数据库对象
    PtrServiceRequestDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<ServiceRequestDO>();
        data->setId(resultSet->getString(1));
        if (!resultSet->isNull(2)) data->setPrescriptionNo(resultSet->getString(2));
        data->setEncounterId(resultSet->getString(3));
        data->setPatientId(resultSet->getString(4));
        data->setStatusEnum(resultSet->getString(5));
        data->setCategoryEnum(resultSet->getString(6));
        data->setQuantity(resultSet->getDouble(7));
        data->setUnitCode(resultSet->getString(8));
        if (!resultSet->isNull(9)) data->setAuthoredTime(resultSet->getString(9));
        data->setRequesterId(resultSet->getString(10));
        if (!resultSet->isNull(11)) data->setPerformerId(resultSet->getString(11));
        if (!resultSet->isNull(12)) data->setLocationId(resultSet->getString(12));
        if (!resultSet->isNull(13)) data->setOrgId(resultSet->getString(13));
        if (!resultSet->isNull(14)) data->setRemark(resultSet->getString(14));
        data->setPrintCount(resultSet->isNull(15) ? 0 : resultSet->getInt(15));
        data->setVersion(resultSet->getInt(16));
        if (!resultSet->isNull(17)) data->setUpdatedBy(resultSet->getString(17));
        if (!resultSet->isNull(18)) data->setUpdatedAt(resultSet->getString(18));
        return data;
    }
};

#endif // !_SERVICEREQUESTMAPPER_H_
