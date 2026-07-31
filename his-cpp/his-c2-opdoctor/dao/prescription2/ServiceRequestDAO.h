#pragma once
#ifndef _SERVICEREQUESTDAO_H_
#define _SERVICEREQUESTDAO_H_

#include "BaseDAO.h"
#include "domain/do/prescription2/ServiceRequestDO.h"

// wkf_service_request 记录的数据访问对象。
class ServiceRequestDAO : public BaseDAO
{
public:
    // 查询未删除的服务申请单。
    PtrServiceRequestDO selectById(const std::string& id);
    // 更新打印次数前查询并锁定服务申请单。
    PtrServiceRequestDO selectByIdForUpdate(const std::string& id);
};

#endif // !_SERVICEREQUESTDAO_H_
