#include "stdafx.h"
#include "deptDTO.h"

bool DeptEnum::isType(const std::string& value)
{
    return value == HOSPITAL || value == DEPARTMENT;
}

bool DeptEnum::isClass(const std::string& value)
{
    return value == OUTPATIENT || value == INPATIENT ||
        value == PHARMACY || value == WAREHOUSE ||
        value == FINANCE || value == NURSE_STATION ||
        value == MANAGEMENT || value == LOGISTICS_SUPPORT;
}

bool DeptEnum::isStatus(const std::string& value)
{
    return value == STATUS_ENABLED || value == STATUS_DISABLED;
}

std::string DeptAddDTO::validate() const
{
    if (!name || name->empty())
        return "name is required.";
    if (name->size() > 255)
        return "name must not exceed 255 characters.";
    if (!typeEnum || !DeptEnum::isType(typeEnum->c_str()))
        return "typeEnum must be HOSPITAL or DEPARTMENT.";
    if (!statusEnum || !DeptEnum::isStatus(statusEnum->c_str()))
        return "statusEnum must be 1 or 0.";
    if (displayOrder && displayOrder.getValue(0) < 0)
        return "displayOrder must be greater than or equal to 0.";

    if (typeEnum == DeptEnum::DEPARTMENT)
    {
        if (!classEnum || !DeptEnum::isClass(classEnum->c_str()))
            return "classEnum is required for DEPARTMENT and must be supported.";
        if (!parentId || parentId->empty())
            return "parentId is required for DEPARTMENT.";
    }
    else if ((classEnum && !classEnum->empty()) ||
        (parentId && !parentId->empty()))
    {
        return "HOSPITAL must not specify classEnum or parentId.";
    }

    return "";
}

std::string DeptModifyDTO::validate() const
{
    if (!id || id->empty())
        return "id is required.";
    if (name && name->empty())
        return "name must not be empty.";
    if (name && name->size() > 255)
        return "name must not exceed 255 characters.";
    if (typeEnum && !DeptEnum::isType(typeEnum->c_str()))
        return "typeEnum must be HOSPITAL or DEPARTMENT.";
    if (classEnum && !classEnum->empty() &&
        !DeptEnum::isClass(classEnum->c_str()))
        return "classEnum is not supported.";
    if (statusEnum && !DeptEnum::isStatus(statusEnum->c_str()))
        return "statusEnum must be 1 or 0.";
    if (displayOrder && displayOrder.getValue(0) < 0)
        return "displayOrder must be greater than or equal to 0.";

    return "";
}
