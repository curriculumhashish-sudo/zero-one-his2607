#pragma once

#ifndef _chinapres3MedicationDefinitionDO_H_
#define _chinapres3MedicationDefinitionDO_H_

#include "../DoInclude.h"

/**
 * 药品定义DO
 *
 * 对应数据库表：
 * med_medication_definition
 *
 * 作用：
 * 保存药品基础信息
 *
 * 在中医处方签发中：
 * 主要用于查询可添加的中药材信息
 */
class chinapres3MedicationDefinitionDO : public BaseDO
{
	// 药品ID
	MYSQL_SYNTHESIZE(string, id, Id);

	// 租户ID
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);

	// 药品名称
	MYSQL_SYNTHESIZE(string, name, Name);

	// 药品分类
	MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);

	// 药材成分
	MYSQL_SYNTHESIZE(string, ingredient, Ingredient);

	// 是否启用
	MYSQL_SYNTHESIZE(bool, activeFlag, ActiveFlag);


	// 剂量单位
	MYSQL_SYNTHESIZE(string, doseUnitCode, DoseUnitCode);

	// 拆零单位
	MYSQL_SYNTHESIZE(string, splitUnitCode, SplitUnitCode);


	// 剂量单位换算比例
	MYSQL_SYNTHESIZE(double, doseUnitRatio, DoseUnitRatio);

	// 采购单位换算比例
	MYSQL_SYNTHESIZE(double, purchaseUnitRatio, PurchaseUnitRatio);

	// 库存单位换算比例
	MYSQL_SYNTHESIZE(double, stockUnitRatio, StockUnitRatio);

	// 包装单位换算比例
	MYSQL_SYNTHESIZE(double, packageUnitRatio, PackageUnitRatio);
public:
	/**
	 * 构造函数
	 *
	 * 对应数据库：
	 * med_medication_definition
	 */
	chinapres3MedicationDefinitionDO()
		: BaseDO("med_medication_definition")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD_NULLABLE("category_code", "s", categoryCode, true);
		MYSQL_ADD_FIELD_NULLABLE("ingredient", "s", ingredient, true);
		MYSQL_ADD_FIELD_NULLABLE("active_flag", "b", activeFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("dose_unit_code", "s", doseUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("split_unit_code", "s", splitUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("dose_unit_ratio", "d", doseUnitRatio, true);
		MYSQL_ADD_FIELD_NULLABLE("purchase_unit_ratio", "d", purchaseUnitRatio, true);
		MYSQL_ADD_FIELD_NULLABLE("stock_unit_ratio", "d", stockUnitRatio, true);
		MYSQL_ADD_FIELD_NULLABLE("package_unit_ratio", "d", packageUnitRatio, true);
	}
};

// 智能指针别名
typedef std::shared_ptr<chinapres3MedicationDefinitionDO> Ptrchinapres3MedicationDefinitionDO;

#endif // !_chinapres3MedicationDefinitionDO_H_