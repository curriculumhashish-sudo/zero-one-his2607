#pragma once
#ifndef _ENCOUNTERPARTICIPANTDO_
#define _ENCOUNTERPARTICIPANTDO_

#include "../DoInclude.h"

class EncounterParticipantDO : public BaseDO
{
public:
    EncounterParticipantDO() : BaseDO("adm_encounter_participant") {}
};

typedef std::shared_ptr<EncounterParticipantDO> PtrEncounterParticipantDO;

#endif
