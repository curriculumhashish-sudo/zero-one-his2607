#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _CHARGEITEMDAO_H_
#define _CHARGEITEMDAO_H_
#include "BaseDAO.h"
#include "domain/do/medicalorder/AdmChargeItemDO.h"

/*
    停嘱前建议检查收费状态：未收费 → 可停,已收费 → 前端弹窗提示退费
*/
class ChargeItemDAO : public BaseDAO
{
public:
    //按来源表+来源ID查收费记录。
    std::list<PtrAdmChargeItemDO> selectByService(
        const std::string& serviceTable, const std::string& serviceId);
};

#endif