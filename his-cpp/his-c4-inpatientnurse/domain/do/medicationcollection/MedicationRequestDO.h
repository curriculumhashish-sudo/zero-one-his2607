#pragma once
#ifndef _MEDICATIONREQUESTDO_
#define _MEDICATIONREQUESTDO_

#include "../DoInclude.h"

class MedicationRequestDO : public BaseDO
{
public:
    MedicationRequestDO() : BaseDO("med_medication_request") {}
};

typedef std::shared_ptr<MedicationRequestDO> PtrMedicationRequestDO;

#endif
