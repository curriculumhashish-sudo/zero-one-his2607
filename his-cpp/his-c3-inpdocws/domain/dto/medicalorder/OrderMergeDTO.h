#pragma once

/*
 Copyright Zero One Star. All rights reserved.
 并嘱请求DTO
 @Author: PKQ
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ORDERMERGEDTO_H_
#define _ORDERMERGEDTO_H_

#include "../../GlobalInclude.h"


#include OATPP_CODEGEN_BEGIN(DTO)

/*
	合并组请求
*/
class OrderMergeDTO : public oatpp::DTO
{
	DTO_INIT(OrderMergeDTO, DTO);
	//合并医嘱ID列表
	API_DTO_FIELD_DEFAULT(List<String>, orderIds, ZH_WORDS_GETTER("medicalorder.field.orderIds"));
	//患者ID
	API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("medicalorder.field.patientId"));
	//就诊ID
	API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("medicalorder.field.encounterId"));
};



#include OATPP_CODEGEN_END(DTO)

#endif