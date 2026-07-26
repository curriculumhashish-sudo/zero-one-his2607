#pragma once

#ifndef _RENTCONTROLLER_MAPPER_
#define _RENTCONTROLLER_MAPPER_

#include "Mapper.h"
#include "../../domain/do/equipment/InventorySnapshotDO.h"
#include "../../domain/do/equipment/InventoryLockDO.h"
#include "../../domain/do/equipment/InventoryItemDO.h"
/**
 * 示例表字段匹配映射
 */
//class EquipmentMapper : public Mapper<EquipmentDO>
//{
//public:
//	SampleDO mapper(ResultSet* resultSet) const override
//	{
//		SampleDO data;
//		data.setId(resultSet->getString(1));
//		data.setName(resultSet->getString(2));
//		data.setSex(resultSet->getString(3));
//		data.setAge(resultSet->getInt(4));
//		return data;
//	}
//};

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