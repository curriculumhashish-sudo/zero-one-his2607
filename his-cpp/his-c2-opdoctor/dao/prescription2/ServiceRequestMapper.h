#pragma once

#ifndef _SERVICEREQUESTMAPPER_H_
#define _SERVICEREQUESTMAPPER_H_

#include "Mapper.h"
#include "domain/do/prescription2/ServiceRequestDO.h"

// 将服务申请单查询结果映射为领域对象。
class PtrServiceRequestMapper : public Mapper<PtrServiceRequestDO>
{
public:
    PtrServiceRequestDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<ServiceRequestDO>();
        data->setId(resultSet->getString(1));
        data->setPrintCount(resultSet->getInt(2));
        data->setUpdatedBy(resultSet->getString(3));
        data->setUpdatedAt(resultSet->getString(4));
        return data;
    }
};

#endif // !_SERVICEREQUESTMAPPER_H_
