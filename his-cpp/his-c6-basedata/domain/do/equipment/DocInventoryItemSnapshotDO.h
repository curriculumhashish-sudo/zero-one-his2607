#pragma once

#ifndef _INVENTORYSNAPSHOTDO_H_
#define _INVENTORYSNAPSHOTDO_H_

#include "../DoInclude.h"

/**
 * 库存快照/批次明细实体
 */
class DocInventoryItemSnapshotDO : public BaseDO
{
	// 主键
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户标识
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// 批次单号（ABU+yyyyMMdd+2位序号）
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	// 快照时刻
	MYSQL_SYNTHESIZE(string, snapshotTime, SnapshotTime);
	// 源库存项标识（写入时刻 wkf_inventory_item.id）
	MYSQL_SYNTHESIZE(string, inventoryItemId, InventoryItemId);
	// 物品定义表名
	MYSQL_SYNTHESIZE(string, itemTable, ItemTable);
	// 物品定义标识
	MYSQL_SYNTHESIZE(string, itemId, ItemId);
	// 收费项定义标识
	MYSQL_SYNTHESIZE(string, chargeItemDefinitionId, ChargeItemDefinitionId);
	// 库房标识（不含货位）
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
	// 拼音码（备份关键字/排序）
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	// 五笔码（备份关键字）
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	// 生产批号
	MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
	// 账面数量（拆零最小单位）
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	// 拆零↔包装倍数（1包装=ratio拆零）
	MYSQL_SYNTHESIZE(double, packageUnitRatio, PackageUnitRatio);
	// 包装单位编码
	MYSQL_SYNTHESIZE(string, packageUnitCode, PackageUnitCode);
	// 批号进价（包装整包）
	MYSQL_SYNTHESIZE(double, lotNumberCostAmount, LotNumberCostAmount);
	// 批号售价（包装整包）
	MYSQL_SYNTHESIZE(double, lotNumberPriceAmount, LotNumberPriceAmount);
	// 进价行总额（写入时刻冻结）
	MYSQL_SYNTHESIZE(double, lineCostTotal, LineCostTotal);
	// 售价行总额（写入时刻冻结）
	MYSQL_SYNTHESIZE(double, linePriceTotal, LinePriceTotal);
	// 生产日期
	MYSQL_SYNTHESIZE(string, productionDate, ProductionDate);
	// 效期
	MYSQL_SYNTHESIZE(string, expirationDate, ExpirationDate);
	// 剩余过期天数（写入时刻冻结）
	MYSQL_SYNTHESIZE(int, remainingDays, RemainingDays);
	// 供应状态（PublicationStatus code）1/0
	MYSQL_SYNTHESIZE(string, inventoryStatusEnum, InventoryStatusEnum);
	// 医保等级码
	MYSQL_SYNTHESIZE(int, chrgitmLv, ChrgitmLv);
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
	// 软删标记
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);

public:

	DocInventoryItemSnapshotDO() : BaseDO("doc_inventory_item_snapshot") //表名
	{
		// 主键
		MYSQL_ADD_FIELD_PK("id", "s", id);

		// 基础字段与业务字段（最后一个参数：false 代表必须有值 NOT NULL，true 代表允许为空 NULL）
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, false);
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, false);
		MYSQL_ADD_FIELD_NULLABLE("snapshot_time", "dt", snapshotTime, false);
		MYSQL_ADD_FIELD_NULLABLE("inventory_item_id", "s", inventoryItemId, false);
		MYSQL_ADD_FIELD_NULLABLE("item_table", "s", itemTable, false);
		MYSQL_ADD_FIELD_NULLABLE("item_id", "s", itemId, false);
		MYSQL_ADD_FIELD_NULLABLE("charge_item_definition_id", "s", chargeItemDefinitionId, true);
		MYSQL_ADD_FIELD_NULLABLE("warehouse_id", "s", warehouseId, false);
		MYSQL_ADD_FIELD_NULLABLE("supplier_id", "s", supplierId, false);
		MYSQL_ADD_FIELD_NULLABLE("product_bus_no", "s", productBusNo, true);
		MYSQL_ADD_FIELD_NULLABLE("name", "s", name, false);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_name", "s", merchandiseName, true);
		MYSQL_ADD_FIELD_NULLABLE("total_volume", "s", totalVolume, true);
		MYSQL_ADD_FIELD_NULLABLE("manufacturer", "s", manufacturer, true);
		MYSQL_ADD_FIELD_NULLABLE("approval_number", "s", approvalNumber, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", ybNo, true);
		MYSQL_ADD_FIELD_NULLABLE("trace_no", "s", traceNo, true);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("lot_number", "s", lotNumber, false);

		// 金额、数量等
		MYSQL_ADD_FIELD_NULLABLE("quantity", "d", quantity, false);
		MYSQL_ADD_FIELD_NULLABLE("package_unit_ratio", "d", packageUnitRatio, true);
		MYSQL_ADD_FIELD_NULLABLE("package_unit_code", "s", packageUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("lot_number_cost_amount", "d", lotNumberCostAmount, true);
		MYSQL_ADD_FIELD_NULLABLE("lot_number_price_amount", "d", lotNumberPriceAmount, true);
		MYSQL_ADD_FIELD_NULLABLE("line_cost_total", "d", lineCostTotal, true);
		MYSQL_ADD_FIELD_NULLABLE("line_price_total", "d", linePriceTotal, true);

		MYSQL_ADD_FIELD_NULLABLE("production_date", "dt", productionDate, true);
		MYSQL_ADD_FIELD_NULLABLE("expiration_date", "dt", expirationDate, true);
		MYSQL_ADD_FIELD_NULLABLE("remaining_days", "i", remainingDays, true);
		MYSQL_ADD_FIELD_NULLABLE("inventory_status_enum", "s", inventoryStatusEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("chrgitm_lv", "i", chrgitmLv, true);
		MYSQL_ADD_FIELD_NULLABLE("medication_category_code", "s", medicationCategoryCode, true);
		MYSQL_ADD_FIELD_NULLABLE("device_category_code", "s", deviceCategoryCode, true);
		MYSQL_ADD_FIELD_NULLABLE("dose_form_code", "s", doseFormCode, true);

		// 公共审计字段
		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
	}
};

// 定义一个智能指针
typedef std::shared_ptr<DocInventoryItemSnapshotDO> PtrDocInventorySnapshotDO;

#endif // !_INVENTORYITEMDO_H_