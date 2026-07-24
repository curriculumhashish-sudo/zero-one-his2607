#pragma once

#ifndef _EQUIPMENT_H_
#define _EQUIPMENT_H_

#include "../DoInclude.h"

/**
 * 库存锁定实体
 */
class InventoryLockDO : public BaseDO
{
	// 器材ID
	MYSQL_SYNTHESIZE(string, id, Id);
	// 住户ID
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// 最后更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// 最后更新人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// 软删标识
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// 乐观锁版本号
	MYSQL_SYNTHESIZE(int, version, Version);
	// 库存锁定记录
	MYSQL_SYNTHESIZE(string, inventoryId, InventoryId);
	// 锁定标识，ture已锁
	MYSQL_SYNTHESIZE(bool, lockFlag, lockFlag);
	// 锁定状态
	MYSQL_SYNTHESIZE(string, status, Status);
	// 锁定单号
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	// 同一业务单内顺序号
	MYSQL_SYNTHESIZE(int, orderFlag, orderFlag);
public:
	InventoryLockDO() : BaseDO("inventory_lock")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);

		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", tenantId, false);

		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);

		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);

		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);

		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);

		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);

		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);

		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);

		MYSQL_ADD_FIELD_NULLABLE("inventory_id", "s", inventoryId, false);

		MYSQL_ADD_FIELD_NULLABLE("lock_flag", "b", lockFlag, false);

		MYSQL_ADD_FIELD_NULLABLE("status", "s", status, false);

		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, false);

		MYSQL_ADD_FIELD_NULLABLE("order_flag", "i", orderFlag, false);
	}
};

// 定义一个器材实体的智能指针
typedef std::shared_ptr<InventoryLockDO> PtrInventoryLockDO;

#endif // !_EQUIPMENT_H_