#pragma once
#ifndef _SERVICEREQUESTDO_H_
#define _SERVICEREQUESTDO_H_

#include "../DoInclude.h"

// 映射至 wkf_service_request 表的领域对象。
class ServiceRequestDO : public BaseDO
{
    // 服务申请单主标识。
    MYSQL_SYNTHESIZE(std::string, id, Id);
    // 申请单已打印次数。
    MYSQL_SYNTHESIZE(int, printCount, PrintCount);
    // 最后更新申请单的用户标识。
    MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
    // 最近一次申请单更新时间。
    MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);

public:
    ServiceRequestDO() : BaseDO("wkf_service_request")
    {
        // 定义 BaseDAO 更新所使用的对象与列映射。
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("print_count", "i", printCount);
        MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
        MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
    }
};

typedef std::shared_ptr<ServiceRequestDO> PtrServiceRequestDO;

#endif // !_SERVICEREQUESTDO_H_
