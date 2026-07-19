#include "ChargeItemController.h"

// 获取患者缴费项
GetChargeItemVO::Wrapper ChargeItemController::execGetChargeItems(const GetChargeItemsQuery::Wrapper& query)
{
    auto vo = GetChargeItemVO::createShared();
    return vo;
}
