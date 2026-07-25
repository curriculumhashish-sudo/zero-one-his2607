#pragma once
#ifndef _VITALSIGNDO_
#define _VITALSIGNDO_

#include "../DoInclude.h"

class VitalSignDO : public BaseDO
{
public:
    VitalSignDO() : BaseDO("dia_vital_sign") {}
};

typedef std::shared_ptr<VitalSignDO> PtrVitalSignDO;

#endif
