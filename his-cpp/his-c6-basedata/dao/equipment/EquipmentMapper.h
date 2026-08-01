#pragma once

#ifndef _EQUIPMENT_MAPPER_H_
#define _EQUIPMENT_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/equipment/AdmDeviceDefinitionDO.h"
#include "../../domain/do/equipment/AdmChargeItemDefinitionDO.h" 

// 详情
// 详情查询Mapper 按ID单条查询
class PtrEquipmentMapper : public Mapper<std::shared_ptr<EquipmentVeiwDO>>
{
public:
	std::shared_ptr<EquipmentVeiwDO> mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<EquipmentVeiwDO>();

		// ================== 1. 基础系统字段 ==================
		data->setId(resultSet->getString(1));
		data->setTenantId(resultSet->getString(2));
		data->setCreatedAt(resultSet->getString(3));
		data->setUpdatedAt(resultSet->getString(4));
		data->setCreatedBy(resultSet->getString(5));
		data->setUpdatedBy(resultSet->getString(6));
		data->setIsDeleted(resultSet->getBoolean(7));
		data->setDeletedAt(resultSet->getString(8));
		data->setDeletedBy(resultSet->getString(9));
		data->setVersion(resultSet->getInt(10));

		// ================== 2. 核心业务及分类字段 ==================
		data->setName(resultSet->getString(11));
		data->setBusNo(resultSet->getString(12));
		data->setPyStr(resultSet->getString(13));
		data->setWbStr(resultSet->getString(14));
		data->setCategoryCode(resultSet->getString(15));
		data->setTypeCode(resultSet->getString(16));
		data->setMerchandiseName(resultSet->getString(17));
		data->setMerchandisePyStr(resultSet->getString(18));
		data->setMerchandiseWbStr(resultSet->getString(19));

		// ================== 3. 单位与规格字段 ==================
		data->setPurchaseUnitCode(resultSet->getString(20));
		data->setSplitUnitCode(resultSet->getString(21));
		data->setDoseUnitCode(resultSet->getString(22));
		data->setPackageUnitCode(resultSet->getString(23));
		data->setStockUnitCode(resultSet->getString(24));
		data->setTotalVolume(resultSet->getString(25));
		data->setItemMinQuantity(resultSet->getDouble(26));
		data->setItemMaxQuantity(resultSet->getDouble(27));

		// ================== 4. 换算比例字段 ==================
		data->setDoseUnitRatio(resultSet->getDouble(28));
		data->setPurchaseUnitRatio(resultSet->getDouble(29));
		data->setStockUnitRatio(resultSet->getDouble(30));
		data->setPackageUnitRatio(resultSet->getDouble(31));

		// ================== 5. 厂家与供应商字段 ==================
		data->setApprovalNumber(resultSet->getString(32));
		data->setManufacturerId(resultSet->getString(33));
		data->setManufacturerText(resultSet->getString(34));
		data->setSupplyId(resultSet->getString(35));
		data->setModelNumber(resultSet->getString(36));

		// ================== 6. 医疗与状态标记字段 ==================
		data->setDefEncounterUnitCode(resultSet->getString(37));
		data->setDefInhospitalUnitCode(resultSet->getString(38));
		data->setOrgId(resultSet->getString(39));
		data->setLocationId(resultSet->getString(40));
		data->setHvcmFlag(resultSet->getBoolean(41));
		data->setAllergenFlag(resultSet->getBoolean(42));
		data->setRxFlag(resultSet->getBoolean(43));
		data->setStatusEnum(resultSet->getString(44));
		data->setActiveFlag(resultSet->getBoolean(45));

		// ================== 7. 文本描述字段 ==================
		data->setDescription(resultSet->getString(46));
		data->setJurisdiction(resultSet->getString(47));
		data->setDeviceVersion(resultSet->getString(48));
		data->setSubstanceText(resultSet->getString(49));

		// ================== 8. 医保相关字段 ==================
		data->setYbFlag(resultSet->getBoolean(50));
		data->setYbNo(resultSet->getString(51));
		data->setYbOrgNo(resultSet->getString(52));
		data->setYbMatchFlag(resultSet->getBoolean(53));
		data->setChrgitmLv(resultSet->getInt(54));
		data->setYbMappingCode(resultSet->getString(55));

