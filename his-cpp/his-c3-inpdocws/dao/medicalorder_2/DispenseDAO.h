#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _DISPENSEDAO_H_
#define _DISPENSEDAO_H_
#include "BaseDAO.h"
#include "domain/do/medicalorder_2/MedMedicationDispenseDO.h"

//ͣ��ǰ�����鷢ҩ״̬
class DispenseDAO : public BaseDAO
{
public:
    //��ҩƷ����ID�鷢ҩ��¼��
    PtrMedMedicationDispenseDO selectByMedReqId(const std::string& medReqId);

    //��ҩƷ����ID���Ƿ����ѷ�ҩ�ļ�¼
    bool hasDispensedRecord(const std::string& medReqId);
};

#endif