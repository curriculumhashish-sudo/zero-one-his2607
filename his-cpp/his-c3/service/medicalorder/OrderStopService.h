#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ORDERSTOPSERVICE_H_
#define _ORDERSTOPSERVICE_H_
#include "domain/dto/medicalorder/OrderStopDTO.h"
#include "domain/vo/medicalorder/MedicalOrderVO.h"


class OrderStopService
{
public:
    //批量停嘱
    StopResultDTO::Wrapper stopOrders(const OrderStopDTO::Wrapper& dto);

private:
    //门控校验辅助函数
    bool gateValidateEncounter(const std::string& encounterId,
        const std::string& patientId,
        bool writable);

    //前置条件
    void assertLongTermMedication(const std::string& requestId);

    //诊疗服务请求必须为长期医嘱
    void assertLongTermService(const std::string& encounterId,
        const std::string& requestId);

    //产品表路由
    static bool isMedicationTable(const std::string& productTableName);
    static bool isActivityTable(const std::string& productTableName);
    static std::string normalizeTableName(const std::string& raw);
};

#endif // !_ORDERSTOPSERVICE_H_