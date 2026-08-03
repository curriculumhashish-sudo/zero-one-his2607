#pragma once
#ifndef _SUPPLIER_MAPPER_H_
#define _SUPPLIER_MAPPER_H_

#include "Mapper.h"
#include "domain/do/sup/SupplierDO.h"

// Maps a cus_supplier result-set row into a SupplierDO object.
class SupplierMapper : public Mapper<PtrSupplierDO>
{
public:
    PtrSupplierDO mapper(ResultSet* resultSet) const override;
};

// Maps a single string column from a result-set row (e.g. tenant_id lookups).
class SupplierStringMapper : public Mapper<std::string>
{
public:
    explicit SupplierStringMapper(const std::string& column) : column(column) {}
    std::string mapper(ResultSet* resultSet) const override;

private:
    std::string column;
};

#endif // !_SUPPLIER_MAPPER_H_
