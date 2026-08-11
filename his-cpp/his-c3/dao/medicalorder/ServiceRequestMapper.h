#pragma once
#ifndef _SERVICE_REQUEST_MAPPER_
#define _SERVICE_REQUEST_MAPPER_

#include "Mapper.h"
#include "../../domain/do/medicalorder/ServiceRequestDO.h"

/**
 * 服务医嘱字段映射器
 * 将ResultSet列映射到ServiceRequestDO字段
 * 列顺序：1=id(主键), 2=status_enum(医嘱状态), 3=therapy_enum(医嘱类型)
 */
class ServiceRequestMapper : public Mapper<ServiceRequestDO>
{
public:
	ServiceRequestDO mapper(ResultSet* resultSet) const override
	{
		ServiceRequestDO data;
		// 第1列：主键ID
		data.setId(resultSet->getString(1));
		// 第2列：医嘱状态（DRAFT=待签发, ACTIVE=生效中, STOPPED=已停嘱, CANCELLED=已撤回）
		data.setStatusEnum(resultSet->getString(2));
		// 第3列：医嘱类型（LONG_TERM=长期医嘱, TEMPORARY=临时医嘱）
		data.setTherapyEnum(resultSet->getString(3));
		return data;
	}
};

/**
 * 服务医嘱智能指针字段映射器
 * 与ServiceRequestMapper功能相同，但返回shared_ptr，供executeQueryOne使用
 */
class PtrServiceRequestMapper : public Mapper<PtrServiceRequestDO>
{
public:
	PtrServiceRequestDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<ServiceRequestDO>();
		data->setId(resultSet->getString(1));
		data->setStatusEnum(resultSet->getString(2));
		data->setTherapyEnum(resultSet->getString(3));
		return data;
	}
};

#endif // !_SERVICE_REQUEST_MAPPER_