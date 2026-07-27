#include "stdafx.h"
#include "PharmMapper.h"

std::string PharmMapper::readNullableString(ResultSet* resultSet, const std::string& column) const
{
	if (resultSet->isNull(column))
	{
		return "";
	}
	return resultSet->getString(column);
}

PtrPharmDO PharmMapper::mapper(ResultSet* resultSet) const
{
	// 用智能指针对象创建，和 OrganizationMapper 写法完全一致
	auto data = std::make_shared<PharmDO>();

	data->setId(resultSet->getUInt64("id"));
	data->setName(readNullableString(resultSet, "name"));
	data->setType(readNullableString(resultSet, "type"));
	data->setStatus(resultSet->getInt("status"));
	data->setSort(resultSet->getInt("sort"));
	data->setRemark(readNullableString(resultSet, "remark"));
	data->setCreate_By(readNullableString(resultSet, "create_by"));
	data->setUpdate_By(readNullableString(resultSet, "update_by"));
	data->setCreate_Time(readNullableString(resultSet, "create_time"));
	data->setUpdate_Time(readNullableString(resultSet, "update_time"));

	return data;
}