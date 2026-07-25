#pragma once
#ifndef _ACCOUNTDO_
#define _ACCOUNTDO_

#include "../DoInclude.h"

class AccountDO : public BaseDO
{
public:
    AccountDO() : BaseDO("adm_account") {}
};

typedef std::shared_ptr<AccountDO> PtrAccountDO;

#endif
