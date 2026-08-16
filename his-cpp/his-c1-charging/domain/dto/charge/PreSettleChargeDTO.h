#pragma once

#ifndef _PRE_SETTLE_CHARGE_DTO_
#define _PRE_SETTLE_CHARGE_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
   预结算请求
*/
class PreSettleChargeRequestDTO : public oatpp::DTO
{
    DTO_INIT(PreSettleChargeRequestDTO, DTO);

    // 就诊主键
    API_DTO_FIELD(String, encounterId,
        ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeRequestDTO.encounterId.description"),
        true, "");

    // 用户选中的待收收费项 ID 列表
    API_DTO_FIELD(Vector<String>, chargeItemIds,
        ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeRequestDTO.chargeItemIds.description"),
        true, {});

    // 费用性质合同主键（医保结算时必传）
    API_DTO_FIELD(String, contractId,
        ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeRequestDTO.contractId.description"),
        false, "");

    // 医保类型编码
    API_DTO_FIELD(String, insuranceTypeCode,
        ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeRequestDTO.insuranceTypeCode.description"),
        false, "");

    // 个人医保编号
    API_DTO_FIELD(String, personalInsuranceNo,
        ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeRequestDTO.personalInsuranceNo.description"),
        false, "");

    // 社保卡号
    API_DTO_FIELD(String, insuranceCardNo,
        ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeRequestDTO.insuranceCardNo.description"),
        false, "");
};

/*
   预结算行预览（previews 子对象）
*/
class PreSettleChargeLinePreviewDTO : public oatpp::DTO
{
    DTO_INIT(PreSettleChargeLinePreviewDTO, DTO);

    // 收费明细主键
    API_DTO_FIELD(String, chargeItemId, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeLinePreviewDTO.chargeItemId.description"), true, "337209313107906560");

    // 项目展示名称
    API_DTO_FIELD(String, itemLabel, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeLinePreviewDTO.itemLabel.description"), true, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeLinePreviewDTO.itemLabel.value"));

    // 本行应收金额
    API_DTO_FIELD(Float64, lineAmount, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeLinePreviewDTO.lineAmount.description"), true, 7.97);

    // 费用性质展示（自费/省医保/市医保等）
    API_DTO_FIELD(String, feeNatureDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeLinePreviewDTO.feeNatureDisplay.description"), false, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeLinePreviewDTO.feeNatureDisplay.value"));
};

/*
   自费汇总（selfPayRollup 子对象）
*/
class OpSelfPayRollupDTO : public oatpp::DTO
{
    DTO_INIT(OpSelfPayRollupDTO, DTO);

    // 自费账户主键
    API_DTO_FIELD(String, accountId, ZH_WORDS_GETTER("lhfc.charging.charge.OpSelfPayRollupDTO.accountId.description"), false, "");

    // 自费收费项 ID 列表
    API_DTO_FIELD(Vector<String>, chargeItemIds, ZH_WORDS_GETTER("lhfc.charging.charge.OpSelfPayRollupDTO.chargeItemIds.description"), true, {"337206129186574336"});

    // 自费合计
    API_DTO_FIELD(Float64, totalAmount, ZH_WORDS_GETTER("lhfc.charging.charge.OpSelfPayRollupDTO.totalAmount.description"), false, 0.0);
};

/*
   医保分组（insuranceGroups 子对象）
*/
class OpInsurancePreSettleGroupDTO : public oatpp::DTO
{
    DTO_INIT(OpInsurancePreSettleGroupDTO, DTO);

