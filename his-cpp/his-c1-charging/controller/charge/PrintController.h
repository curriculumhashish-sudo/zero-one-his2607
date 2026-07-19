#pragma once
#ifndef _PRINT_CONTROLLER_
#define _PRINT_CONTROLLER_
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/charge/PrintChargeReceiptDTO.h"
#include "domain/vo/charge/ChargePrintVO.h"
#include "domain/dto/charge/PreparePrintDTO.h"
#include "domain/dto/charge/ConfirmPrintDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("lhfc.charging.charge.tags.t4")
/*
   打印控制器
   负责门诊收费小票打印流程：打印小票、准备打印模板、确认打印审计落库
*/
class PrintController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(PrintController);
public:
    // 打印门诊收费小票
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.PrintChargeReceipt.summary"),
        printChargeReceipt,
        PrintChargeReceiptVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/charging/charge/print-charge-receipt", printChargeReceipt,
        BODY_DTO(PrintChargeReceiptRequestDTO::Wrapper, dto), execPrintChargeReceipt(dto)
    );

    // 准备打印（获取打印模板）
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrint.summary"),
        preparePrint,
        PreparePrintVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/charging/charge/prepare-print", preparePrint,
        BODY_DTO(PreparePrintRequestDTO::Wrapper, dto), execPreparePrint(dto)
    );

    // 确认打印（审计落库）
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrint.summary"),
        confirmPrint,
        ConfirmPrintVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/charging/charge/confirm-print", confirmPrint,
        BODY_DTO(ConfirmPrintRequestDTO::Wrapper, dto), execConfirmPrint(dto)
    );

private:
    // 打印门诊收费小票
    PrintChargeReceiptVO::Wrapper execPrintChargeReceipt(const PrintChargeReceiptRequestDTO::Wrapper& dto);
    // 准备打印
    PreparePrintVO::Wrapper execPreparePrint(const PreparePrintRequestDTO::Wrapper& dto);
    // 确认打印
    ConfirmPrintVO::Wrapper execConfirmPrint(const ConfirmPrintRequestDTO::Wrapper& dto);
};
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_PRINT_CONTROLLER_
