#pragma once
#ifndef _SERVICE_REQUEST_DAO_
#define _SERVICE_REQUEST_DAO_

#include "BaseDAO.h"
#include "../../domain/do/medicalorder/ServiceRequestDO.h"

/**
 * 服务医嘱数据访问对象
 * 操作wkf_service_request表（服务医嘱表）
 */
class ServiceRequestDAO : public BaseDAO
{
public:
	// 按主键查询单条服务医嘱，未找到返回null
	PtrServiceRequestDO selectById(std::string id);
};

#endif // !_SERVICE_REQUEST_DAO_