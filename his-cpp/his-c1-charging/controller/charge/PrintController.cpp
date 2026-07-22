#include "PrintController.h"

// 打印门诊收费小票
PrintChargeReceiptVO::Wrapper PrintController::execPrintChargeReceipt(const PrintChargeReceiptRequestDTO::Wrapper& dto)
{
    auto vo = PrintChargeReceiptVO::createShared();
    return vo;
}

// 准备打印（获取打印模板）
PreparePrintVO::Wrapper PrintController::execPreparePrint(const PreparePrintRequestDTO::Wrapper& dto)
{
    auto vo = PreparePrintVO::createShared();
    return vo;
}

// 确认打印（审计落库）
ConfirmPrintVO::Wrapper PrintController::execConfirmPrint(const ConfirmPrintRequestDTO::Wrapper& dto)
{
    auto vo = ConfirmPrintVO::createShared();
    return vo;
}
