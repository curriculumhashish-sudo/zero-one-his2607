#pragma once

/*
 Copyright Zero One Star. All rights reserved.
 拆嘱请求DTO
 @Author: PKQ
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ORDERSPLITDTO_H_
#define _ORDERSPLITDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
	拆嘱请求DTO
*/
class OrderSplitDTO : public oatpp::DTO
{
	DTO_INIT(OrderSplitDTO, DTO);
	//拆分医嘱ID
	API_DTO_FIELD_DEFAULT(String, orderId, ZH_WORDS_GETTER("medicalorder.field.orderId"));
	//如果所有患者在一个组，设置为true(可拆分)
	API_DTO_FIELD_DEFAULT(Boolean, splitAll, ZH_WORDS_GETTER("medicalorder.field.splitAll"));
};

#include OATPP_CODEGEN_END(DTO)

#endif