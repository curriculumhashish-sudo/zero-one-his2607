#pragma once
#ifndef _INVENTORYMATERIALDO_H_
#define _INVENTORYMATERIALDO_H_

#include "../DoInclude.h"

class InventoryMaterialDO : public BaseDO {
	// Id
	MYSQL_SYNTHESIZE(string, id, Id);

	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);

	// 最后更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);

	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);

	// 修改人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);

	// 是否删除
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);

	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);

	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);

	// 版本号
	MYSQL_SYNTHESIZE(int, version, Version);

	// 库存编号
	MYSQL_SYNTHESIZE(string, inventoryId, InventoryId);

	// 锁定标识
	MYSQL_SYNTHESIZE(int, lockFlag, LockFlag);

	// 状态
	MYSQL_SYNTHESIZE(string, status, Status);

	// 业务单号
	MYSQL_SYNTHESIZE(string, busNo, BusNo);

	// 排序标识
	MYSQL_SYNTHESIZE(int, orderFlag, OrderFlag);

public:

	InventoryMaterialDO() : BaseDO("inventory_lock")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("inventory_id", "s", inventoryId);
		MYSQL_ADD_FIELD("lock_flag", "i", lockFlag);
		MYSQL_ADD_FIELD("status", "s", status);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("order_flag", "i", orderFlag);
	}
};

#endif // !_INVENTORYMATERIALDO_H_
