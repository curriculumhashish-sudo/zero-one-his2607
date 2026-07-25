#include "SettlementController.h"
#include "service/charge/SettlementService.h"

// 预结算
PreSettleChargeVO::Wrapper SettlementController::execPreSettleCharge(const PreSettleChargeRequestDTO::Wrapper& dto)
{
    auto vo = PreSettleChargeVO::createShared();
    SettlementService service;
    auto resp = service.preSettle(dto);
    vo->success(resp);
    return vo;
}

// 正式结算
ConfirmSettleChargeVO::Wrapper SettlementController::execConfirmSettleCharge(const ConfirmSettleChargeRequestDTO::Wrapper& dto)
{
    auto vo = ConfirmSettleChargeVO::createShared();
    SettlementService service;
    auto resp = service.confirmSettle(dto);
    vo->success(resp);
    return vo;
}
