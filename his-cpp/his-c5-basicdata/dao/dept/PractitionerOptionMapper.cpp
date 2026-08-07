#include "stdafx.h"
#include "PractitionerOptionMapper.h"

PractitionerOptionDTO::Wrapper PractitionerOptionMapper::mapper(
    ResultSet* resultSet
) const
{
    auto data = PractitionerOptionDTO::createShared();

    data->id = resultSet->getString("id").c_str();
    data->name = resultSet->getString("name").c_str();

    if (resultSet->isNull("status_enum"))
    {
        data->status = "";
    }
    else
    {
        data->status = resultSet->getString("status_enum").c_str();
    }

    return data;
}
