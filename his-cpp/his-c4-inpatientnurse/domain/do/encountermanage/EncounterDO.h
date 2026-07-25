#pragma once
#ifndef _ENCOUNTERDO_
#define _ENCOUNTERDO_

#include "../DoInclude.h"

class EncounterDO : public BaseDO
{
public:
    EncounterDO() : BaseDO("adm_encounter") {}
};

typedef std::shared_ptr<EncounterDO> PtrEncounterDO;

#endif
