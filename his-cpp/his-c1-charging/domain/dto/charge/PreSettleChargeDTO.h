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
        ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeRequestDTO.encounterId"),
        true, "");

    // 用户选中的待收收费项 ID 列表
    API_DTO_FIELD(Vector<String>, chargeItemIds,
        ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeRequestDTO.chargeItemIds"),
        true, {});
};

/*
   预结算行预览（previews 子对象）
*/
class PreSettleChargeLinePreviewDTO : public oatpp::DTO
{
    DTO_INIT(PreSettleChargeLinePreviewDTO, DTO);

    // 收费明细主键
    API_DTO_FIELD_DEFAULT(String, chargeItemId, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeLinePreviewDTO.chargeItemId"));

    // 项目展示名称
    API_DTO_FIELD_DEFAULT(String, itemLabel, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeLinePreviewDTO.itemLabel"));

    // 本行应收金额
    API_DTO_FIELD_DEFAULT(Float64, lineAmount, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeLinePreviewDTO.lineAmount"));

    // 费用性质展示（自费/省医保/市医保等）
    API_DTO_FIELD_DEFAULT(String, feeNatureDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeLinePreviewDTO.feeNatureDisplay"));
};

/*
   自费汇总（selfPayRollup 子对象）
*/
class OpSelfPayRollupDTO : public oatpp::DTO
{
    DTO_INIT(OpSelfPayRollupDTO, DTO);

    // 自费账户主键
    API_DTO_FIELD_DEFAULT(String, accountId, ZH_WORDS_GETTER("lhfc.charging.charge.OpSelfPayRollupDTO.accountId"));

    // 自费收费项 ID 列表
    API_DTO_FIELD_DEFAULT(Vector<String>, chargeItemIds, ZH_WORDS_GETTER("lhfc.charging.charge.OpSelfPayRollupDTO.chargeItemIds"));

    // 自费合计
    API_DTO_FIELD_DEFAULT(Float64, totalAmount, ZH_WORDS_GETTER("lhfc.charging.charge.OpSelfPayRollupDTO.totalAmount"));
};

/*
   医保分组（insuranceGroups 子对象）
*/
class OpInsurancePreSettleGroupDTO : public oatpp::DTO
{
    DTO_INIT(OpInsurancePreSettleGroupDTO, DTO);

    // 分组序号
    API_DTO_FIELD_DEFAULT(Int32, groupIndex, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.groupIndex"));

    // 费用性质合同主键
    API_DTO_FIELD_DEFAULT(String, contractId, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.contractId"));

    // 记账账户主键
    API_DTO_FIELD_DEFAULT(String, accountId, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.accountId"));

    // 该组的收费项 ID 列表
    API_DTO_FIELD_DEFAULT(Vector<String>, chargeItemIds, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.chargeItemIds"));

    // 医疗类别 code
    API_DTO_FIELD_DEFAULT(String, medType, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.medType"));

    // 医疗类别展示
    API_DTO_FIELD_DEFAULT(String, medTypeDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.medTypeDisplay"));

    // 诊断编码
    API_DTO_FIELD_DEFAULT(String, diseCodg, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.diseCodg"));

    // 诊断名称
    API_DTO_FIELD_DEFAULT(String, diseName, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.diseName"));

    // 医疗费总额
    API_DTO_FIELD_DEFAULT(Float64, medfeeSumamt, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.medfeeSumamt"));

    // 统筹基金支付
    API_DTO_FIELD_DEFAULT(Float64, fundPaySumamt, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.fundPaySumamt"));

    // 个人账户支付
    API_DTO_FIELD_DEFAULT(Float64, acctPay, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.acctPay"));

    // 个人现金支付
    API_DTO_FIELD_DEFAULT(Float64, psnCashPay, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.psnCashPay"));

    // 医保收费批次号——步骤 2 的 ybChrgBchnos 来源
    API_DTO_FIELD_DEFAULT(String, chrgBchno, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.chrgBchno"));

    // 医保预结算 ID
    API_DTO_FIELD_DEFAULT(String, preSettleId, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.preSettleId"));

    // 就诊 ID（医保侧）
    API_DTO_FIELD_DEFAULT(String, mdtrtId, ZH_WORDS_GETTER("lhfc.charging.charge.OpInsurancePreSettleGroupDTO.mdtrtId"));
};

/*
   预结算响应
*/
class PreSettleChargeResponseDTO : public oatpp::DTO
{
    DTO_INIT(PreSettleChargeResponseDTO, DTO);

    // 支付草稿主键——步骤 2 正式结算的必传入参
    API_DTO_FIELD_DEFAULT(String, paymentDraftId, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.paymentDraftId"));

    // 展示应收金额
    API_DTO_FIELD_DEFAULT(Float64, displayAmount, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.displayAmount"));

    // 是否已完成医保 2206 预结算
    API_DTO_FIELD_DEFAULT(Boolean, insurancePreSettled, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.insurancePreSettled"));

    // 结算构成：MIXED / INSURANCE_ONLY / SELF_PAY_ONLY
    API_DTO_FIELD_DEFAULT(String, settlementComposition, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.settlementComposition"));

    // 医保收费批次号（单组兼容）
    API_DTO_FIELD_DEFAULT(String, ybChrgBchno, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.ybChrgBchno"));

    // 医保医疗费总额
    API_DTO_FIELD_DEFAULT(Float64, ybMedfeeSumamt, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.ybMedfeeSumamt"));

    // 医保统筹支付
    API_DTO_FIELD_DEFAULT(Float64, ybFundPaySumamt, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.ybFundPaySumamt"));

    // 医保个账支付
    API_DTO_FIELD_DEFAULT(Float64, ybAcctPay, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.ybAcctPay"));

    // 医保现金自付
    API_DTO_FIELD_DEFAULT(Float64, ybPsnCashPay, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.ybPsnCashPay"));

    // 行预览列表
    API_DTO_FIELD_DEFAULT(Vector<Object<PreSettleChargeLinePreviewDTO>>, previews, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.previews"));

    // 自费汇总
    API_DTO_FIELD_DEFAULT(Object<OpSelfPayRollupDTO>, selfPayRollup, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.selfPayRollup"));

    // 医保分组列表
    API_DTO_FIELD_DEFAULT(Vector<Object<OpInsurancePreSettleGroupDTO>>, insuranceGroups, ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleChargeResponseDTO.insuranceGroups"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRE_SETTLE_CHARGE_DTO_
