#pragma once
#ifndef _CHARGE_CONTROLLER_
#define _CHARGE_CONTROLLER_
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/charge/GetPatientQueueDTO.h"
#include "domain/vo/charge/GetPatientQueueJsonVO.h"
#include "domain/query/charge/GetPatientQueueQuery.h"
#include "domain/query/charge/GetChargeItemsQuery.h"
#include "domain/dto/charge/GetChargeItemDTO.h"
#include "domain/vo/charge/GetChargeItemVO.h"
#include "domain/dto/charge/PreSettleChargeDTO.h"
#include "domain/vo/charge/SettleChargeVO.h"
#include "domain/dto/charge/ConfirmSettleChargeDTO.h"
#include "domain/dto/charge/PrintChargeReceiptDTO.h"
#include "domain/vo/charge/ChargePrintVO.h"
#include "domain/dto/charge/PreparePrintDTO.h"
#include "domain/dto/charge/ConfirmPrintDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("lhfc.charging.charge.tags.t1")
/*
   门诊收费控制器
*/
class ChargeController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(ChargeController);
public:
    // ---- 功能1: 获取患者队列 ----
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueue.summary"),
        queryPatientQueue,
        GetPatientQueueQuery,
        GetPatientQueueJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/charge/query-patient-queue", queryPatientQueue,
        GetPatientQueueQuery, execQueryPatientQueue(query)
    );

    // ---- 功能2: 获取患者缴费项 ----
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItems.summary"),
        getChargeItems,
        GetChargeItemsQuery,
        GetChargeItemVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/charge/get-charge-items", getChargeItems,
        GetChargeItemsQuery, execGetChargeItems(query)
    );

    // ---- 功能3A: 预结算 ----
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleCharge.summary"),
        preSettleCharge,
        PreSettleChargeVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/charge/pre-settle-charge", preSettleCharge,
        BODY_DTO(PreSettleChargeRequestDTO::Wrapper, dto), execPreSettleCharge(dto)
    );

    // ---- 功能3C: 正式结算 ----
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleCharge.summary"),
        confirmSettleCharge,
        ConfirmSettleChargeVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/charge/confirm-settle-charge", confirmSettleCharge,
        BODY_DTO(ConfirmSettleChargeRequestDTO::Wrapper, dto), execConfirmSettleCharge(dto)
    );

    // ---- 功能4: 打印门诊收费小票 ----
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.PrintChargeReceipt.summary"),
        printChargeReceipt,
        PrintChargeReceiptVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/charge/print-charge-receipt", printChargeReceipt,
        BODY_DTO(PrintChargeReceiptRequestDTO::Wrapper, dto), execPrintChargeReceipt(dto)
    );

    // ---- 功能4B: 准备打印 ----
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrint.summary"),
        preparePrint,
        PreparePrintVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/charge/prepare-print", preparePrint,
        BODY_DTO(PreparePrintRequestDTO::Wrapper, dto), execPreparePrint(dto)
    );

    // ---- 功能4C: 确认打印 ----
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrint.summary"),
        confirmPrint,
        ConfirmPrintVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/charge/confirm-print", confirmPrint,
        BODY_DTO(ConfirmPrintRequestDTO::Wrapper, dto), execConfirmPrint(dto)
    );

private:
    // 功能1
    GetPatientQueueJsonVO::Wrapper execQueryPatientQueue(const GetPatientQueueQuery::Wrapper& query);
    // 功能2
    GetChargeItemVO::Wrapper execGetChargeItems(const GetChargeItemsQuery::Wrapper& query);
    // 功能3A
    PreSettleChargeVO::Wrapper execPreSettleCharge(const PreSettleChargeRequestDTO::Wrapper& dto);
    // 功能3C
    ConfirmSettleChargeVO::Wrapper execConfirmSettleCharge(const ConfirmSettleChargeRequestDTO::Wrapper& dto);
    // 功能4
    PrintChargeReceiptVO::Wrapper execPrintChargeReceipt(const PrintChargeReceiptRequestDTO::Wrapper& dto);
    // 功能4B
    PreparePrintVO::Wrapper execPreparePrint(const PreparePrintRequestDTO::Wrapper& dto);
    // 功能4C
    ConfirmPrintVO::Wrapper execConfirmPrint(const ConfirmPrintRequestDTO::Wrapper& dto);
};
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif //!_CHARGE_CONTROLLER_