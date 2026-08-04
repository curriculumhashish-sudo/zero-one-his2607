#include "stdafx.h"
#include "SupplierMapper.h"

namespace
{
    // Reads a nullable column, returning "" when the column is NULL.
    std::string nullableString(ResultSet* resultSet, const std::string& column)
    {
        return resultSet->isNull(column) ? "" : resultSet->getString(column);
    }
}

PtrSupplierDO SupplierMapper::mapper(ResultSet* resultSet) const
{
    auto data = std::make_shared<SupplierDO>();
    data->setId(resultSet->getString("id"));
    data->setTenantId(nullableString(resultSet, "tenant_id"));
    data->setCreatedAt(nullableString(resultSet, "created_at"));
    data->setUpdatedAt(nullableString(resultSet, "updated_at"));
    data->setCreatedBy(nullableString(resultSet, "created_by"));
    data->setUpdatedBy(nullableString(resultSet, "updated_by"));
    data->setIsDeleted(resultSet->getBoolean("is_deleted"));
    data->setDeletedAt(nullableString(resultSet, "deleted_at"));
    data->setDeletedBy(nullableString(resultSet, "deleted_by"));
    data->setVersion(resultSet->getInt("version"));
    data->setBusNo(nullableString(resultSet, "bus_no"));
    data->setName(resultSet->getString("name"));
    data->setTypeEnum(nullableString(resultSet, "type_enum"));
    data->setCreditCode(nullableString(resultSet, "credit_code"));
    data->setLegalRepresentative(nullableString(resultSet, "legal_representative"));
    data->setAddress(nullableString(resultSet, "address"));
    data->setPyStr(nullableString(resultSet, "py_str"));
    data->setWbStr(nullableString(resultSet, "wb_str"));
    data->setPhone(nullableString(resultSet, "phone"));
    data->setEmail(nullableString(resultSet, "email"));
    data->setBankName(nullableString(resultSet, "bank_name"));
    data->setBankAccount(nullableString(resultSet, "bank_account"));
    data->setOrgId(nullableString(resultSet, "org_id"));
    data->setDisplayOrder(resultSet->isNull("display_order") ? 0 : resultSet->getInt("display_order"));
    data->setStatusEnum(nullableString(resultSet, "status_enum"));
    data->setNote(nullableString(resultSet, "note"));
    return data;
}

std::string SupplierStringMapper::mapper(ResultSet* resultSet) const
{
    return resultSet->isNull(column) ? "" : resultSet->getString(column);
}
