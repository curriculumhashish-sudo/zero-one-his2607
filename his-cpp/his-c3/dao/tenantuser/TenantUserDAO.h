#pragma once
#ifndef _TENANTUSERDAO_H_
#define _TENANTUSERDAO_H_

#include "BaseDAO.h"
#include "domain/query/tenantuser/TenantUserQuery.h"
#include "domain/do/tenantuser/TenantUserRelDO.h"
#include "domain/do/tenantuser/TenantUserQueryDO.h"

class TenantUserDAO : public BaseDAO
{
public:
	uint64_t count(const TenantUserQuery::Wrapper& query);
	std::list<PtrTenantUserQueryDO> selectPage(const TenantUserQuery::Wrapper& query);
	std::list<std::string> selectExistingUserIds(const std::list<std::string>& userIds);
	std::list<std::string> selectBoundUserIds(const std::string& tenantId, const std::list<std::string>& userIds);
	int softDeleteByTenantAndUserIds(const std::string& tenantId, const std::list<std::string>& userIds, const std::string& deletedBy, const std::string& deletedAt);
};

#endif // !_TENANTUSERDAO_H_
