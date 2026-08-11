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

// 单条停嘱行：标识待停止的医嘱项
class StopLineDTO : public oatpp::DTO
{
    DTO_INIT(StopLineDTO, DTO);
    // 待停止的医嘱/服务请求ID
    API_DTO_FIELD_DEFAULT(String, requestId, ZH_WORDS_GETTER("medicalorder.field.orderId"));
    // 产品定义表名（如 med_medication_definition / wkf_activity_definition）
    API_DTO_FIELD_DEFAULT(String, productTableName, ZH_WORDS_GETTER("medicalorder.field.productTableName"));
};

// 停止医嘱请求DTO
class OrderStopDTO : public oatpp::DTO
{
    DTO_INIT(OrderStopDTO, DTO);
    // 就诊ID
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("medicalorder.field.encounterId"));
    // 患者ID
    API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("medicalorder.field.patientId"));
    // 操作人ID
    API_DTO_FIELD_DEFAULT(String, operatorId, ZH_WORDS_GETTER("medicalorder.field.operatorId"));
    // 停嘱原因码
    API_DTO_FIELD_DEFAULT(Int32, stopReason, ZH_WORDS_GETTER("medicalorder.field.stopReason"));
    // 停嘱行列表（每条指定requestId + productTableName）
    API_DTO_FIELD_DEFAULT(List<Object<StopLineDTO>>, lines, ZH_WORDS_GETTER("medicalorder.field.lines"));
};

#include OATPP_CODEGEN_END(DTO)

#endif
