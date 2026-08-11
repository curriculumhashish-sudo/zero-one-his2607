#pragma once
#ifndef _RENT_MAPPER_
#define _RENT_MAPPER_

#include "Mapper.h"
#include "domain/do/rent/RentDO.h"

class RentMapper : public Mapper<RentDO>
{
public:
	RentDO mapper(ResultSet* resultSet) const override
	{
		auto data = RentDO();
		data.setId(resultSet->getInt("id"));
		data.setName(resultSet->getString("name"));
		data.setCode(resultSet->getString("code"));
		data.setDescription(resultSet->getString("description"));
		data.setIsActive(resultSet->getInt("is_active"));
		data.setExtraProperties(resultSet->getString("extra_properties"));
		data.setCreatedAt(resultSet->getString("created_at"));
		data.setUpdatedAt(resultSet->getString("updated_at"));
		data.setCreatedBy(resultSet->getString("created_by"));
		data.setUpdatedBy(resultSet->getString("updated_by"));
		data.setIsDeleted(resultSet->getInt("is_deleted"));
		data.setDeletedAt(resultSet->getString("deleted_at"));
		data.setDeletedBy(resultSet->getString("deleted_by"));
		return data;
	}
};

class RentNameMapper : public Mapper<RentDO>
{
public:
	RentDO mapper(ResultSet* resultSet) const override
	{
		auto data = RentDO();
		data.setId(resultSet->getInt("id"));
		data.setName(resultSet->getString("name"));
		data.setCode(resultSet->getString("code"));
		return data;
	}
};

#endif // !_RENT_MAPPER_