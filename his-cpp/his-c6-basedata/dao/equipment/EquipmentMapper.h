#pragma once

#ifndef _RENTCONTROLLER_MAPPER_
#define _RENTCONTROLLER_MAPPER_

#include "Mapper.h"
#include "../../domain/do/equipment/DocInventoryItemSnapshotDO.h"
#include "../../domain/do/equipment/InventoryLockDO.h"
#include "../../domain/do/equipment/WkfInventoryItemDO.h"
#include "../../domain/do/equipment/AdmDeviceDefinition.h"

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
 * 分页
 */
class QueryMapper : public Mapper<DocInventoryItemSnapshotDO>
{
public:
	DocInventoryItemSnapshotDO mapper(ResultSet* resultSet) const override
	{
		DocInventoryItemSnapshotDO data;

		// 主键id
		data.setId(resultSet->getString(1));
		// 项目编码
		data.setProductBusNo(resultSet->getString(2));
		// 项目名
		data.setName(resultSet->getString(3));
		// 规格
		data.setTotalVolume(resultSet->getString(4));
		// 状态
		data.setInventoryStatusEnum(resultSet->getString(5));
		// 耗材分类编码
		data.setDeviceCategoryCode(resultSet->getString(6));
		// 厂家/产地
		data.setManufacturer(resultSet->getString(7));
		// 批号售价（包装整包）
		data.setLotNumberPriceAmount(resultSet->getDouble(8));

		return data;
	}
};

// 详情
// 详情查询Mapper 按ID单条查询
class PtrByIdMapper : public Mapper<std::shared_ptr<AdmDeviceDefinitionDO>>
{
public:
	std::shared_ptr<AdmDeviceDefinitionDO> mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<AdmDeviceDefinitionDO>();

		// ========== 基础主键、租户、创建更新、删除、乐观锁字段 ==========
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

		// ========== 核心业务器材基础信息字段 ==========
		data->setName(resultSet->getString(11));
		data->setBusNo(resultSet->getString(12));
		data->setPyStr(resultSet->getString(13));
		data->setWbStr(resultSet->getString(14));
		data->setCategoryCode(resultSet->getString(15));
		data->setTypeCode(resultSet->getString(16));
		data->setPurchaseUnitCode(resultSet->getString(17));
		data->setSplitUnitCode(resultSet->getString(18));
		data->setTotalVolume(resultSet->getString(19));
		data->setItemMinQuantity(resultSet->getDouble(20));
		data->setItemMaxQuantity(resultSet->getDouble(21));
		data->setMerchandiseName(resultSet->getString(22));
		data->setMerchandisePyStr(resultSet->getString(23));
		data->setMerchandiseWbStr(resultSet->getString(24));
		data->setOrgId(resultSet->getString(25));
		data->setModelNumber(resultSet->getString(26));
		data->setHvcmFlag(resultSet->getBoolean(27));
		data->setApprovalNumber(resultSet->getString(28));
		data->setManufacturerId(resultSet->getString(29));
		data->setManufacturerText(resultSet->getString(30));
		data->setSupplyId(resultSet->getString(31));
		data->setStatusEnum(resultSet->getString(32));
		data->setActiveFlag(resultSet->getBoolean(33));

		// ========== 医保、财务、单位换算、进价售价字段 ==========
		data->setAllergenFlag(resultSet->getBoolean(34));
		data->setRxFlag(resultSet->getBoolean(35));
		data->setYbFlag(resultSet->getBoolean(36));
		data->setYbNo(resultSet->getString(37));
		data->setYbMatchFlag(resultSet->getBoolean(38));
		data->setChrgitmLv(resultSet->getInt(39));

		data->setDoseUnitRatio(resultSet->getDouble(40));
		data->setPurchaseUnitRatio(resultSet->getDouble(41));
		data->setStockUnitRatio(resultSet->getDouble(42));
		data->setPackageUnitRatio(resultSet->getDouble(43));

		data->setLotNumberCostAmount(resultSet->getDouble(44));
		data->setLotNumberPriceAmount(resultSet->getDouble(45));

		return data;
	}
};
#endif // !_RENTCONTROLLER_MAPPER_