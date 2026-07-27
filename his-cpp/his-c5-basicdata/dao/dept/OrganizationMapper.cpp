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


PtrAdmOrganizationDO OrganizationMapper::mapper(ResultSet* resultSet) const
{
    auto data = std::make_shared<AdmOrganizationDO>();

    data->setId(resultSet->getString("id"));
    data->setName(resultSet->getString("name"));
    data->setBusNo(resultSet->getString("bus_no"));
    data->setParentId(readNullableString(resultSet, "parent_id"));
    data->setTypeEnum(resultSet->getString("type_enum"));
    data->setClassEnum(readNullableString(resultSet, "class_enum"));
    data->setStatusEnum(resultSet->getString("status_enum"));
    data->setDefDoctorId(readNullableString(resultSet, "def_doctor_id"));
    data->setCaty(readNullableString(resultSet, "caty"));
    data->setYbNo(readNullableString(resultSet, "yb_no"));
    data->setYbName(readNullableString(resultSet, "yb_name"));
    data->setMedinsId(readNullableString(resultSet, "medins_id"));
    data->setMedinsAdmdvs(readNullableString(resultSet, "medins_admdvs"));
    data->setMedinsType(readNullableString(resultSet, "medins_type"));
    data->setMedinsLv(readNullableString(resultSet, "medins_lv"));
    data->setPyStr(readNullableString(resultSet, "py_str"));
    data->setWbStr(readNullableString(resultSet, "wb_str"));

    if (resultSet->isNull("display_order"))
    {
        data->setDisplayOrder(0);
    }
    else
    {
        data->setDisplayOrder(resultSet->getInt("display_order"));
    }

    return data;
}
