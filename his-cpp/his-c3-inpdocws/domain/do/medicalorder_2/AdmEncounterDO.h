#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ADMENCOUNTERDO_H_
#define _ADMENCOUNTERDO_H_

#include "../DoInclude.h"

/*
    用于校验：检查患者和就诊记录是否存在
*/
class AdmEncounterDO : public BaseDO
{
    MYSQL_SYNTHESIZE(std::string, id, Id);
    MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(std::string, orgId, OrgId);

public:
    AdmEncounterDO() : BaseDO("adm_encounter")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("org_id", "s", orgId);
    }
};

typedef std::shared_ptr<AdmEncounterDO> PtrAdmEncounterDO;

#endif