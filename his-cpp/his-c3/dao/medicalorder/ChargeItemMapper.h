#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _CHARGEITEMMAPPER_H_
#define _CHARGEITEMMAPPER_H_
#include "Mapper.h"
#include "domain/do/medicalorder/AdmChargeItemDO.h"


class ChargeItemMapper : public Mapper<PtrAdmChargeItemDO>
{
public:
    PtrAdmChargeItemDO mapper(ResultSet* resultSet) const override
    {
        PtrAdmChargeItemDO obj = std::make_shared<AdmChargeItemDO>();
        obj->setId(resultSet->getString("id"));
        obj->setParentId(resultSet->getString("parent_id"));
        obj->setServiceTable(resultSet->getString("service_table"));
        obj->setServiceId(resultSet->getString("service_id"));
        obj->setStatusEnum(resultSet->getString("status_enum"));
        return obj;
    }
};

#endif