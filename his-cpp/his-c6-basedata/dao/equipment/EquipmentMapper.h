#pragma once

#ifndef _EQUIPMENT_MAPPER_H_
#define _EQUIPMENT_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/equipment/AdmDeviceDefinitionDO.h"
#include "../../domain/do/equipment/AdmChargeItemDefinitionDO.h" 


inline std::string safeStr(const char* str) {
	return str ? std::string(str) : std::string("");
}
inline std::string safeStr(const std::string& str) {
	return str; // 如果底层驱动已经做了安全处理返回了string，则直接放行
}

// ==========================================================
// 详情查询 Mapper 按 ID 单条查询
// ==========================================================
class PtrEquipmentMapper : public Mapper<std::shared_ptr<EquipmentVeiwDO>>
{
public:
	std::shared_ptr<EquipmentVeiwDO> mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<EquipmentVeiwDO>();

		// ================== 1. 基础系统字段 ==================
		data->setId(safeStr(resultSet->getString(1)));
		data->setTenantId(safeStr(resultSet->getString(2)));
		data->setCreatedAt(safeStr(resultSet->getString(3)));
		data->setUpdatedAt(safeStr(resultSet->getString(4)));
		data->setCreatedBy(safeStr(resultSet->getString(5)));
		data->setUpdatedBy(safeStr(resultSet->getString(6)));
		data->setIsDeleted(resultSet->getBoolean(7)); // 布尔值不崩溃，不包
		data->setDeletedAt(safeStr(resultSet->getString(8)));
		data->setDeletedBy(safeStr(resultSet->getString(9)));
		data->setVersion(resultSet->getInt(10));      // 数字不崩溃，不包

		// ================== 2. 核心业务及分类字段 ==================
		data->setName(safeStr(resultSet->getString(11)));
		data->setBusNo(safeStr(resultSet->getString(12)));
		data->setPyStr(safeStr(resultSet->getString(13)));
		data->setWbStr(safeStr(resultSet->getString(14)));
		data->setCategoryCode(safeStr(resultSet->getString(15)));
		data->setTypeCode(safeStr(resultSet->getString(16)));
		data->setMerchandiseName(safeStr(resultSet->getString(17)));
		data->setMerchandisePyStr(safeStr(resultSet->getString(18)));
		data->setMerchandiseWbStr(safeStr(resultSet->getString(19)));

		// ================== 3. 单位与规格字段 ==================
		data->setPurchaseUnitCode(safeStr(resultSet->getString(20)));
		data->setSplitUnitCode(safeStr(resultSet->getString(21)));
		data->setDoseUnitCode(safeStr(resultSet->getString(22)));
		data->setPackageUnitCode(safeStr(resultSet->getString(23)));
		data->setStockUnitCode(safeStr(resultSet->getString(24)));
		data->setTotalVolume(safeStr(resultSet->getString(25)));
		data->setItemMinQuantity(resultSet->getDouble(26));
		data->setItemMaxQuantity(resultSet->getDouble(27));

		// ================== 4. 换算比例字段 ==================
		data->setDoseUnitRatio(resultSet->getDouble(28));
		data->setPurchaseUnitRatio(resultSet->getDouble(29));
		data->setStockUnitRatio(resultSet->getDouble(30));
		data->setPackageUnitRatio(resultSet->getDouble(31));

		// ================== 5. 厂家与供应商字段 ==================
		data->setApprovalNumber(safeStr(resultSet->getString(32)));
		data->setManufacturerId(safeStr(resultSet->getString(33)));
		data->setManufacturerText(safeStr(resultSet->getString(34)));
		data->setSupplyId(safeStr(resultSet->getString(35)));
		data->setModelNumber(safeStr(resultSet->getString(36)));

		// ================== 6. 医疗与状态标记字段 ==================
		data->setDefEncounterUnitCode(safeStr(resultSet->getString(37)));
		data->setDefInhospitalUnitCode(safeStr(resultSet->getString(38)));
		data->setOrgId(safeStr(resultSet->getString(39)));
		data->setLocationId(safeStr(resultSet->getString(40)));
		data->setHvcmFlag(resultSet->getBoolean(41));
		data->setAllergenFlag(resultSet->getBoolean(42));
		data->setRxFlag(resultSet->getBoolean(43));
		data->setStatusEnum(safeStr(resultSet->getString(44)));
		data->setActiveFlag(resultSet->getBoolean(45));

		// ================== 7. 文本描述字段 ==================
		data->setDescription(safeStr(resultSet->getString(46)));
		data->setJurisdiction(safeStr(resultSet->getString(47)));
		data->setDeviceVersion(safeStr(resultSet->getString(48)));
		data->setSubstanceText(safeStr(resultSet->getString(49)));

		// ================== 8. 医保相关字段 ==================
		data->setYbFlag(resultSet->getBoolean(50));
		data->setYbNo(safeStr(resultSet->getString(51)));
		data->setYbOrgNo(safeStr(resultSet->getString(52)));
		data->setYbMatchFlag(resultSet->getBoolean(53));
		data->setChrgitmLv(resultSet->getInt(54));
		data->setYbMappingCode(safeStr(resultSet->getString(55)));

