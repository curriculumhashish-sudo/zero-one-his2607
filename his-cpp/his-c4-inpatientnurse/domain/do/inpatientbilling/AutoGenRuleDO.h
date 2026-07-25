#pragma once
#ifndef _AUTOGENRULEDO_
#define _AUTOGENRULEDO_

#include "../DoInclude.h"

class AutoGenRuleDO : public BaseDO
{
public:
    AutoGenRuleDO() : BaseDO("cus_charge_item_auto_gen") {}
};

typedef std::shared_ptr<AutoGenRuleDO> PtrAutoGenRuleDO;

#endif
