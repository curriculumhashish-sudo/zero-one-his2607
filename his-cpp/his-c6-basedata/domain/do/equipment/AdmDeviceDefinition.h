#pragma once

#ifndef _ADM_DEVICE_DEFINITION_DO_H_
#define _ADM_DEVICE_DEFINITION_DO_H_

#include "../DoInclude.h"
#include "DocInventoryItemSnapshotDO.h"
/**
 * 耗材/器材基础定义字典实体
 */
	class AdmDeviceDefinitionDO : public BaseDO
{
	// ================== 1. 基础系统字段 ==================
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
	// 软删标记 (0-正常 1-已删除)
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// 乐观锁版本号
	MYSQL_SYNTHESIZE(int, version, Version);


	// ================== 2. 核心业务字段 ==================
	// 器材/耗材名称
	MYSQL_SYNTHESIZE(string, name, Name);
	// 器材业务编码（院内唯一）
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	// 拼音码（用于检索）
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	// 五笔码（用于检索）
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	// 器材分类编码
	MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
	// 器材种类编码
	MYSQL_SYNTHESIZE(string, typeCode, TypeCode);
	// 采购单位
	MYSQL_SYNTHESIZE(string, purchaseUnitCode, PurchaseUnitCode);
	// 拆零单位
	MYSQL_SYNTHESIZE(string, splitUnitCode, SplitUnitCode);
	// 包装规格描述
	MYSQL_SYNTHESIZE(string, totalVolume, TotalVolume);
	// 最小库存警戒量
	MYSQL_SYNTHESIZE(double, itemMinQuantity, ItemMinQuantity);
	// 最大库存警戒量
	MYSQL_SYNTHESIZE(double, itemMaxQuantity, ItemMaxQuantity);
	// 商品名称
	MYSQL_SYNTHESIZE(string, merchandiseName, MerchandiseName);
	// 商品名拼音检索码
	MYSQL_SYNTHESIZE(string, merchandisePyStr, MerchandisePyStr);
	// 商品名五笔检索码
	MYSQL_SYNTHESIZE(string, merchandiseWbStr, MerchandiseWbStr);
	// 所属科室标识
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	// 产品型号
	MYSQL_SYNTHESIZE(string, modelNumber, ModelNumber);
	// 高值器材标志
	MYSQL_SYNTHESIZE(bool, hvcmFlag, HvcmFlag);
	// 批准文号
	MYSQL_SYNTHESIZE(string, approvalNumber, ApprovalNumber);
	// 生产厂家标识
	MYSQL_SYNTHESIZE(string, manufacturerId, ManufacturerId);
	// 生产厂家名称
	MYSQL_SYNTHESIZE(string, manufacturerText, ManufacturerText);
	// 供应商标识
	MYSQL_SYNTHESIZE(string, supplyId, SupplyId);
	// 状态枚举 (发布状态)
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	// 是否启用标记 (启停用功能的核心)
	MYSQL_SYNTHESIZE(bool, activeFlag, ActiveFlag);


	// ================== 3. 医保与财务转换相关字段 ==================
	// 过敏标记
	MYSQL_SYNTHESIZE(bool, allergenFlag, AllergenFlag);
	// 处方标志
	MYSQL_SYNTHESIZE(bool, rxFlag, RxFlag);
	// 医保标记
	MYSQL_SYNTHESIZE(bool, ybFlag, YbFlag);
	// 医保编码
	MYSQL_SYNTHESIZE(string, ybNo, YbNo);
	// 医保对码标记
	MYSQL_SYNTHESIZE(bool, ybMatchFlag, YbMatchFlag);
	// 医保等级
	MYSQL_SYNTHESIZE(int, chrgitmLv, ChrgitmLv);

	// 剂量单位→拆零换算
	MYSQL_SYNTHESIZE(double, doseUnitRatio, DoseUnitRatio);
	// 采购单位→拆零换算
	MYSQL_SYNTHESIZE(double, purchaseUnitRatio, PurchaseUnitRatio);
	// 库存单位→拆零换算
	MYSQL_SYNTHESIZE(double, stockUnitRatio, StockUnitRatio);
	// 销售单位→拆零换算
	MYSQL_SYNTHESIZE(double, packageUnitRatio, PackageUnitRatio);

	// 批号进价（包装整包）
	MYSQL_SYNTHESIZE(double, lotNumberCostAmount, LotNumberCostAmount);
	// 批号售价（包装整包）
	MYSQL_SYNTHESIZE(double, lotNumberPriceAmount, LotNumberPriceAmount);

public:

	AdmDeviceDefinitionDO() : BaseDO("adm_device_definition") // 绑定表名
	{
		// ================== 映射主键 ==================
		MYSQL_ADD_FIELD_PK("id", "s", id);

		// ================== 映射基础字段 ==================
		// 最后一个参数：false 代表 NOT NULL (数据库中必填)，true 代表允许为空
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);

		// ================== 映射业务字段 ==================
		MYSQL_ADD_FIELD_NULLABLE("name", "s", name, false); // 名称是必填的
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, true);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("category_code", "s", categoryCode, true);
		MYSQL_ADD_FIELD_NULLABLE("type_code", "s", typeCode, true);
		MYSQL_ADD_FIELD_NULLABLE("purchase_unit_code", "s", purchaseUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("split_unit_code", "s", splitUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("total_volume", "s", totalVolume, true);
		MYSQL_ADD_FIELD_NULLABLE("item_min_quantity", "d", itemMinQuantity, true);
		MYSQL_ADD_FIELD_NULLABLE("item_max_quantity", "d", itemMaxQuantity, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_name", "s", merchandiseName, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_py_str", "s", merchandisePyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_wb_str", "s", merchandiseWbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
		MYSQL_ADD_FIELD_NULLABLE("model_number", "s", modelNumber, true);
		MYSQL_ADD_FIELD_NULLABLE("hvcm_flag", "b", hvcmFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("approval_number", "s", approvalNumber, true);
		MYSQL_ADD_FIELD_NULLABLE("manufacturer_id", "s", manufacturerId, true);
		MYSQL_ADD_FIELD_NULLABLE("manufacturer_text", "s", manufacturerText, true);
		MYSQL_ADD_FIELD_NULLABLE("supply_id", "s", supplyId, true);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("active_flag", "b", activeFlag, false); // 启停用标志必填

		// ================== 映射医保与比例字段 ==================
		MYSQL_ADD_FIELD_NULLABLE("allergen_flag", "b", allergenFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("rx_flag", "b", rxFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_flag", "b", ybFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", ybNo, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_match_flag", "b", ybMatchFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("chrgitm_lv", "i", chrgitmLv, true);

		MYSQL_ADD_FIELD_NULLABLE("dose_unit_ratio", "d", doseUnitRatio, true);
		MYSQL_ADD_FIELD_NULLABLE("purchase_unit_ratio", "d", purchaseUnitRatio, true);
		MYSQL_ADD_FIELD_NULLABLE("stock_unit_ratio", "d", stockUnitRatio, true);
		MYSQL_ADD_FIELD_NULLABLE("package_unit_ratio", "d", packageUnitRatio, true);
	}
};

// 智能指针
typedef std::shared_ptr<AdmDeviceDefinitionDO> PtrAdmDeviceDefinitionDO;

#endif // !_ADM_DEVICE_DEFINITION_DO_H_