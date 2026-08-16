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
#include "domain/do/medicalorder_2/AdmChargeItemDO.h"

/*
    ͣ��ǰ�������շ�״̬��δ�շ� �� ��ͣ,���շ� �� ǰ�˵�����ʾ�˷�
*/
class ChargeItemDAO : public BaseDAO
{
public:
    //����Դ��+��ԴID���շѼ�¼��
    std::list<PtrAdmChargeItemDO> selectByService(
        const std::string& serviceTable, const std::string& serviceId);
};

#endif