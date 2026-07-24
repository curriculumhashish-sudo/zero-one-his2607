#pragma once

#ifndef _USERDO_H_
#define _USERDO_H_

#include "../DoInclude.h"

/**
 * 
 */
class InventoryLockDO : public BaseDO
{
	// 器材ID
	MYSQL_SYNTHESIZE(string, id, Id);
	// 住户ID
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
public:
	InventoryLockDO() : BaseDO("inventory_lock")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		
	}
};

// 定义一个用户实体的智能指针
typedef std::shared_ptr<InventoryLockDO> PtrInventoryLockDO;

#endif // !_USERDO_H_