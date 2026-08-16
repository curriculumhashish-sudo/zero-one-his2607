#pragma once

#ifndef _PRINT_CHARGE_RECEIPT_SERVICE_H_
#define _PRINT_CHARGE_RECEIPT_SERVICE_H_

#include "domain/vo/charge/ChargePrintVO.h"
#include "domain/dto/charge/PrintChargeReceiptDTO.h"
#include "domain/dto/charge/PreparePrintDTO.h"
#include "domain/dto/charge/ConfirmPrintDTO.h"

/**
 * 打印服务（接口4A打印小票 + 4B准备打印 + 4C确认打印）
 */
class PrintChargeReceiptService
{
public:
    // 4A: 获取小票打印数据
    PrintChargeReceiptResponseDTO::Wrapper printChargeReceipt(const PrintChargeReceiptRequestDTO::Wrapper& dto);

    // 4B: 获取打印模板
    PreparePrintResponseDTO::Wrapper preparePrint(const PreparePrintRequestDTO::Wrapper& dto);

    // 4C: 确认打印审计落库
    ConfirmPrintResponseDTO::Wrapper confirmPrint(const ConfirmPrintRequestDTO::Wrapper& dto);
};

#endif // !_PRINT_CHARGE_RECEIPT_SERVICE_H_
