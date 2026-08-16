#pragma once
#include "Mapper.h"
#include "../../domain/do/settle/GetCategoryDetailsDO.h"

class GetCategoryDetailsMapper : public Mapper<GetCategoryDetailsDO>
{
public:
    GetCategoryDetailsDO mapper(ResultSet* resultSet) const override
    {
        GetCategoryDetailsDO data;
        data.setSort_num(resultSet->getInt("sort_num"));
        data.setCharge_type_name(resultSet->getString("charge_type_name"));
        data.setType_total_fee(resultSet->getDouble("type_total_fee"));
        return data;
    }
};
