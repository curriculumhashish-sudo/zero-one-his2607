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
#include "domain/do/medicalorder_2/WkfServiceRequestDO.h"

/**
 * Service request DAO (wkf_service_request)
 */
class WkfServiceRequestDAO : public BaseDAO
{
public:
    /**
     * Batch stop service requests
     * @param orderIds List of IDs to stop
     * @param stopReason Stop reason code
     * @param operatorId Operator ID
     * @param encounterId Encounter ID
     * @param patientId Patient ID
     * @return Number of affected rows
     */
    int stopOrderBatch(const std::list<std::string>& orderIds, int stopReason, const std::string& operatorId, const std::string& encounterId, const std::string& patientId);

    PtrWkfServiceRequestDO selectById(const std::string& id);

    // Transaction control
    void beginTransaction() { sqlSession->beginTransaction(); }
    void commitTransaction() { sqlSession->commitTransaction(); }
    void rollbackTransaction() { sqlSession->rollbackTransaction(); }
};

#endif