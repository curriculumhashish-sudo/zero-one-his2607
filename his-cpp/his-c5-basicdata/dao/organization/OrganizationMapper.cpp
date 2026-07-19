#include"stdafx.h"
#include"OrganizationMapper.h"

std::string OrganizationMapper::readNullableString(ResultSet* resultSet, const std::string& column) const
{
    if (resultSet->isNull(column))
    {
        return "";
    }

    return resultSet->getString(column);
}


PtrOrganizationDO OrganizationMapper::mapper(ResultSet* resultSet) const override
{
    // Map one SQL result row into the organization domain object.
    auto data = std::make_shared<OrganizationDO>();

    data->setId(resultSet->getString("id"));
    data->setName(resultSet->getString("name"));
    data->setParent_Id(readNullableString(resultSet, "parent_id"));
    data->setType_Enum(resultSet->getString("type_enum"));
    data->setClass_Enum(readNullableString(resultSet, "class_enum"));
    data->setStatus_Enum(resultSet->getString("status_enum"));
    data->setYb_No(readNullableString(resultSet, "yb_no"));
    data->setYb_Name(readNullableString(resultSet, "yb_name"));
    data->setMedins_Id(readNullableString(resultSet, "medins_id"));
    data->setMedins_Admdvs(readNullableString(resultSet, "medins_admdvs"));
    data->setMedins_Type(readNullableString(resultSet, "medins_type"));
    data->setMedins_Lv(readNullableString(resultSet, "medins_lv"));
    data->setPy_Str(readNullableString(resultSet, "py_str"));
    data->setWb_Str(readNullableString(resultSet, "wb_str"));

    if (resultSet->isNull("display_order"))
    {
        data->setDisplay_Order(0);
    }
    else
    {
        data->setDisplay_Order(resultSet->getInt("display_order"));
    }

    return data;
}
