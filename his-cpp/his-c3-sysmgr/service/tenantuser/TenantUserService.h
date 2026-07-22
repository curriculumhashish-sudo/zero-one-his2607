#pragma once
#ifndef _TENANTUSERSERVICE_H_
#define _TENANTUSERSERVICE_H_

#include "domain/query/tenantuser/TenantUserQuery.h"
#include "domain/dto/tenantuser/TenantUserDTO.h"
#include "domain/dto/tenantuser/TenantUserBatchAddDTO.h"
#include "domain/dto/tenantuser/TenantUserBatchRemoveDTO.h"

class TenantUserService
{
public:
	TenantUserPageDTO::Wrapper listAll(const TenantUserQuery::Wrapper& query);
	oatpp::List<oatpp::String> saveBatch(const TenantUserBatchAddDTO::Wrapper& dto);
	oatpp::List<oatpp::String> removeBatch(const TenantUserBatchRemoveDTO::Wrapper& dto);
};

#endif // !_TENANTUSERSERVICE_H_
