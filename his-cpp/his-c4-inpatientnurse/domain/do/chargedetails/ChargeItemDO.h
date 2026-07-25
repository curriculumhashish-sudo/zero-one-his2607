#pragma once
#ifndef _CHARGEITEMDO_
#define _CHARGEITEMDO_

#include "../DoInclude.h"

class ChargeItemDO : public BaseDO
{
public:
    ChargeItemDO() : BaseDO("adm_charge_item") {}
};

typedef std::shared_ptr<ChargeItemDO> PtrChargeItemDO;

#endif
