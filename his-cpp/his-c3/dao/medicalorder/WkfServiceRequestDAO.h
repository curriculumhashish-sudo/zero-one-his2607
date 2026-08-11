#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/23 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _WKFSERVICEREQUESTDAO_H_
#define _WKFSERVICEREQUESTDAO_H_

#include "BaseDAO.h"
#include "domain/do/medicalorder/WkfServiceRequestDO.h"

/**
 * 诊疗服务请求 DAO
 */
class WkfServiceRequestDAO : public BaseDAO
{
public:
    /**
     * 批量停嘱诊疗
     * @param orderIds 诊疗请求ID列表
     * @param stopReason 停嘱原因
     * @param operatorId 操作人ID
     * @return 受影响的行数
     */
    int stopOrderBatch(const std::list<std::string>& orderIds, int stopReason, const std::string& operatorId);

    PtrWkfServiceRequestDO selectById(const std::string& id);
};

#endif