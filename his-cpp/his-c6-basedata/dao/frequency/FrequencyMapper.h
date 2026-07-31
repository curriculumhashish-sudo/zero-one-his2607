#pragma once
#ifndef _FREQUENCY_MAPPER_H_
#define _FREQUENCY_MAPPER_H_

#include"Mapper.h"
#include"../../domain/do/frequency/CusFrequencyDO.h"

class CusFrequencyMapper :public Mapper<CusFrequencyDO>
{
public:
	CusFrequencyDO mapper(ResultSet* resultSet) const override
	{
		CusFrequencyDO data;
		data.setId(resultSet->getString(1));
		data.setTenantId(resultSet->getString(2));
		data.setCreatedAt(resultSet->getString(3));
		data.setUpdatedAt(resultSet->getString(4));
		data.setCreatedBy(resultSet->getString(5));
		data.setUpdatedBy(resultSet->getString(6));
		data.setIsDeleted(resultSet->getInt(7));
		data.setDeletedAt(resultSet->getString(8));
		data.setDeletedBy(resultSet->getString(9));
		data.setVersion(resultSet->getInt(10));
		data.setRateCode(resultSet->getString(11));
		data.setName(resultSet->getString(12));
		data.setCycleType(resultSet->getString(13));
		data.setDayCount(resultSet->getInt(14));
		data.setDayInterval(resultSet->getInt(15));
		data.setDayTimes(resultSet->getString(16));
		data.setWeekCycleFlag(resultSet->getInt(17));
		data.setTotalExecutionCount(resultSet->getInt(18));
		data.setExecutionPeriod(resultSet->getInt(19));
		data.setExecutionPeriodUnit(resultSet->getString(20));
		data.setApplicableScenes(resultSet->getString(21));
		data.setAllowCrossDay(resultSet->getString(22));
		data.setActiveFlag(resultSet->getInt(23));
		data.setSortNo(resultSet->getInt(24));
		return data;
	}
};

class PtrCusFrequencyMapper :public Mapper<PtrCusFrequencyDO>
{
public:
	PtrCusFrequencyDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<CusFrequencyDO>();
		data->setId(resultSet->getString(1));
		data->setTenantId(resultSet->getString(2));
		data->setCreatedAt(resultSet->getString(3));
		data->setUpdatedAt(resultSet->getString(4));
		data->setCreatedBy(resultSet->getString(5));
		data->setUpdatedBy(resultSet->getString(6));
		data->setIsDeleted(resultSet->getInt(7));
		data->setDeletedAt(resultSet->getString(8));
		data->setDeletedBy(resultSet->getString(9));
		data->setVersion(resultSet->getInt(10));
		data->setRateCode(resultSet->getString(11));
		data->setName(resultSet->getString(12));
		data->setCycleType(resultSet->getString(13));
		data->setDayCount(resultSet->getInt(14));
		data->setDayInterval(resultSet->getInt(15));
		data->setDayTimes(resultSet->getString(16));
		data->setWeekCycleFlag(resultSet->getInt(17));
		data->setTotalExecutionCount(resultSet->getInt(18));
		data->setExecutionPeriod(resultSet->getInt(19));
		data->setExecutionPeriodUnit(resultSet->getString(20));
		data->setApplicableScenes(resultSet->getString(21));
		data->setAllowCrossDay(resultSet->getString(22));
		data->setActiveFlag(resultSet->getInt(23));
		data->setSortNo(resultSet->getInt(24));
		return data;
	}
};

#endif
