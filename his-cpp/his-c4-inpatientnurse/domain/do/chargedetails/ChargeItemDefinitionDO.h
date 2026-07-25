#pragma once
#ifndef _CHARGEITEMDEFINITIONDO_
#define _CHARGEITEMDEFINITIONDO_

#include "../DoInclude.h"

class ChargeItemDefinitionDO : public BaseDO
{
public:
    ChargeItemDefinitionDO() : BaseDO("adm_charge_item_definition") {}
};

typedef std::shared_ptr<ChargeItemDefinitionDO> PtrChargeItemDefinitionDO;

#endif
