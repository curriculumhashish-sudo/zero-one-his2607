#pragma once

#ifndef _INVENTORYITEMDO_H_
#define _INVENTORYITEMDO_H_

#include "../DoInclude.h"

/**
	* 库存物品（器材）实体
	*/
class InventoryItemDO : public BaseDO
{
	// 主键
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户标识
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// 最后更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// 最后更新人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// 软删标记 (0未删, 1已删)
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// 乐观锁版本号
	MYSQL_SYNTHESIZE(int, version, Version);

	// 库存项聚合根
	MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
	// 物品定义表名
	MYSQL_SYNTHESIZE(string, itemTable, ItemTable);
	// 物品定义标识
	MYSQL_SYNTHESIZE(string, itemId, ItemId);
	// 物品名称（冗余）
	MYSQL_SYNTHESIZE(string, name, Name);
	// 拼音码
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	// 五笔码
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	// 库存状态（ACTIVE启用/INACTIVE停用）
	MYSQL_SYNTHESIZE(string, inventoryStatusEnum, InventoryStatusEnum);
	// 供应商标识
	MYSQL_SYNTHESIZE(string, supplierId, SupplierId);
	// 描述文本
	MYSQL_SYNTHESIZE(string, descriptionText, DescriptionText);
	// 计量单位编码
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	// 当前库存数量 (decimal 通常用 double)
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	// 特征扩展 JSON
	MYSQL_SYNTHESIZE(string, characteristicJson, CharacteristicJson);
	// 产品批号
	MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
	// 生产日期
	MYSQL_SYNTHESIZE(string, productionDate, ProductionDate);
	// 效期
	MYSQL_SYNTHESIZE(string, expirationDate, ExpirationDate);
	// 有效期月数
	MYSQL_SYNTHESIZE(int, validityMon, ValidityMon);
	// 货位标识
	MYSQL_SYNTHESIZE(string, locationStoreId, LocationStoreId);
	// 库房标识
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	// 追溯码
	MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
	// 追溯码包装层级
	MYSQL_SYNTHESIZE(int, packagingLevels, PackagingLevels);

public:
	InventoryItemDO() : BaseDO("wkf_inventory_item") // 表名
	{
		// 基础字段映射
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);

		// 业务字段映射
		MYSQL_ADD_FIELD_NULLABLE("category_code", "s", categoryCode, true);
		MYSQL_ADD_FIELD_NULLABLE("item_table", "s", itemTable, false);
		MYSQL_ADD_FIELD_NULLABLE("item_id", "s", itemId, false);
		MYSQL_ADD_FIELD_NULLABLE("name", "s", name, false);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("inventory_status_enum", "s", inventoryStatusEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("supplier_id", "s", supplierId, false);
		MYSQL_ADD_FIELD_NULLABLE("description_text", "s", descriptionText, true);
		MYSQL_ADD_FIELD_NULLABLE("unit_code", "s", unitCode, false);
		MYSQL_ADD_FIELD_NULLABLE("quantity", "d", quantity, false);
		MYSQL_ADD_FIELD_NULLABLE("characteristic_json", "s", characteristicJson, true);
		MYSQL_ADD_FIELD_NULLABLE("lot_number", "s", lotNumber, false);
		MYSQL_ADD_FIELD_NULLABLE("production_date", "dt", productionDate, true);
		MYSQL_ADD_FIELD_NULLABLE("expiration_date", "dt", expirationDate, true);
		MYSQL_ADD_FIELD_NULLABLE("validity_mon", "i", validityMon, true);
		MYSQL_ADD_FIELD_NULLABLE("location_store_id", "s", locationStoreId, true);
		MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, false);
		MYSQL_ADD_FIELD_NULLABLE("trace_no", "s", traceNo, true);
		MYSQL_ADD_FIELD_NULLABLE("packaging_levels", "i", packagingLevels, true);
	}
};

// 定义一个库存物品（器材）实体的智能指针
typedef std::shared_ptr<InventoryItemDO> PtrInventoryItemDO;

#endif // !_INVENTORYITEMDO_H_