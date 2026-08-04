#pragma once
#ifndef _PATIENT_STATUS_H_
#define _PATIENT_STATUS_H_

#include "ResultStatus.h"

namespace PatientStatus
{
    inline ResultStatus invalid(const std::string& message)
    {
        return ResultStatus(message, 54001);
    }

    inline ResultStatus notFound()
    {
        return ResultStatus("patient does not exist", 54002);
    }

    inline ResultStatus forbidden()
    {
        return ResultStatus("no permission to operate this patient", 54003);
    }

    inline ResultStatus duplicateBusNo()
    {
        return ResultStatus("patient business number already exists", 54004);
    }

    inline ResultStatus duplicateIdCard()
    {
        return ResultStatus("patient identity card already exists", 54005);
    }

    inline ResultStatus organizationNotFound()
    {
        return ResultStatus("organization does not exist in the current tenant", 54006);
    }

    inline ResultStatus updateConflict()
    {
        return ResultStatus("patient update conflict", 54007);
    }

    inline ResultStatus databaseFailure()
    {
        return ResultStatus("patient database operation failed", 54008);
    }
}

#endif // !_PATIENT_STATUS_H_
