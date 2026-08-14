#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/23 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _DISPENSEMAPPER_H_
#define _DISPENSEMAPPER_H_
#include "Mapper.h"
#include "domain/do/medicalorder_2/MedMedicationDispenseDO.h"


class DispenseMapper : public Mapper<PtrMedMedicationDispenseDO>
{
public:
    PtrMedMedicationDispenseDO mapper(ResultSet* resultSet) const override
    {
        PtrMedMedicationDispenseDO obj = std::make_shared<MedMedicationDispenseDO>();
        obj->setId(resultSet->getString("id"));
        obj->setMedReqId(resultSet->getString("med_req_id"));
        obj->setStatusEnum(resultSet->getString("status_enum"));
        obj->setDispenseQuantity(resultSet->getInt("dispense_quantity"));
        return obj;
    }
};

#endif