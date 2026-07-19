#include "ChargeController.h"

// ---- 功能1: 获取患者队列 ----
GetPatientQueueJsonVO::Wrapper ChargeController::execQueryPatientQueue(const GetPatientQueueQuery::Wrapper& query)
{
    auto vo = GetPatientQueueJsonVO::createShared();
    return vo;
}

// ---- 功能2: 获取患者缴费项 ----
GetChargeItemVO::Wrapper ChargeController::execGetChargeItems(const GetChargeItemsQuery::Wrapper& query)
{
    auto vo = GetChargeItemVO::createShared();
    return vo;
}

// ---- 功能3A: 预结算 ----
PreSettleChargeVO::Wrapper ChargeController::execPreSettleCharge(const PreSettleChargeRequestDTO::Wrapper& dto)
{
    auto vo = PreSettleChargeVO::createShared();
    return vo;
}

// ---- 功能3C: 正式结算 ----
ConfirmSettleChargeVO::Wrapper ChargeController::execConfirmSettleCharge(const ConfirmSettleChargeRequestDTO::Wrapper& dto)
{
    auto vo = ConfirmSettleChargeVO::createShared();
    return vo;
}

// ---- 功能4: 打印门诊收费小票 ----
PrintChargeReceiptVO::Wrapper ChargeController::execPrintChargeReceipt(const PrintChargeReceiptRequestDTO::Wrapper& dto)
{
    auto vo = PrintChargeReceiptVO::createShared();
    return vo;
}

// ---- 功能4B: 准备打印 ----
PreparePrintVO::Wrapper ChargeController::execPreparePrint(const PreparePrintRequestDTO::Wrapper& dto)
{
    auto vo = PreparePrintVO::createShared();
    return vo;
}

// ---- 功能4C: 确认打印 ----
ConfirmPrintVO::Wrapper ChargeController::execConfirmPrint(const ConfirmPrintRequestDTO::Wrapper& dto)
{
    auto vo = ConfirmPrintVO::createShared();
    return vo;
}
