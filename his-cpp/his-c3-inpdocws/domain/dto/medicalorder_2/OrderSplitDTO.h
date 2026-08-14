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

    //就诊主键
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("medicalorder.field.encounterId"));
    //患者主键
    API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("medicalorder.field.patientId"));
    //操作人ID
    API_DTO_FIELD_DEFAULT(String, operatorId, ZH_WORDS_GETTER("medicalorder.field.operatorId"));

    //需要拆组的orderId列表
    API_DTO_FIELD_DEFAULT(List<String>, orderIds, ZH_WORDS_GETTER("medicalorder.field.orderIds"));
};

#include OATPP_CODEGEN_END(DTO)

#endif
