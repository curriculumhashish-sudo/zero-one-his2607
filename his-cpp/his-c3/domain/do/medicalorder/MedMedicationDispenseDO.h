#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _MEDMEDICATIONDISPENSEDO_H_
#define _MEDMEDICATIONDISPENSEDO_H_

#include "../DoInclude.h"

/*
    药品发放记录，映射到 med_medication_dispense 表。停止订单前必须检查
*/
class MedMedicationDispenseDO : public BaseDO
{
    MYSQL_SYNTHESIZE(std::string, id, Id);
    //外键指向 med_medication_request.id
    MYSQL_SYNTHESIZE(std::string, medReqId, MedReqId);
    //配发状态："未配发"/ "已配发" /"部分配发"
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    //已发放数量
    MYSQL_SYNTHESIZE(int, dispenseQuantity, DispenseQuantity);

public:
    MedMedicationDispenseDO() : BaseDO("med_medication_dispense")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("med_req_id", "s", medReqId);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("dispense_quantity", "i", dispenseQuantity);
    }
};

typedef std::shared_ptr<MedMedicationDispenseDO> PtrMedMedicationDispenseDO;

#endif