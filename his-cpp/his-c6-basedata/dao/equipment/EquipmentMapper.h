#pragma once

#ifndef _RENTCONTROLLER_MAPPER_
#define _RENTCONTROLLER_MAPPER_

#include "Mapper.h"
#include "../../domain/do/equipment/InventorySnapshotDO.h"
#include "../../domain/do/equipment/InventoryLockDO.h"
#include "../../domain/do/equipment/InventoryItemDO.h"


// 器材ID
MYSQL_SYNTHESIZE(string, id, Id);
// 住户ID
MYSQL_SYNTHESIZE(string, tenantId, TenantId);
// 创建时间
MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
// 最后更新时间
MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
// 创建人
MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
// 最后更新人
MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
// 软删标识
MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
// 删除时间
MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
// 删除人
MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
// 乐观锁版本号
MYSQL_SYNTHESIZE(int, version, Version);
// 库存锁定记录
MYSQL_SYNTHESIZE(string, inventoryId, InventoryId);
// 锁定标识，ture已锁
MYSQL_SYNTHESIZE(bool, lockFlag, lockFlag);
// 锁定状态
MYSQL_SYNTHESIZE(string, status, Status);
// 锁定单号
MYSQL_SYNTHESIZE(string, busNo, BusNo);
// 同一业务单内顺序号
MYSQL_SYNTHESIZE(int, orderFlag, orderFlag);
/**
 * 示例表字段匹配映射
 */
class EquipmentMapper : public Mapper<InventoryLockDO>
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
		data.setUpdatedAt(resultSet->getString(8));
		data.setUpdatedAt(resultSet->getString(9));
		data.setUpdatedAt(resultSet->getString(10));
		data.setUpdatedAt(resultSet->getString(11));
		data.setUpdatedAt(resultSet->getString(12));
		data.setUpdatedAt(resultSet->getString(13));
		data.setUpdatedAt(resultSet->getString(14));
		data.setUpdatedAt(resultSet->getString(15));
		data.setUpdatedAt(resultSet->getString(16));
		data.setUpdatedAt(resultSet->getString(17));
		data.setUpdatedAt(resultSet->getString(18));
		data.setUpdatedAt(resultSet->getString(19));
		data.setUpdatedAt(resultSet->getString(20));
		data.setUpdatedAt(resultSet->getString(21));
		data.setUpdatedAt(resultSet->getString(22));
		data.setUpdatedAt(resultSet->getString(23));
		data.setUpdatedAt(resultSet->getString(24));
		return data;
	}
};

/**
 * 示例表字段匹配映射-创建智能指针对象
 */
//class PtrSampleMapper : public Mapper<PtrSampleDO>
//{
//public:
//	PtrSampleDO mapper(ResultSet* resultSet) const override
//	{
//		auto data = std::make_shared<SampleDO>();
//		data->setId(resultSet->getString(1));
//		data->setName(resultSet->getString(2));
//		data->setSex(resultSet->getString(3));
//		data->setAge(resultSet->getInt(4));
//		return data;
//	}
//};

#endif // !_RENTCONTROLLER_MAPPER_