#pragma once

/*
 Copyright Zero One Star. All rights reserved.
 OrderStop DTOs
 @Author: PKQ
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ORDERSTOPDTO_H_
#define _ORDERSTOPDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Single stop line: identifies one order/service to stop
class StopLineDTO : public oatpp::DTO
{
    DTO_INIT(StopLineDTO, DTO);
    API_DTO_FIELD_DEFAULT(String, requestId, ZH_WORDS_GETTER("medicalorder.field.orderId"));
    API_DTO_FIELD_DEFAULT(String, productTableName, ZH_WORDS_GETTER("medicalorder.field.productTableName"));
};

// Stop failure detail: maps requestId to failure reason
class StopFailDetailDTO : public oatpp::DTO
{
    DTO_INIT(StopFailDetailDTO, DTO);
    API_DTO_FIELD_DEFAULT(String, requestId, ZH_WORDS_GETTER("medicalorder.field.orderId"));
    API_DTO_FIELD_DEFAULT(String, reason, ZH_WORDS_GETTER("medicalorder.field.reason"));
};

// Stop order request DTO
class OrderStopDTO : public oatpp::DTO
{
    DTO_INIT(OrderStopDTO, DTO);
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("medicalorder.field.encounterId"));
    API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("medicalorder.field.patientId"));
    API_DTO_FIELD_DEFAULT(String, operatorId, ZH_WORDS_GETTER("medicalorder.field.operatorId"));
    // stopReason code: 1-condition change, 2-adverse reaction, 3-order error, 4-patient request, 5-other
    API_DTO_FIELD_DEFAULT(Int32, stopReason, ZH_WORDS_GETTER("medicalorder.field.stopReason"));
    API_DTO_FIELD_DEFAULT(List<Object<StopLineDTO>>, lines, ZH_WORDS_GETTER("medicalorder.field.lines"));
};

#include OATPP_CODEGEN_END(DTO)

#endif
