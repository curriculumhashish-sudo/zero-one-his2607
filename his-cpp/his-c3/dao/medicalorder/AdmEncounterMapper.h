#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/23 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ADMENCOUNTERMAPPER_H_
#define _ADMENCOUNTERMAPPER_H_
#include "Mapper.h"
#include "domain/do/medicalorder/AdmEncounterDO.h"


class AdmEncounterMapper : public Mapper<PtrAdmEncounterDO>
{
public:
    PtrAdmEncounterDO mapper(ResultSet* resultSet) const override
    {
        PtrAdmEncounterDO obj = std::make_shared<AdmEncounterDO>();
        obj->setId(resultSet->getString("id"));
        obj->setPatientId(resultSet->getString("patient_id"));
        obj->setStatusEnum(resultSet->getString("status_enum"));
        obj->setOrgId(resultSet->getString("org_id"));
        return obj;
    }
};

#endif