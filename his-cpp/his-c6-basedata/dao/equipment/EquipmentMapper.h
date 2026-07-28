#pragma once

#ifndef _RENTCONTROLLER_MAPPER_
#define _RENTCONTROLLER_MAPPER_

#include "Mapper.h"
#include "../../domain/do/equipment/DocInventoryItemSnapshotDO.h"
#include "../../domain/do/equipment/InventoryLockDO.h"
#include "../../domain/do/equipment/WkfInventoryItemDO.h"


/**
 * 示例表字段匹配映射InventoryLock
 */
class InventoryLockMapper : public Mapper<InventoryLockDO>
{
public:
	InventoryLockDO mapper(ResultSet* resultSet) const override
	{
		InventoryLockDO data;
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
		data.setInventoryId(resultSet->getString(11));
		data.setlockFlag(resultSet->getInt(12));
		data.setStatus(resultSet->getString(13));
		data.setBusNo(resultSet->getString(14));
		data.setorderFlag(resultSet->getInt(15));
		
		return data;
	}
};

/**
 * 示例表字段匹配映射-创建智能指针对象InventoryLock
 */
class PtrInventoryLockMapper : public Mapper<PtrInventoryLockDO>
{
public:
	PtrInventoryLockDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<InventoryLockDO>();
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
		data->setInventoryId(resultSet->getString(11));
		data->setlockFlag(resultSet->getInt(12));
		data->setStatus(resultSet->getString(13));
		data->setBusNo(resultSet->getString(14));
		data->setorderFlag(resultSet->getInt(15));

		return data;
	}
};

/**
 * 库存快照表 普通值对象映射
 */
class DocInventoryItemSnapshotMapper : public Mapper<DocInventoryItemSnapshotDO>
{
public:
	DocInventoryItemSnapshotDO mapper(ResultSet* resultSet) const override
	{
		DocInventoryItemSnapshotDO data;
		// 1.主键id
		data.setId(resultSet->getString(1));
		// 业务字段依次按添加顺序赋值
		data.setTenantId(resultSet->getString(2));
		data.setBusNo(resultSet->getString(3));
		data.setSnapshotTime(resultSet->getString(4));
		data.setInventoryItemId(resultSet->getString(5));
		data.setItemTable(resultSet->getString(6));
		data.setItemId(resultSet->getString(7));
		data.setChargeItemDefinitionId(resultSet->getString(8));
		data.setWarehouseId(resultSet->getString(9));
		data.setSupplierId(resultSet->getString(10));
		data.setProductBusNo(resultSet->getString(11));
		data.setName(resultSet->getString(12));
		data.setMerchandiseName(resultSet->getString(13));
		data.setTotalVolume(resultSet->getString(14));
		data.setManufacturer(resultSet->getString(15));
		data.setApprovalNumber(resultSet->getString(16));
		data.setYbNo(resultSet->getString(17));
		data.setTraceNo(resultSet->getString(18));
		data.setPyStr(resultSet->getString(19));
		data.setWbStr(resultSet->getString(20));
		data.setLotNumber(resultSet->getString(21));

		data.setQuantity(resultSet->getDouble(22));
		data.setPackageUnitRatio(resultSet->getDouble(23));
		data.setPackageUnitCode(resultSet->getString(24));
		data.setLotNumberCostAmount(resultSet->getDouble(25));
		data.setLotNumberPriceAmount(resultSet->getDouble(26));
		data.setLineCostTotal(resultSet->getDouble(27));
		data.setLinePriceTotal(resultSet->getDouble(28));

		data.setProductionDate(resultSet->getString(29));
		data.setExpirationDate(resultSet->getString(30));
		data.setRemainingDays(resultSet->getInt(31));
		data.setInventoryStatusEnum(resultSet->getString(32));
		data.setChrgitmLv(resultSet->getInt(33));
		data.setMedicationCategoryCode(resultSet->getString(34));
		data.setDeviceCategoryCode(resultSet->getString(35));
		data.setDoseFormCode(resultSet->getString(36));

		// 审计公共字段
		data.setCreatedAt(resultSet->getString(37));
		data.setUpdatedAt(resultSet->getString(38));
		data.setCreatedBy(resultSet->getString(39));
		data.setUpdatedBy(resultSet->getString(40));
		data.setIsDeleted(resultSet->getBoolean(41));
		data.setDeletedAt(resultSet->getString(42));
		data.setDeletedBy(resultSet->getString(43));

		return data;
	}
};

/**
 * 库存快照表 智能指针共享对象映射（分页列表查询专用）
 */
class PtrDocInventorySnapshotMapper : public Mapper<PtrDocInventorySnapshotDO>
{
public:
	PtrDocInventorySnapshotDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<DocInventoryItemSnapshotDO>();
		// 顺序和上面完全一致，使用 -> 箭头访问成员
		data->setId(resultSet->getString(1));
		data->setTenantId(resultSet->getString(2));
		data->setBusNo(resultSet->getString(3));
		data->setSnapshotTime(resultSet->getString(4));
		data->setInventoryItemId(resultSet->getString(5));
		data->setItemTable(resultSet->getString(6));
		data->setItemId(resultSet->getString(7));
		data->setChargeItemDefinitionId(resultSet->getString(8));
		data->setWarehouseId(resultSet->getString(9));
		data->setSupplierId(resultSet->getString(10));
		data->setProductBusNo(resultSet->getString(11));
		data->setName(resultSet->getString(12));
		data->setMerchandiseName(resultSet->getString(13));
		data->setTotalVolume(resultSet->getString(14));
		data->setManufacturer(resultSet->getString(15));
		data->setApprovalNumber(resultSet->getString(16));
		data->setYbNo(resultSet->getString(17));
		data->setTraceNo(resultSet->getString(18));
		data->setPyStr(resultSet->getString(19));
		data->setWbStr(resultSet->getString(20));
		data->setLotNumber(resultSet->getString(21));

