#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/23 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ADMENCOUNTERDAO_H_
#define _ADMENCOUNTERDAO_H_
#include "BaseDAO.h"
#include "domain/do/medicalorder/AdmEncounterDO.h"


class AdmEncounterDAO : public BaseDAO
{
public:
    //按主键查就诊
    PtrAdmEncounterDO selectById(const std::string& encounterId);
};

#endif