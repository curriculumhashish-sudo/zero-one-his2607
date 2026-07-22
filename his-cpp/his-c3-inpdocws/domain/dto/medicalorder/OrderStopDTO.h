#pragma once

/*
 Copyright Zero One Star. All rights reserved.
 停嘱请求DTO
 @Author: PKQ
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ORDERSTOPDTO_H_
#define _ORDERSTOPDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
	停嘱请求DTO
*/
class OrderStopDTO : public oatpp::DTO
{
	DTO_INIT(OrderStopDTO, DTO);
	//停嘱列表
	API_DTO_FIELD_DEFAULT(List<String>, orderIds, ZH_WORDS_GETTER("medicalorder.field.orderIds"));
	//停嘱原因码
	API_DTO_FIELD_DEFAULT(Int32, stopReason, ZH_WORDS_GETTER("medicalorder.field.stopReason"));
	//是否自动退费
	API_DTO_FIELD_DEFAULT(Boolean, autoRefund, ZH_WORDS_GETTER("medicalorder.field.autoRefund"));
};


#include OATPP_CODEGEN_END(DTO)

#endif