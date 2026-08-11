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
#include "domain/do/medicalorder/MedMedicationDispenseDO.h"

//停嘱前必须检查发药状态
class DispenseDAO : public BaseDAO
{
public:
    //按药品请求ID查发药记录。
    PtrMedMedicationDispenseDO selectByMedReqId(const std::string& medReqId);

    //按药品请求ID查是否有已发药的记录
    bool hasDispensedRecord(const std::string& medReqId);
};

#endif