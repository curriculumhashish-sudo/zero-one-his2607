#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/

#ifndef _MEDICALORDERQUERY_H_
#define _MEDICALORDERQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
	医嘱查询搜索对象
*/
class MedicalOrderQuery : public PageQuery
{
	DTO_INIT(MedicalOrderQuery, PageQuery);
	//患者ID
	API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("medicalorder.field.patientId"));

	//就诊ID
	API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("medicalorder.field.encounterId"));

	//医嘱状态
	API_DTO_FIELD_DEFAULT(String, statusEnum, ZH_WORDS_GETTER("medicalorder.field.statusEnum"));

	//分组编号
	API_DTO_FIELD_DEFAULT(String, groupId, ZH_WORDS_GETTER("medicalorder.field.groupId"));
};


#include OATPP_CODEGEN_END(DTO)

#endif