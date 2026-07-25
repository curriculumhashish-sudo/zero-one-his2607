#pragma once
#ifndef _PATIENTDO_
#define _PATIENTDO_

#include "../DoInclude.h"

class PatientDO : public BaseDO
{
public:
    PatientDO() : BaseDO("adm_patient") {}
};

typedef std::shared_ptr<PatientDO> PtrPatientDO;

#endif
