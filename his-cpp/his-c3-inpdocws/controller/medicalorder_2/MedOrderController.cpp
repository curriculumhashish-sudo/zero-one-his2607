#include "stdafx.h"
#include "MedOrderController.h"
#include "service/medicalorder_2/OrderMergeService.h"
#include "service/medicalorder_2/OrderSplitService.h"
#include "service/medicalorder_2/OrderStopService.h"


// Merge + Split
MergeResultJsonVO::Wrapper MedOrderController::execMergeOrder(
    const OrderMergeDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto vo = MergeResultJsonVO::createShared();
    // Set operator from auth payload; do not trust client-supplied operatorId
    dto->operatorId = payload.getId();
    OrderMergeService service;
    auto result = service.mergeOrders(dto);
    vo->success(result);
    return vo;
}

// Split
SplitResultJsonVO::Wrapper MedOrderController::execSplitOrder(
    const OrderSplitDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto vo = SplitResultJsonVO::createShared();
    // Set operator from auth payload; do not trust client-supplied operatorId
    dto->operatorId = payload.getId();
    OrderSplitService service;
    auto result = service.splitOrder(dto);
    vo->success(result);
    return vo;
}

// Batch stop orders
StopResultJsonVO::Wrapper MedOrderController::execStopOrder(
    const OrderStopDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto vo = StopResultJsonVO::createShared();
    // Set operator from auth payload; do not trust client-supplied operatorId
    dto->operatorId = payload.getId();
    OrderStopService service;
    auto result = service.stopOrders(dto);
    vo->success(result);
    return vo;
}
