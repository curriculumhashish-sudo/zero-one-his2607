#include "ChargeItemController.h"
#include "service/charge/ChargeItemService.h"

// 获取患者缴费项
GetChargeItemVO::Wrapper ChargeItemController::execGetChargeItems(const GetChargeItemsQuery::Wrapper& query)
{
    auto vo = GetChargeItemVO::createShared();
    ChargeItemService service;
    auto dto = service.listAll(query);
    vo->success(dto);
    return vo;
}
