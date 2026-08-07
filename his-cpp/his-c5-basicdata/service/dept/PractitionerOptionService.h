#ifndef _PRACTITIONER_OPTION_SERVICE_H_
#define _PRACTITIONER_OPTION_SERVICE_H_

#include "domain/dto/dept/PractitionerOptionDTO.h"
#include "domain/dto/PayloadDTO.h"

#include <list>

struct PractitionerOptionQueryResult
{
    bool tenantFound = false;
    std::list<PractitionerOptionDTO::Wrapper> options;
};

class PractitionerOptionService
{
public:
    PractitionerOptionQueryResult getDoctorOptions(
        const PayloadDTO& payload
    );
};

#endif // !_PRACTITIONER_OPTION_SERVICE_H_
