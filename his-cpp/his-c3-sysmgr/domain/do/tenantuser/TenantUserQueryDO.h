#pragma once
#ifndef _TENANTUSERQUERYDO_H_
#define _TENANTUSERQUERYDO_H_

#include "../DoInclude.h"

class TenantUserQueryDO
{
	CC_SYNTHESIZE(string, id, Id);
	CC_SYNTHESIZE(string, tenantId, TenantId);
	CC_SYNTHESIZE(string, userId, UserId);
	CC_SYNTHESIZE(string, userName, UserName);
	CC_SYNTHESIZE(string, name, Name);
	CC_SYNTHESIZE(string, phoneNumber, PhoneNumber);
	CC_SYNTHESIZE(string, email, Email);
	CC_SYNTHESIZE(int, isActive, IsActive);
	CC_SYNTHESIZE(string, createdAt, CreatedAt);
	CC_SYNTHESIZE(string, createdBy, CreatedBy);
};

typedef std::shared_ptr<TenantUserQueryDO> PtrTenantUserQueryDO;

#endif // !_TENANTUSERQUERYDO_H_