    // 分组序号
    API_DTO_FIELD(Int32, groupIndex, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.groupIndex.description"), true, 0);

    // 费用性质合同主键
    API_DTO_FIELD(String, contractId, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.contractId.description"), false, "");

    // 记账账户主键
    API_DTO_FIELD(String, accountId, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.accountId.description"), false, "");

    // 该组的收费项 ID 列表
    API_DTO_FIELD(Vector<String>, chargeItemIds, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.chargeItemIds.description"), true, {"337206129186574336"});

    // 医疗类别 code
    API_DTO_FIELD(String, medType, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.medType.description"), false, "");

    // 医疗类别展示
    API_DTO_FIELD(String, medTypeDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.medTypeDisplay.description"), false, "");

    // 诊断编码
    API_DTO_FIELD(String, diseCodg, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.diseCodg.description"), false, "");

    // 诊断名称
    API_DTO_FIELD(String, diseName, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.diseName.description"), false, "");

    // 医疗费总额
    API_DTO_FIELD(Float64, medfeeSumamt, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.medfeeSumamt.description"), false, 0.0);

    // 统筹基金支付
    API_DTO_FIELD(Float64, fundPaySumamt, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.fundPaySumamt.description"), false, 0.0);

    // 个人账户支付
    API_DTO_FIELD(Float64, acctPay, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.acctPay.description"), false, 0.0);

    // 个人现金支付
    API_DTO_FIELD(Float64, psnCashPay, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.psnCashPay.description"), false, 0.0);

    // 医保收费批次号——步骤 2 的 ybChrgBchnos 来源
    API_DTO_FIELD(String, chrgBchno, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.chrgBchno.description"), false, "");

    // 医保预结算 ID
    API_DTO_FIELD(String, preSettleId, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.preSettleId.description"), false, "");

    // 就诊 ID（医保侧）
    API_DTO_FIELD(String, mdtrtId, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.mdtrtId.description"), false, "");
};

/*
   预结算响应
*/
class PreSettleChargeResponseDTO : public oatpp::DTO
{
    DTO_INIT(PreSettleChargeResponseDTO, DTO);

    // 支付草稿主键——步骤 2 正式结算的必传入参
    API_DTO_FIELD(String, paymentDraftId, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.paymentDraftId.description"), true, "337209423854309376");

    // 展示应收金额
    API_DTO_FIELD(Float64, displayAmount, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.displayAmount.description"), true, 17.31);

    // 是否已完成医保 2206 预结算
    API_DTO_FIELD(Boolean, insurancePreSettled, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.insurancePreSettled.description"), true, false);

    // 结算构成：MIXED / INSURANCE_ONLY / SELF_PAY_ONLY
    API_DTO_FIELD(String, settlementComposition, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.settlementComposition.description"), false, "SELF_PAY_ONLY");

    // 医保收费批次号（单组兼容）
    API_DTO_FIELD(String, ybChrgBchno, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.ybChrgBchno.description"), false, "");

    // 医保医疗费总额
    API_DTO_FIELD(Float64, ybMedfeeSumamt, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.ybMedfeeSumamt.description"), false, 0.0);

    // 医保统筹支付
    API_DTO_FIELD(Float64, ybFundPaySumamt, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.ybFundPaySumamt.description"), false, 0.0);

    // 医保个账支付
    API_DTO_FIELD(Float64, ybAcctPay, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.ybAcctPay.description"), false, 0.0);

    // 医保现金自付
    API_DTO_FIELD(Float64, ybPsnCashPay, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.ybPsnCashPay.description"), false, 0.0);

    // 行预览列表
    API_DTO_FIELD(Vector<Object<PreSettleChargeLinePreviewDTO>>, previews, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.previews.description"), true, {Object<PreSettleChargeLinePreviewDTO>(PreSettleChargeLinePreviewDTO::createShared())});

    // 自费汇总
    API_DTO_FIELD(Object<OpSelfPayRollupDTO>, selfPayRollup, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.selfPayRollup.description"), false, {});

    // 医保分组列表
    API_DTO_FIELD(Vector<Object<OpInsurancePreSettleGroupDTO>>, insuranceGroups, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.insuranceGroups.description"), false, {Object<OpInsurancePreSettleGroupDTO>(OpInsurancePreSettleGroupDTO::createShared())});
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRE_SETTLE_CHARGE_DTO_
