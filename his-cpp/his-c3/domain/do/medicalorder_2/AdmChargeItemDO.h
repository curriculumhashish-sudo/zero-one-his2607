#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ADMCHARGEITEMDO_H_
#define _ADMCHARGEITEMDO_H_

#include "../DoInclude.h"

/*
    收费项目记录（对应 adm_charge_item 表）在拆分/停止订单时，必须执行同步
*/
class AdmChargeItemDO : public BaseDO
{
    MYSQL_SYNTHESIZE(std::string, id, Id);
    //父收费项目ID —— 拆分时清空
    MYSQL_SYNTHESIZE(std::string, parentId, ParentId);
    //源表为 med_medication_request 或 wkf_service_request
    MYSQL_SYNTHESIZE(std::string, serviceTable, ServiceTable);
    //来源记录ID
    MYSQL_SYNTHESIZE(std::string, serviceId, ServiceId);
    //状态改变
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);

public:
    AdmChargeItemDO() : BaseDO("adm_charge_item")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("parent_id", "s", parentId);
        MYSQL_ADD_FIELD("service_table", "s", serviceTable);
        MYSQL_ADD_FIELD("service_id", "s", serviceId);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
    }
};

typedef std::shared_ptr<AdmChargeItemDO> PtrAdmChargeItemDO;

#endif