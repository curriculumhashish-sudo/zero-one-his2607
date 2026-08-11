#include "stdafx.h"
#include "MedOrderController.h"
#include "service/medicalorder/OrderMergeService.h"
#include "service/medicalorder/OrderSplitService.h"
#include "service/medicalorder/OrderStopService.h"


//并组+拆组
MergeResultJsonVO::Wrapper MedOrderController::execMergeOrder(
    const OrderMergeDTO::Wrapper& dto)
{
    auto vo = MergeResultJsonVO::createShared();
    OrderMergeService service;
    auto result = service.mergeOrders(dto);
    if (result) {
        vo->success(result);
    }
    else {
        vo->fail({});
    }
    return vo;
}

//拆组
SplitResultJsonVO::Wrapper MedOrderController::execSplitOrder(
    const OrderSplitDTO::Wrapper& dto)
{
    auto vo = SplitResultJsonVO::createShared();
    OrderSplitService service;
    auto result = service.splitOrder(dto);
    if (result) {
        vo->success(result);
    }
    else {
        vo->fail({});
    }
    return vo;
}

//批量停止长期医嘱
StopResultJsonVO::Wrapper MedOrderController::execStopOrder(
    const OrderStopDTO::Wrapper& dto)
{
    auto vo = StopResultJsonVO::createShared();
    OrderStopService service;
    auto result = service.stopOrders(dto);
    if (result) {
        vo->success(result);
    }
    else {
        vo->fail({});
    }

    return vo;
}