#pragma once

#ifndef _CONFIRM_SETTLE_CHARGE_DTO_
#define _CONFIRM_SETTLE_CHARGE_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
   支付明细提交（paymentDetails 子对象）
*/
class PaymentDetailSubmitDTO : public oatpp::DTO
{
    DTO_INIT(PaymentDetailSubmitDTO, DTO);

    // 支付渠道编码（CASH / WECHAT / ALIPAY / SCAN_PAY / INSURANCE_ACCT / INSURANCE_FUND）
    API_DTO_FIELD(String, payChannelCode, ZH_WORDS_GETTER("lhfc.charging.charge.PaymentDetailSubmitDTO.payChannelCode.description"), true, "CASH");

    // 金额
    API_DTO_FIELD(Float64, amount, ZH_WORDS_GETTER("lhfc.charging.charge.PaymentDetailSubmitDTO.amount.description"), true, 17.31);
};

/*
   银联被扫回执（clientScanResult 子对象，仅扫码支付时传入）
*/
class ClientScanResultDTO : public oatpp::DTO
{
    DTO_INIT(ClientScanResultDTO, DTO);

    // 预留字段，暂不展开
};

/*
   正式结算请求
*/
class ConfirmSettleChargeRequestDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmSettleChargeRequestDTO, DTO);

    // 操作人载荷（由Controller注入，Service读取）
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

    // 支付草稿主键——核心防重/防换单锚点
    API_DTO_FIELD(String, paymentDraftId,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeRequestDTO.paymentDraftId.description"),
        true, "");

    // 就诊主键
    API_DTO_FIELD(String, encounterId,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeRequestDTO.encounterId.description"),
        true, "");

    // 收费项 ID 列表，必须与草稿绑定的集合完全一致
    API_DTO_FIELD(Vector<String>, chargeItemIds,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeRequestDTO.chargeItemIds.description"),
        true, {});

    // 支付渠道 + 实付金额
    API_DTO_FIELD(Vector<Object<PaymentDetailSubmitDTO>>, paymentDetails,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeRequestDTO.paymentDetails.description"),
        true, {Object<PaymentDetailSubmitDTO>(PaymentDetailSubmitDTO::createShared())});

    // 账单日期（客户端生成，格式 YYYY-MM-DD HH:mm:ss）
    API_DTO_FIELD(String, billDate,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeRequestDTO.billDate.description"),
        true, "");

    // 结算结果枚举
    API_DTO_FIELD(String, outcomeEnum,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeRequestDTO.outcomeEnum.description"),
        true, "1");

    // 医保收费批次号（单组）
    API_DTO_FIELD(String, ybChrgBchno,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeRequestDTO.ybChrgBchno.description"),
        false, "");

    // 医保收费批次号列表（多组）
    API_DTO_FIELD(Vector<String>, ybChrgBchnos,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeRequestDTO.ybChrgBchnos.description"),
        false, {});

    // 银联被扫回执（仅扫码支付时传入）
    API_DTO_FIELD(Object<ClientScanResultDTO>, clientScanResult,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeRequestDTO.clientScanResult.description"),
        false, {});
};

/*
   正式结算响应
*/
class ConfirmSettleChargeResponseDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmSettleChargeResponseDTO, DTO);

    // 正式支付主键——打印小票的必传入参
    API_DTO_FIELD(String, paymentId, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeResponseDTO.paymentId.description"), true, "337209423854309376");

    // 票据草稿主键
    API_DTO_FIELD(String, invoiceId, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeResponseDTO.invoiceId.description"), true, "337209733486219264");

    // 发票状态 DRAFT / ISSUED / CANCELLED
    API_DTO_FIELD(String, invoiceStatus, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeResponseDTO.invoiceStatus.description"), true, "DRAFT");

    // 成功语义码
    API_DTO_FIELD(String, successMessageCode, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeResponseDTO.successMessageCode.description"), false, "EX-OPC-410");

    // 医保结算 ID（多组逗号分隔）
    API_DTO_FIELD(String, ybSetlId, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleChargeResponseDTO.ybSetlId.description"), false, "");
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CONFIRM_SETTLE_CHARGE_DTO_
