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
#include "domain/dto/medicalorder_2/OrderStopDTO.h"
#include "domain/vo/medicalorder_2/MedicalOrderVO.h"


class OrderStopService
{
public:
    //����ͣ��
    StopResultDTO::Wrapper stopOrders(const OrderStopDTO::Wrapper& dto);

private:
    //�ſ�У�鸨������
    bool gateValidateEncounter(const std::string& encounterId,
        const std::string& patientId,
        bool writable);

    //ǰ������
    void assertLongTermMedication(const std::string& requestId, const std::string& encounterId, const std::string& patientId);

    //���Ʒ����������Ϊ����ҽ��
    void assertLongTermService(const std::string& encounterId, const std::string& requestId, const std::string& patientId);

    //��Ʒ��·��
    static bool isMedicationTable(const std::string& productTableName);
    static bool isActivityTable(const std::string& productTableName);
    static std::string normalizeTableName(const std::string& raw);
};

#endif // !_ORDERSTOPSERVICE_H_