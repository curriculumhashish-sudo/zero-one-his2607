#include "stdafx.h"
#include "SupplierAddDTO.h"
#include "SupplierUpdateDTO.h"

namespace
{
    bool isValidTypeEnum(const std::string& value)
    {
        // 1 = Manufacturer, 2 = Supplier
        return value == "1" || value == "2";
    }
}

std::string SupplierAddDTO::validate() const
{
    // name is a NOT NULL column and the primary search field.
    if (!name || name->empty())
        return "name is required.";
    if (name->size() > 255)
        return "name must not exceed 255 characters.";
    // type_enum accepts only "1" (manufacturer) or "2" (supplier).
    if (!typeEnum || !isValidTypeEnum(typeEnum->c_str()))
        return "typeEnum must be 1 or 2.";
    // org_id is NOT NULL in cus_supplier; the client must pick the owning organization.
    if (!orgId || orgId->empty())
        return "orgId is required.";
    if (orgId->size() > 64)
        return "orgId must not exceed 64 characters.";
    if (displayOrder && displayOrder.getValue(0) < 0)
        return "displayOrder must be greater than or equal to 0.";
    return "";
}

std::string SupplierUpdateDTO::validate() const
{
    if (!id || id->empty())
        return "id is required.";
    // version drives the optimistic-lock check; it must be present and non-negative.
    if (!version)
        return "version is required.";
    if (version.getValue(-1) < 0)
        return "version must be greater than or equal to 0.";
    // Patch semantics: only validate fields that were provided (non-null).
    if (name && name->empty())
        return "name must not be empty.";
    if (name && name->size() > 255)
        return "name must not exceed 255 characters.";
    if (typeEnum && !isValidTypeEnum(typeEnum->c_str()))
        return "typeEnum must be 1 or 2.";
    if (orgId && orgId->empty())
        return "orgId must not be empty.";
    if (orgId && orgId->size() > 64)
        return "orgId must not exceed 64 characters.";
    if (displayOrder && displayOrder.getValue(0) < 0)
        return "displayOrder must be greater than or equal to 0.";
    return "";
}
