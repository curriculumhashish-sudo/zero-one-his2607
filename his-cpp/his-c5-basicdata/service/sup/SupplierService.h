#pragma once
#ifndef _SUPPLIER_SERVICE_H_
#define _SUPPLIER_SERVICE_H_

#include "ResultStatus.h"
#include "domain/dto/sup/SupplierDTO.h"
#include "domain/dto/sup/SupplierAddDTO.h"
#include "domain/dto/sup/SupplierUpdateDTO.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/GlobalInclude.h"

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

// Business logic for supplier operations.
class SupplierService
{
public:
    // ============================================================
    // Batch operations (c5)
    // ============================================================

    // Batch enable/disable suppliers
    StringJsonVO::Wrapper batchEnableDisable(const SupplierBatchEnableDisableDTO::Wrapper& dto);

    // Batch soft-delete suppliers; returns deleted ID list
    ListJsonVO<String>::Wrapper batchRemove(const oatpp::List<oatpp::String>& ids);

    // ============================================================
    // Add/Update operations (c5-supplier-add-update)
    // ============================================================

    // Add a new supplier with transaction, tenant isolation and name uniqueness check
    SupplierOperationResult add(const SupplierAddDTO::Wrapper& dto);

    // Modify an existing supplier with optimistic locking
    SupplierOperationResult modify(const SupplierUpdateDTO::Wrapper& dto);
};

#endif // !_SUPPLIER_SERVICE_H_
