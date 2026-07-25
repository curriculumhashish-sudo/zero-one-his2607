#pragma once
#ifndef _VITALSIGNDETAILDO_
#define _VITALSIGNDETAILDO_

#include "../DoInclude.h"

class VitalSignDetailDO : public BaseDO
{
public:
    VitalSignDetailDO() : BaseDO("dia_vital_sign_detail") {}
};

typedef std::shared_ptr<VitalSignDetailDO> PtrVitalSignDetailDO;

#endif
