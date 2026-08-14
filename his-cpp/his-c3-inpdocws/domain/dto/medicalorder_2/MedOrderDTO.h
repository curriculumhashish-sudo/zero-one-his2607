#pragma once

/*
 Copyright Zero One Star. All rights reserved.
 医用通用DTO
 @Author: PKQ
 @Date: 2026/07/20 00:00:00
 
 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _MEDORDERDTO_H_
#define _MEDORDERDTO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
	单个药品订单数据对象（用于列表展示）
*/
class MedOrderDTO : public oatpp::DTO
{
	DTO_INIT(MedOrderDTO, DTO);
	//医嘱ID
	API_DTO_FIELD_DEFAULT(String, id, ZH_WORDS_GETTER("medicalorder.field.id"));
	//业务编号
	API_DTO_FIELD_DEFAULT(String, busNo, ZH_WORDS_GETTER("medicalorder.field.busNo"));
	//分组编号
	API_DTO_FIELD_DEFAULT(String, groupId, ZH_WORDS_GETTER("medicalorder.field.groupId"));
	//医嘱状态
	API_DTO_FIELD_DEFAULT(String, statusEnum, ZH_WORDS_GETTER("medicalorder.field.statusEnum"));
	//患者ID
	API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("medicalorder.field.patientId"));
	//药品ID
	API_DTO_FIELD_DEFAULT(String, medicationId, ZH_WORDS_GETTER("medicalorder.field.medicationId"));
	//处方号
	API_DTO_FIELD_DEFAULT(String, prescriptionNo, ZH_WORDS_GETTER("medicalorder.field.prescriptionNo"));
};


/*
	分页药品订单列表
*/
class MedOrderPageDTO : public PageDTO<MedOrderDTO::Wrapper>
{
	DTO_INIT(MedOrderPageDTO, PageDTO<MedOrderDTO::Wrapper>);
};



#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDORDERDTO_H_