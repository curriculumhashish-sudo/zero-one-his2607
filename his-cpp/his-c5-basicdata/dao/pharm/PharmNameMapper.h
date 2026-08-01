#pragma once
#ifndef _PHARM_NAME_MAPPER_H_
#define _PHARM_NAME_MAPPER_H_

#include "Mapper.h"
#include "domain/do/pharm/PharmDO.h"

// Lightweight mapper for dropdown selection list: only maps id and name
class PharmNameMapper : public Mapper<PtrPharmDO>
{
public:
    PtrPharmDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<PharmDO>();
        data->setId(resultSet->getUInt64("id"));
        data->setName(resultSet->getString("name"));
        return data;
    }
};

#endif // !_PHARM_NAME_MAPPER_H_
