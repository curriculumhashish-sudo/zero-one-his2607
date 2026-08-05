#pragma once

#ifndef _SERVICEREQUESTDAO_H_
#define _SERVICEREQUESTDAO_H_

#include "BaseDAO.h"
#include "domain/do/prescription2/ServiceRequestDO.h"

class ServiceRequestDAO : public BaseDAO
{
public:
    // Query one service request.
    PtrServiceRequestDO selectById(const std::string& id);
    // Query and lock one service request for a print operation.
    PtrServiceRequestDO selectByIdForUpdate(const std::string& id);
    // Increment the print count with version validation.
    int incrementPrintCount(const ServiceRequestDO& data, int expectedVersion);
};

#endif // !_SERVICEREQUESTDAO_H_
