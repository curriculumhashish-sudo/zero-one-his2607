#pragma once
#ifndef _ENCOUNTERDIAGNOSISDO_
#define _ENCOUNTERDIAGNOSISDO_

#include "../DoInclude.h"

class EncounterDiagnosisDO : public BaseDO
{
public:
    EncounterDiagnosisDO() : BaseDO("adm_encounter_diagnosis") {}
};

typedef std::shared_ptr<EncounterDiagnosisDO> PtrEncounterDiagnosisDO;

#endif
