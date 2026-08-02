#pragma once
#ifndef _SUPPLIER_SERVICE_H_
#define _SUPPLIER_SERVICE_H_

#include "ResultStatus.h"
#include "domain/dto/sup/SupplierAddDTO.h"
#include "domain/dto/sup/SupplierUpdateDTO.h"

// Outcome of a supplier operation, mirroring the Dept module's result pattern.
struct SupplierOperationResult
{
    SupplierOperationResult(bool success, const std::string& id, const ResultStatus& status, int httpStatus)
        : success(success), id(id), status(status), httpStatus(httpStatus)
    {
    }

    bool success;
    std::string id;        // The affected supplier id (empty on failure)
    ResultStatus status;   // Business status code + message
    int httpStatus;        // Reserved; the controller currently always returns HTTP 200
};

// Business logic for supplier add/modify: transactions, tenant isolation,
// name uniqueness, and optimistic-lock handling.
class SupplierService
{
public:
    SupplierOperationResult add(const SupplierAddDTO::Wrapper& dto);
    SupplierOperationResult modify(const SupplierUpdateDTO::Wrapper& dto);
};

#endif // !_SUPPLIER_SERVICE_H_
