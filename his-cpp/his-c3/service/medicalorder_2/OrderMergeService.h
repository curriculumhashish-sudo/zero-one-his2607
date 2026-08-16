#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ORDERMERGESERVICE_H_
#define _ORDERMERGESERVICE_H_
#include "domain/dto/medicalorder_2/OrderMergeDTO.h"
#include "domain/vo/medicalorder_2/MedicalOrderVO.h"

//����service��
class OrderMergeService
{
public:
    /**
     *�ϲ����
     *@param dto�ϲ��������
     */
    MergeResultDTO::Wrapper mergeOrders(const OrderMergeDTO::Wrapper& dto);

private:
    //ǰ��У�飺��֤�����¼���ڡ�����ƥ�䡢IMP���͡���д״̬
    bool gateValidateEncounter(const std::string& encounterId, const std::string& patientId, bool writable);
};

#endif