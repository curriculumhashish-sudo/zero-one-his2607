#pragma once
#ifndef _SUPPLIER_DAO_H_
#define _SUPPLIER_DAO_H_

#include "BaseDAO.h"
#include "domain/do/sup/SupplierDO.h"
#include "domain/dto/sup/SupplierUpdateDTO.h"

// Data-access layer for the cus_supplier table.
class SupplierDAO : public BaseDAO
{
public:
    // Resolves the tenant_id of the current user from the login-session table.
    std::string selectTenantIdByUserId(const std::string& userId);

    // Locks and returns the supplier row by id (excludes soft-deleted rows).
    PtrSupplierDO selectByIdForUpdate(const std::string& id);

    // Checks whether a supplier name already exists within the tenant;
    // excludeId (optional) skips the row being modified.
    bool existsName(const std::string& tenantId, const std::string& name, const std::string& excludeId = "");

    // Inserts a new supplier row; returns the affected-rows count.
    int insertSupplier(const SupplierDO& data);

    // Patch-updates the supplier with optimistic locking (WHERE version = expectedVersion).
    int updatePatch(const SupplierUpdateDTO& dto, const std::string& userId, int expectedVersion);
};

#endif // !_SUPPLIER_DAO_H_