		data->setPurchasePrice(resultSet->getDouble(56));
		data->setSalePrice(resultSet->getDouble(57));
		data->setLimitPrice(resultSet->getDouble(58));
		data->setYbType(resultSet->getString(59));
		data->setTypeCode(resultSet->getString(60));
		return data;
	}
};

class EquipmentMapper : public Mapper<EquipmentVeiwDO>
{
public:
	EquipmentVeiwDO mapper(ResultSet* resultSet) const override
	{
		EquipmentVeiwDO data;

		// ================== 1. 基础系统字段 ==================
		data.setId(resultSet->getString(1));
		data.setTenantId(resultSet->getString(2));
		data.setCreatedAt(resultSet->getString(3));
		data.setUpdatedAt(resultSet->getString(4));
		data.setCreatedBy(resultSet->getString(5));
		data.setUpdatedBy(resultSet->getString(6));
		data.setIsDeleted(resultSet->getBoolean(7));
		data.setDeletedAt(resultSet->getString(8));
		data.setDeletedBy(resultSet->getString(9));
		data.setVersion(resultSet->getInt(10));

		// ================== 2. 核心业务及分类字段 ==================
		data.setName(resultSet->getString(11));
		data.setBusNo(resultSet->getString(12));
		data.setPyStr(resultSet->getString(13));
		data.setWbStr(resultSet->getString(14));
		data.setCategoryCode(resultSet->getString(15));
		data.setTypeCode(resultSet->getString(16));
		data.setMerchandiseName(resultSet->getString(17));
		data.setMerchandisePyStr(resultSet->getString(18));
		data.setMerchandiseWbStr(resultSet->getString(19));

		// ================== 3. 单位与规格字段 ==================
		data.setPurchaseUnitCode(resultSet->getString(20));
		data.setSplitUnitCode(resultSet->getString(21));
		data.setDoseUnitCode(resultSet->getString(22));
		data.setPackageUnitCode(resultSet->getString(23));
		data.setStockUnitCode(resultSet->getString(24));
		data.setTotalVolume(resultSet->getString(25));
		data.setItemMinQuantity(resultSet->getDouble(26));
		data.setItemMaxQuantity(resultSet->getDouble(27));

		// ================== 4. 换算比例字段 ==================
		data.setDoseUnitRatio(resultSet->getDouble(28));
		data.setPurchaseUnitRatio(resultSet->getDouble(29));
		data.setStockUnitRatio(resultSet->getDouble(30));
		data.setPackageUnitRatio(resultSet->getDouble(31));

		// ================== 5. 厂家与供应商字段 ==================
		data.setApprovalNumber(resultSet->getString(32));
		data.setManufacturerId(resultSet->getString(33));
		data.setManufacturerText(resultSet->getString(34));
		data.setSupplyId(resultSet->getString(35));
		data.setModelNumber(resultSet->getString(36));

		// ================== 6. 医疗与状态标记字段 ==================
		data.setDefEncounterUnitCode(resultSet->getString(37));
		data.setDefInhospitalUnitCode(resultSet->getString(38));
		data.setOrgId(resultSet->getString(39));
		data.setLocationId(resultSet->getString(40));
		data.setHvcmFlag(resultSet->getBoolean(41));
		data.setAllergenFlag(resultSet->getBoolean(42));
		data.setRxFlag(resultSet->getBoolean(43));
		data.setStatusEnum(resultSet->getString(44));
		data.setActiveFlag(resultSet->getBoolean(45));

		// ================== 7. 文本描述字段 ==================
		data.setDescription(resultSet->getString(46));
		data.setJurisdiction(resultSet->getString(47));
		data.setDeviceVersion(resultSet->getString(48));
		data.setSubstanceText(resultSet->getString(49));

		// ================== 8. 医保相关字段 ==================
		data.setYbFlag(resultSet->getBoolean(50));
		data.setYbNo(resultSet->getString(51));
		data.setYbOrgNo(resultSet->getString(52));
		data.setYbMatchFlag(resultSet->getBoolean(53));
		data.setChrgitmLv(resultSet->getInt(54));
		data.setYbMappingCode(resultSet->getString(55));

		data.setPurchasePrice(resultSet->getDouble(56));
		data.setSalePrice(resultSet->getDouble(57));
		data.setLimitPrice(resultSet->getDouble(58));
		data.setYbType(resultSet->getString(59));
		data.setTypeCode(resultSet->getString(60));
		return data;
	}
};

#endif // !_EQUIPMENT_MAPPER_H_