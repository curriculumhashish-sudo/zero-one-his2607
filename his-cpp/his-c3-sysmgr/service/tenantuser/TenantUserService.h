#pragma once
#ifndef _TENANTUSERSERVICE_H_
#define _TENANTUSERSERVICE_H_

#include "domain/query/tenantuser/TenantUserQuery.h"
#include "domain/dto/tenantuser/TenantUserDTO.h"
#include "domain/dto/tenantuser/TenantUserBatchAddDTO.h"
#include "domain/dto/tenantuser/TenantUserBatchRemoveDTO.h"

struct TenantUserBatchResult
{
	bool success = false;
	oatpp::List<oatpp::String> userIds = oatpp::List<oatpp::String>::createShared();
};

class TenantUserService
{
public:
	TenantUserPageDTO::Wrapper listAll(const TenantUserQuery::Wrapper& query);
	TenantUserBatchResult saveBatch(const TenantUserBatchAddDTO::Wrapper& dto);
	TenantUserBatchResult removeBatch(const TenantUserBatchRemoveDTO::Wrapper& dto);
};

#endif // !_TENANTUSERSERVICE_H_
