#include "SettlementController.h"

// 预结算——生成草稿，锁定收费项
PreSettleChargeVO::Wrapper SettlementController::execPreSettleCharge(const PreSettleChargeRequestDTO::Wrapper& dto)
{
    auto vo = PreSettleChargeVO::createShared();
    return vo;
}

// 正式结算——落账收费
ConfirmSettleChargeVO::Wrapper SettlementController::execConfirmSettleCharge(const ConfirmSettleChargeRequestDTO::Wrapper& dto)
{
    auto vo = ConfirmSettleChargeVO::createShared();
    return vo;
}
