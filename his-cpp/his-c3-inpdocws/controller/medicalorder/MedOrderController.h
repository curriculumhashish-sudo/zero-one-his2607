#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 
 @Author: PKQ
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/

#ifndef _MEDORDERCONTROLLER_H_
#define _MEDORDERCOHTROLLER_H_

#include "domain/query/medicalorder/MedicalOrderQuery.h"
#include "domain/vo/medicalorder/MedicalOrderVO.h"
#include "domain/dto/medicalorder/OrderMergeDTO.h"
#include "domain/dto/medicalorder/OrderSplitDTO.h"
#include "domain/dto/medicalorder/OrderStopDTO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("medicalorder.tag")

class MedOrderController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(MedOrderController);
public:
	//并组医嘱
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medicalorder.merge-order.summary"), mergeOrder, MergeResultJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/order/merge_order", mergeOrder, BODY_DTO(OrderMergeDTO::Wrapper, dto), execMergeOrder(dto));

    //拆嘱医嘱
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medicalorder.split-order.summary"), splitOrder, SplitResultJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/order/split-order", splitOrder, BODY_DTO(OrderSplitDTO::Wrapper, dto), execSplitOrder(dto));

    //(停嘱)
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medicalorder.stop-order.summary"), stopOrder, StopResultJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/order/stop-order", stopOrder, BODY_DTO(OrderStopDTO::Wrapper, dto), execStopOrder(dto));
private:
	MergeResultJsonVO::Wrapper execMergeOrder(const OrderMergeDTO::Wrapper& dto);
	SplitResultJsonVO::Wrapper execSplitOrder(const OrderSplitDTO::Wrapper& dto);
	StopResultJsonVO::Wrapper execStopOrder(const OrderStopDTO::Wrapper& dto);


};

#include OATPP_CODEGEN_END(ApiController)

#endif