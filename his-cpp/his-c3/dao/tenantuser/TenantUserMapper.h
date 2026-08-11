#pragma once
#ifndef _TENANTUSERMAPPER_H_
#define _TENANTUSERMAPPER_H_

#include "Mapper.h"
#include "domain/do/tenantuser/TenantUserQueryDO.h"

class TenantUserMapper : public Mapper<PtrTenantUserQueryDO>
{
public:
	PtrTenantUserQueryDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<TenantUserQueryDO>();
		data->setId(resultSet->getString("id"));
		data->setTenantId(resultSet->getString("tenant_id"));
		data->setUserId(resultSet->getString("user_id"));
		data->setUserName(resultSet->getString("user_name"));
		data->setName(resultSet->getString("name"));
		data->setPhoneNumber(resultSet->getString("phone_number"));
		data->setEmail(resultSet->getString("email"));
		data->setIsActive(resultSet->getInt("is_active"));
		data->setCreatedAt(resultSet->getString("created_at"));
		data->setCreatedBy(resultSet->getString("created_by"));
		return data;
	}
};

#endif // !_TENANTUSERMAPPER_H_
