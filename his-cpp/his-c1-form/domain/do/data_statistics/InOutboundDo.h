#pragma once

#ifndef _INOUTBOUNDDO_H_
#define _INOUTBOUNDDO_H_

#include "../DoInclude.h"

class InOutboundDO : public BaseDO
{
	// id
	MYSQL_SYNTHESIZE(string, id, Id);

	// 租户标识
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);

	// 批次单号
	MYSQL_SYNTHESIZE(string, busNo, BusNo);

	// 快照时间
	MYSQL_SYNTHESIZE(string, snapshotTime, SnapshotTime);

	// 源库存项标识
	MYSQL_SYNTHESIZE(string, inventoryItemId, InventoryItemId);

	// 物品定义表名
	MYSQL_SYNTHESIZE(string, itemTable, ItemTable);

	// 物品定义标识
	MYSQL_SYNTHESIZE(string, itemId, ItemId);

	// 收费项定义标识
	MYSQL_SYNTHESIZE(string, chargeItemDefinitionId, ChargeItemDefinitionId);

	// 库房标识
	MYSQL_SYNTHESIZE(string, warehouseId, WarehouseId);

	// 供应商标识
	MYSQL_SYNTHESIZE(string, supplierId, SupplierId);

	// 项目编码
	MYSQL_SYNTHESIZE(string, productBusNo, ProductBusNo);

	// 项目名称
	MYSQL_SYNTHESIZE(string, name, Name);

	// 商品名称
	MYSQL_SYNTHESIZE(string, merchandiseName, MerchandiseName);

	// 规格
	MYSQL_SYNTHESIZE(string, totalVolume, TotalVolume);

	// 厂家/产地
	MYSQL_SYNTHESIZE(string, manufacturer, Manufacturer);

	// 批准文号
	MYSQL_SYNTHESIZE(string, approvalNumber, ApprovalNumber);

	// 医保码
	MYSQL_SYNTHESIZE(string, ybNo, YbNo);

	// 追溯码
	MYSQL_SYNTHESIZE(string, traceNo, TraceNo);

	// 拼音码
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);

	// 五笔码
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);

	// 生产批号
	MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);

	// 数量
	MYSQL_SYNTHESIZE(double, quantity, Quantity);

	// 包装单位换算比例
	MYSQL_SYNTHESIZE(double, packageUnitRatio, PackageUnitRatio);

	// 包装单位编码
	MYSQL_SYNTHESIZE(string, packageUnitCode, PackageUnitCode);

	// 批号进价
	MYSQL_SYNTHESIZE(double, lotNumberCostAmount, LotNumberCostAmount);

	// 批号售价
	MYSQL_SYNTHESIZE(double, lotNumberPriceAmount, LotNumberPriceAmount);

	// 进价行总额
	MYSQL_SYNTHESIZE(double, lineCostTotal, LineCostTotal);

	// 售价行总额
	MYSQL_SYNTHESIZE(double, linePriceTotal, LinePriceTotal);

	// 生产日期
	MYSQL_SYNTHESIZE(string, productionDate, ProductionDate);

	// 有效期
	MYSQL_SYNTHESIZE(string, expirationDate, ExpirationDate);

	// 剩余过期天数
	MYSQL_SYNTHESIZE(int32_t, remainingDays, RemainingDays);

	// 供应状态
	MYSQL_SYNTHESIZE(string, inventoryStatusEnum, InventoryStatusEnum);

	// 医保等级码
	MYSQL_SYNTHESIZE(int32_t, chrgitmLv, ChrgitmLv);

	// 药品分类编码
	MYSQL_SYNTHESIZE(string, medicationCategoryCode, MedicationCategoryCode);

	// 耗材分类编码
	MYSQL_SYNTHESIZE(string, deviceCategoryCode, DeviceCategoryCode);

	// 剂型编码
	MYSQL_SYNTHESIZE(string, doseFormCode, DoseFormCode);

	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);

	// 最后更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);

	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);

	// 最后更新人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);

	// 删除标记
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);

	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);

	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);


public:

	InOutboundDO() : BaseDO("doc_inventory_item_snapshot")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);

		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("snapshot_time", "s", snapshotTime);

		MYSQL_ADD_FIELD("inventory_item_id", "s", inventoryItemId);
		MYSQL_ADD_FIELD("item_table", "s", itemTable);
		MYSQL_ADD_FIELD("item_id", "s", itemId);

		MYSQL_ADD_FIELD("charge_item_definition_id", "s", chargeItemDefinitionId);

		MYSQL_ADD_FIELD("warehouse_id", "s", warehouseId);
		MYSQL_ADD_FIELD("supplier_id", "s", supplierId);

		MYSQL_ADD_FIELD("product_bus_no", "s", productBusNo);

		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("merchandise_name", "s", merchandiseName);
		MYSQL_ADD_FIELD("total_volume", "s", totalVolume);

		MYSQL_ADD_FIELD("manufacturer", "s", manufacturer);
		MYSQL_ADD_FIELD("approval_number", "s", approvalNumber);

		MYSQL_ADD_FIELD("yb_no", "s", ybNo);
		MYSQL_ADD_FIELD("trace_no", "s", traceNo);

		MYSQL_ADD_FIELD("py_str", "s", pyStr);
		MYSQL_ADD_FIELD("wb_str", "s", wbStr);

		MYSQL_ADD_FIELD("lot_number", "s", lotNumber);

		MYSQL_ADD_FIELD("quantity", "d", quantity);

		MYSQL_ADD_FIELD("package_unit_ratio", "d", packageUnitRatio);
		MYSQL_ADD_FIELD("package_unit_code", "s", packageUnitCode);

		MYSQL_ADD_FIELD("lot_number_cost_amount", "d", lotNumberCostAmount);
		MYSQL_ADD_FIELD("lot_number_price_amount", "d", lotNumberPriceAmount);

		MYSQL_ADD_FIELD("line_cost_total", "d", lineCostTotal);
		MYSQL_ADD_FIELD("line_price_total", "d", linePriceTotal);

		MYSQL_ADD_FIELD("production_date", "s", productionDate);
		MYSQL_ADD_FIELD("expiration_date", "s", expirationDate);

		MYSQL_ADD_FIELD("remaining_days", "i", remainingDays);

		MYSQL_ADD_FIELD("inventory_status_enum", "s", inventoryStatusEnum);

		MYSQL_ADD_FIELD("chrgitm_lv", "i", chrgitmLv);

		MYSQL_ADD_FIELD("medication_category_code", "s", medicationCategoryCode);
		MYSQL_ADD_FIELD("device_category_code", "s", deviceCategoryCode);
		MYSQL_ADD_FIELD("dose_form_code", "s", doseFormCode);

		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);

		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);

		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);

		MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
	}
};

#endif // !__INOUTBOUNDDO_H_