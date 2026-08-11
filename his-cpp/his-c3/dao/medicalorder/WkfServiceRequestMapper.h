#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/23 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _WKFSERVICEREQUESTMAPPER_H_
#define _WKFSERVICEREQUESTMAPPER_H_
#include "Mapper.h"
#include "domain/do/medicalorder/WkfServiceRequestDO.h"


class WkfServiceRequestMapper : public Mapper<PtrWkfServiceRequestDO>
{
public:
    PtrWkfServiceRequestDO mapper(ResultSet* resultSet) const override
    {
        PtrWkfServiceRequestDO obj = std::make_shared<WkfServiceRequestDO>();
        obj->setId(resultSet->getString("id"));
        obj->setGroupId(resultSet->getString("group_id"));
        obj->setParentId(resultSet->getString("parent_id"));
        obj->setStatusEnum(resultSet->getString("status_enum"));
        obj->setTherapyEnum(resultSet->getString("therapy_enum"));
        obj->setBasedOnTable(resultSet->getString("based_on_table"));
        obj->setBasedOnId(resultSet->getString("based_on_id"));
        obj->setPatientId(resultSet->getString("patient_id"));
        obj->setEncounterId(resultSet->getString("encounter_id"));
        obj->setStatusChangedTime(resultSet->getString("status_changed_time"));
        return obj;
    }
};

#endif