		data->setPurchasePrice(resultSet->getDouble(56));
		data->setSalePrice(resultSet->getDouble(57));
		data->setLimitPrice(resultSet->getDouble(58));
		data->setYbType(safeStr(resultSet->getString(59)));

		return data;
	}
};

// ==========================================================
// 列表查询 Mapper
// ==========================================================
class EquipmentMapper : public Mapper<EquipmentVeiwDO>
{
public:
	EquipmentVeiwDO mapper(ResultSet* resultSet) const override
	{
		EquipmentVeiwDO data;

		// ================== 1. 基础系统字段 ==================
		data.setId(safeStr(resultSet->getString(1)));
		data.setTenantId(safeStr(resultSet->getString(2)));
		data.setCreatedAt(safeStr(resultSet->getString(3)));
		data.setUpdatedAt(safeStr(resultSet->getString(4)));
		data.setCreatedBy(safeStr(resultSet->getString(5)));
		data.setUpdatedBy(safeStr(resultSet->getString(6)));
		data.setIsDeleted(resultSet->getBoolean(7));
		data.setDeletedAt(safeStr(resultSet->getString(8)));
		data.setDeletedBy(safeStr(resultSet->getString(9)));
		data.setVersion(resultSet->getInt(10));

		// ================== 2. 核心业务及分类字段 ==================
		data.setName(safeStr(resultSet->getString(11)));
		data.setBusNo(safeStr(resultSet->getString(12)));
		data.setPyStr(safeStr(resultSet->getString(13)));
		data.setWbStr(safeStr(resultSet->getString(14)));
		data.setCategoryCode(safeStr(resultSet->getString(15)));
		data.setTypeCode(safeStr(resultSet->getString(16)));
		data.setMerchandiseName(safeStr(resultSet->getString(17)));
		data.setMerchandisePyStr(safeStr(resultSet->getString(18)));
		data.setMerchandiseWbStr(safeStr(resultSet->getString(19)));

		// ================== 3. 单位与规格字段 ==================
		data.setPurchaseUnitCode(safeStr(resultSet->getString(20)));
		data.setSplitUnitCode(safeStr(resultSet->getString(21)));
		data.setDoseUnitCode(safeStr(resultSet->getString(22)));
		data.setPackageUnitCode(safeStr(resultSet->getString(23)));
		data.setStockUnitCode(safeStr(resultSet->getString(24)));
		data.setTotalVolume(safeStr(resultSet->getString(25)));
		data.setItemMinQuantity(resultSet->getDouble(26));
		data.setItemMaxQuantity(resultSet->getDouble(27));

		// ================== 4. 换算比例字段 ==================
		data.setDoseUnitRatio(resultSet->getDouble(28));
		data.setPurchaseUnitRatio(resultSet->getDouble(29));
		data.setStockUnitRatio(resultSet->getDouble(30));
		data.setPackageUnitRatio(resultSet->getDouble(31));

		// ================== 5. 厂家与供应商字段 ==================
		data.setApprovalNumber(safeStr(resultSet->getString(32)));
		data.setManufacturerId(safeStr(resultSet->getString(33)));
		data.setManufacturerText(safeStr(resultSet->getString(34)));
		data.setSupplyId(safeStr(resultSet->getString(35)));
		data.setModelNumber(safeStr(resultSet->getString(36)));

		// ================== 6. 医疗与状态标记字段 ==================
		data.setDefEncounterUnitCode(safeStr(resultSet->getString(37)));
		data.setDefInhospitalUnitCode(safeStr(resultSet->getString(38)));
		data.setOrgId(safeStr(resultSet->getString(39)));
		data.setLocationId(safeStr(resultSet->getString(40)));
		data.setHvcmFlag(resultSet->getBoolean(41));
		data.setAllergenFlag(resultSet->getBoolean(42));
		data.setRxFlag(resultSet->getBoolean(43));
		data.setStatusEnum(safeStr(resultSet->getString(44)));
		data.setActiveFlag(resultSet->getBoolean(45));

		// ================== 7. 文本描述字段 ==================
		data.setDescription(safeStr(resultSet->getString(46)));
		data.setJurisdiction(safeStr(resultSet->getString(47)));
		data.setDeviceVersion(safeStr(resultSet->getString(48)));
		data.setSubstanceText(safeStr(resultSet->getString(49)));

		// ================== 8. 医保相关字段 ==================
		data.setYbFlag(resultSet->getBoolean(50));
		data.setYbNo(safeStr(resultSet->getString(51)));
		data.setYbOrgNo(safeStr(resultSet->getString(52)));
		data.setYbMatchFlag(resultSet->getBoolean(53));
		data.setChrgitmLv(resultSet->getInt(54));
		data.setYbMappingCode(safeStr(resultSet->getString(55)));

		data.setPurchasePrice(resultSet->getDouble(56));
		data.setSalePrice(resultSet->getDouble(57));
		data.setLimitPrice(resultSet->getDouble(58));
		data.setYbType(safeStr(resultSet->getString(59)));

		return data;
	}
};

#endif // !_EQUIPMENT_MAPPER_H_