		data->setQuantity(resultSet->getDouble(22));
		data->setPackageUnitRatio(resultSet->getDouble(23));
		data->setPackageUnitCode(resultSet->getString(24));
		data->setLotNumberCostAmount(resultSet->getDouble(25));
		data->setLotNumberPriceAmount(resultSet->getDouble(26));
		data->setLineCostTotal(resultSet->getDouble(27));
		data->setLinePriceTotal(resultSet->getDouble(28));

		data->setProductionDate(resultSet->getString(29));
		data->setExpirationDate(resultSet->getString(30));
		data->setRemainingDays(resultSet->getInt(31));
		data->setInventoryStatusEnum(resultSet->getString(32));
		data->setChrgitmLv(resultSet->getInt(33));
		data->setMedicationCategoryCode(resultSet->getString(34));
		data->setDeviceCategoryCode(resultSet->getString(35));
		data->setDoseFormCode(resultSet->getString(36));

		data->setCreatedAt(resultSet->getString(37));
		data->setUpdatedAt(resultSet->getString(38));
		data->setCreatedBy(resultSet->getString(39));
		data->setUpdatedBy(resultSet->getString(40));
		data->setIsDeleted(resultSet->getBoolean(41));
		data->setDeletedAt(resultSet->getString(42));
		data->setDeletedBy(resultSet->getString(43));

		return data;
	}
};


/**
 * 库存物品主表 普通值对象映射
 */
class WkfInventoryItemMapper : public Mapper<WkfInventoryItemDO>
{
public:
	WkfInventoryItemDO mapper(ResultSet* resultSet) const override
	{
		WkfInventoryItemDO data;
		// 1 主键id
		data.setId(resultSet->getString(1));
		// 基础审计、乐观锁字段
		data.setTenantId(resultSet->getString(2));
		data.setCreatedAt(resultSet->getString(3));
		data.setUpdatedAt(resultSet->getString(4));
		data.setCreatedBy(resultSet->getString(5));
		data.setUpdatedBy(resultSet->getString(6));
		data.setIsDeleted(resultSet->getBoolean(7));
		data.setDeletedAt(resultSet->getString(8));
		data.setDeletedBy(resultSet->getString(9));
		data.setVersion(resultSet->getInt(10));

		// 业务库存字段依次赋值
		data.setCategoryCode(resultSet->getString(11));
		data.setItemTable(resultSet->getString(12));
		data.setItemId(resultSet->getString(13));
		data.setName(resultSet->getString(14));
		data.setPyStr(resultSet->getString(15));
		data.setWbStr(resultSet->getString(16));
		data.setInventoryStatusEnum(resultSet->getString(17));
		data.setSupplierId(resultSet->getString(18));
		data.setDescriptionText(resultSet->getString(19));
		data.setUnitCode(resultSet->getString(20));
		data.setQuantity(resultSet->getDouble(21));
		data.setCharacteristicJson(resultSet->getString(22));
		data.setLotNumber(resultSet->getString(23));
		data.setProductionDate(resultSet->getString(24));
		data.setExpirationDate(resultSet->getString(25));
		data.setValidityMon(resultSet->getInt(26));
		data.setLocationStoreId(resultSet->getString(27));
		data.setLocationId(resultSet->getString(28));
		data.setTraceNo(resultSet->getString(29));
		data.setPackagingLevels(resultSet->getInt(30));

		return data;
	}
};

/**
 * 库存物品主表 智能指针映射（分页列表查询专用）
 */
class PtrWkfInventoryItemMapper : public Mapper<PtrWkfInventoryItemDO>
{
public:
	PtrWkfInventoryItemDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<WkfInventoryItemDO>();
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

		data->setCategoryCode(resultSet->getString(11));
		data->setItemTable(resultSet->getString(12));
		data->setItemId(resultSet->getString(13));
		data->setName(resultSet->getString(14));
		data->setPyStr(resultSet->getString(15));
		data->setWbStr(resultSet->getString(16));
		data->setInventoryStatusEnum(resultSet->getString(17));
		data->setSupplierId(resultSet->getString(18));
		data->setDescriptionText(resultSet->getString(19));
		data->setUnitCode(resultSet->getString(20));
		data->setQuantity(resultSet->getDouble(21));
		data->setCharacteristicJson(resultSet->getString(22));
		data->setLotNumber(resultSet->getString(23));
		data->setProductionDate(resultSet->getString(24));
		data->setExpirationDate(resultSet->getString(25));
		data->setValidityMon(resultSet->getInt(26));
		data->setLocationStoreId(resultSet->getString(27));
		data->setLocationId(resultSet->getString(28));
		data->setTraceNo(resultSet->getString(29));
		data->setPackagingLevels(resultSet->getInt(30));

		return data;
	}
};
#endif // !_RENTCONTROLLER_MAPPER_