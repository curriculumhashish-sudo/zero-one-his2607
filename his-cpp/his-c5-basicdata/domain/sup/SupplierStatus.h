#pragma once
#ifndef _SUPPLIER_STATUS_H_
#define _SUPPLIER_STATUS_H_

#include "ResultStatus.h"

/**
 * Supplier domain result-status factories.
 *
 * Error codes live in the 53xxx range to avoid colliding with other modules
 * (e.g. DeptStatus uses 52xxx).
 */
namespace SupplierStatus
{
    // Request validation failed; the specific message is passed through.
    inline ResultStatus invalid(const std::string& message)
    {
        return ResultStatus(message, 53001);
    }

    // The target supplier does not exist (or has been soft-deleted).
    inline ResultStatus notFound()
    {
        return ResultStatus("supplier does not exist", 53002);
    }

    // The caller is not allowed to operate on this supplier (tenant mismatch).
    inline ResultStatus forbidden()
    {
        return ResultStatus("no permission to operate this supplier", 53003);
    }

    // A supplier with the same name already exists in the tenant.
    inline ResultStatus duplicateName()
    {
        return ResultStatus("supplier name already exists", 53004);
    }

    // Optimistic-lock conflict: the supplied version does not match the current row.
    inline ResultStatus updateConflict()
    {
        return ResultStatus("supplier update conflict, please refresh and retry", 53005);
    }

    // A database operation failed.
    inline ResultStatus databaseFailure()
    {
        return ResultStatus("supplier database operation failed", 53006);
    }
}

#endif // !_SUPPLIER_STATUS_H_
