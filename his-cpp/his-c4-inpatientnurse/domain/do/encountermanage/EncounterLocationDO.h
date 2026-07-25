#pragma once
#ifndef _ENCOUNTERLOCATIONDO_
#define _ENCOUNTERLOCATIONDO_

#include "../DoInclude.h"

class EncounterLocationDO : public BaseDO
{
public:
    EncounterLocationDO() : BaseDO("adm_encounter_location") {}
};

typedef std::shared_ptr<EncounterLocationDO> PtrEncounterLocationDO;

#endif
