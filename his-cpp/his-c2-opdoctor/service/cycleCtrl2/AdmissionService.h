#pragma once

#ifndef _ADMISSION_SERVICE_
#define _ADMISSION_SERVICE_
#include "domain/dto/doctor/RequestInpatientAdmissionDTO.h"
#include "domain/dto/doctor/Admission2ResultDTO.h"

class AdmissionService
{
public:
    oatpp::Object<Admission2ResultDTO> applyAdmission(const RequestInpatientAdmissionDTO::Wrapper& dto,
        const std::string& operatorId);
};

#endif
