#pragma once
#ifndef _SUPPLYDELIVERYDO_
#define _SUPPLYDELIVERYDO_

#include "../DoInclude.h"

class SupplyDeliveryDO : public BaseDO
{
public:
    SupplyDeliveryDO() : BaseDO("wkf_supply_delivery") {}
};

typedef std::shared_ptr<SupplyDeliveryDO> PtrSupplyDeliveryDO;

#endif
