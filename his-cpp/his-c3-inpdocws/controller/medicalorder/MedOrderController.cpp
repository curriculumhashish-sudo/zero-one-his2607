#include "stdafx.h"
#include "MedOrderController.h"
#include "service/medicalorder/OrderMergeService.h"
#include "service/medicalorder/OrderSplitService.h"
#include "service/medicalorder/OrderStopService.h"

MergeResultJsonVO::Wrapper MedOrderController::execMergeOrder(const OrderMergeDTO::Wrapper& dto)
{
	//¥¥Ω®œÏ”¶vo
	auto vo = MergeResultJsonVO::createShared();
	return vo;
}

SplitResultJsonVO::Wrapper MedOrderController::execSplitOrder(const OrderSplitDTO::Wrapper& dto)
{
    auto vo = SplitResultJsonVO::createShared();
    return vo;
}

StopResultJsonVO::Wrapper MedOrderController::execStopOrder(const OrderStopDTO::Wrapper& dto)
{
    auto vo = StopResultJsonVO::createShared();
    return vo;
}