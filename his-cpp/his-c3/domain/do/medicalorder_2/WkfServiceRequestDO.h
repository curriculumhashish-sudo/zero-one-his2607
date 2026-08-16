#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _WKFSERVICEREQUESTDO_H_
#define _WKFSERVICEREQUESTDO_H_

#include "../DoInclude.h"

/*
    非药品服务订单数据库对象
*/
class WkfServiceRequestDO : public BaseDO
{
    MYSQL_SYNTHESIZE(std::string, id, Id);
    MYSQL_SYNTHESIZE(std::string, groupId, GroupId);
    MYSQL_SYNTHESIZE(std::string, parentId, ParentId);
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(std::string, therapyEnum, TherapyEnum);
    MYSQL_SYNTHESIZE(std::string, statusChangedTime, StatusChangedTime);
    MYSQL_SYNTHESIZE(std::string, basedOnTable, BasedOnTable);
    MYSQL_SYNTHESIZE(std::string, basedOnId, BasedOnId);
    MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
    MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);


public:
    WkfServiceRequestDO() : BaseDO("wkf_service_request")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("group_id", "s", groupId);
        MYSQL_ADD_FIELD("parent_id", "s", parentId);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("therapy_enum", "s", therapyEnum);
        MYSQL_ADD_FIELD("status_changed_time", "s", statusChangedTime);
        MYSQL_ADD_FIELD("based_on_table", "s", basedOnTable);
        MYSQL_ADD_FIELD("based_on_id", "s", basedOnId);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        
    }
};

typedef std::shared_ptr<WkfServiceRequestDO> PtrWkfServiceRequestDO;

#endif