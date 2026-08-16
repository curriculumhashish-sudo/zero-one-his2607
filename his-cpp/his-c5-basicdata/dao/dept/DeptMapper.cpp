#include "stdafx.h"
#include "DeptMapper.h"

namespace
{
    std::string nullableString(ResultSet* resultSet, const std::string& column)
    {
        return resultSet->isNull(column) ? "" : resultSet->getString(column);
    }
}

PtrDeptDO DeptMapper::mapper(ResultSet* resultSet) const
{
    auto data = std::make_shared<DeptDO>();
    data->setId(resultSet->getString("id"));
    data->setTenantId(nullableString(resultSet, "tenant_id"));
    data->setName(resultSet->getString("name"));
    data->setBusNo(resultSet->getString("bus_no"));
    data->setParentId(nullableString(resultSet, "parent_id"));
    data->setTypeEnum(resultSet->getString("type_enum"));
    data->setClassEnum(nullableString(resultSet, "class_enum"));
    data->setStatusEnum(resultSet->getString("status_enum"));
    data->setDisplayOrder(resultSet->isNull("display_order") ? 0 : resultSet->getInt("display_order"));
    data->setDefDoctorId(nullableString(resultSet, "def_doctor_id"));
    data->setPyStr(nullableString(resultSet, "py_str"));
    data->setWbStr(nullableString(resultSet, "wb_str"));
    data->setYbNo(nullableString(resultSet, "yb_no"));
    data->setYbName(nullableString(resultSet, "yb_name"));
    data->setMedinsId(nullableString(resultSet, "medins_id"));
    data->setMedinsAdmdvs(nullableString(resultSet, "medins_admdvs"));
    data->setMedinsType(nullableString(resultSet, "medins_type"));
    data->setMedinsLv(nullableString(resultSet, "medins_lv"));
    data->setCaty(nullableString(resultSet, "caty"));
    data->setVersion(resultSet->getInt("version"));
    return data;
}

std::string DeptStringMapper::mapper(ResultSet* resultSet) const
{
    return resultSet->isNull(column) ? "" : resultSet->getString(column);
}
