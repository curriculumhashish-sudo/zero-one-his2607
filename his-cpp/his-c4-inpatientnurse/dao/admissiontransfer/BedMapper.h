#pragma once
#ifndef _BED_MAPPER_
#define _BED_MAPPER_
#include "Mapper.h"
#include "../../domain/do/admissiontransfer/adm_locationDO.h"

// Mapper for adm_location free-bed query
class BedMapper : public Mapper<adm_locationDO>
{
public:
    adm_locationDO mapper(ResultSet* resultSet) const override
    {
        adm_locationDO data;
        data.setId(resultSet->getString(1));
        data.setName(resultSet->getString(2));
        data.setParent_id(resultSet->getString(3));
        data.setForm_enum(resultSet->getString(4));
        data.setStatus_enum(resultSet->getString(5));
        data.setOrganization_id(resultSet->getString(6));
        return data;
    }
};

#endif // !_BED_MAPPER_
