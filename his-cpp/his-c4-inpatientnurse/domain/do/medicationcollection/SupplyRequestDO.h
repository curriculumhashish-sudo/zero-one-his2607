#pragma once
#ifndef _SUPPLYREQUESTDO_
#define _SUPPLYREQUESTDO_

#include "../DoInclude.h"

class SupplyRequestDO : public BaseDO
{
public:
    SupplyRequestDO() : BaseDO("wkf_supply_request") {}
};

typedef std::shared_ptr<SupplyRequestDO> PtrSupplyRequestDO;

#endif
