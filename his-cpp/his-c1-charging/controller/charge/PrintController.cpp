#include "PrintController.h"
#include "service/charge/PrintChargeReceiptService.h"

// 打印门诊收费小票
PrintChargeReceiptVO::Wrapper PrintController::execPrintChargeReceipt(const PrintChargeReceiptRequestDTO::Wrapper& dto)
{
    auto vo = PrintChargeReceiptVO::createShared();
    PrintChargeReceiptService service;
    auto resp = service.printChargeReceipt(dto);
    vo->success(resp);
    return vo;
}

// 准备打印（获取打印模板）
PreparePrintVO::Wrapper PrintController::execPreparePrint(const PreparePrintRequestDTO::Wrapper& dto)
{
    auto vo = PreparePrintVO::createShared();
    PrintChargeReceiptService service;
    auto resp = service.preparePrint(dto);
    vo->success(resp);
    return vo;
}

// 确认打印（审计落库）
ConfirmPrintVO::Wrapper PrintController::execConfirmPrint(const ConfirmPrintRequestDTO::Wrapper& dto)
{
    auto vo = ConfirmPrintVO::createShared();
    PrintChargeReceiptService service;
    auto resp = service.confirmPrint(dto);
    vo->success(resp);
    return vo;
}
