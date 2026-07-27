#pragma once
#ifndef _WARD_NAME_MAPPER_H_
#define _WARD_NAME_MAPPER_H_

#include "Mapper.h"
#include "domain/do/ward/WardDO.h"

class WardNameMapper : public Mapper<WardDO>
{
public:
    WardDO mapper(ResultSet* resultSet) const override
    {
        WardDO data;
        data.setId(resultSet->getString("id"));
        data.setName(resultSet->getString("name"));
        return data;
    }
};

#endif
