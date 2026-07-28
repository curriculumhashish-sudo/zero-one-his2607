#pragma once
#ifndef _DEPT_STATUS_H_
#define _DEPT_STATUS_H_

#include "ResultStatus.h"

namespace DeptStatus
{
    inline ResultStatus invalid(const std::string& message)
    {
        return ResultStatus(message, 52001);
    }

    inline ResultStatus notFound()
    {
        return ResultStatus("department does not exist", 52002);
    }

    inline ResultStatus forbidden()
    {
        return ResultStatus("no permission to operate this department", 52003);
    }

    inline ResultStatus duplicateName()
    {
        return ResultStatus("department name already exists", 52004);
    }

    inline ResultStatus invalidParent()
    {
        return ResultStatus("parent organization must be a hospital in the same tenant", 52005);
    }

    inline ResultStatus hasChildren()
    {
        return ResultStatus("hospital has child departments", 52006);
    }

    inline ResultStatus doctorNotFound()
    {
        return ResultStatus("default doctor does not exist", 52007);
    }

    inline ResultStatus businessReferenced()
    {
        return ResultStatus("department is referenced by business data", 52008);
    }

    inline ResultStatus enabledDeleteDenied()
    {
        return ResultStatus("enabled department cannot be deleted", 52009);
    }

    inline ResultStatus updateConflict()
    {
        return ResultStatus("department update conflict", 52010);
    }

    inline ResultStatus databaseFailure()
    {
        return ResultStatus("department database operation failed", 52011);
    }
}

#endif // !_DEPT_STATUS_H_
