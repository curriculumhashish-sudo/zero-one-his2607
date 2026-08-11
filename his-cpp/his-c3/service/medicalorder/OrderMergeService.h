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
#include "domain/dto/medicalorder/OrderMergeDTO.h"
#include "domain/vo/medicalorder/MedicalOrderVO.h"

//并组service层
class OrderMergeService
{
public:
    /**
     *合并拆分
     *@param dto合并请求对象
     */
    MergeResultDTO::Wrapper mergeOrders(const OrderMergeDTO::Wrapper& dto);

private:
    //前置校验：验证就诊记录存在、患者匹配、IMP类型、可写状态
    bool gateValidateEncounter(const std::string& encounterId, const std::string& patientId, bool writable);
};

#endif