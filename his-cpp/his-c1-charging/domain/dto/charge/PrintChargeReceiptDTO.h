#pragma once

#ifndef _PRINT_CHARGE_RECEIPT_DTO_
#define _PRINT_CHARGE_RECEIPT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
   打印门诊收费小票请求
*/
class PrintChargeReceiptRequestDTO : public oatpp::DTO
{
    DTO_INIT(PrintChargeReceiptRequestDTO, DTO);

    // 就诊主键（与 paymentId 至少传一个）
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("lhfc.charging.charge.PrintChargeReceiptRequestDTO.encounterId"));

    // 支付流水主键（场景 A/B 都传；为空时自动取该就诊最近一次成功支付）
    API_DTO_FIELD_DEFAULT(String, paymentId, ZH_WORDS_GETTER("lhfc.charging.charge.PrintChargeReceiptRequestDTO.paymentId"));
};

/*
   收费项目逐行（chargeItemLines 子对象）
*/
class ChargeReceiptChargeLineDTO : public oatpp::DTO
{
    DTO_INIT(ChargeReceiptChargeLineDTO, DTO);

    // 项目名称
    API_DTO_FIELD_DEFAULT(String, itemName, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeReceiptChargeLineDTO.itemName"));

    // 数量展示（如 "1 次"）
    API_DTO_FIELD_DEFAULT(String, quantityText, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeReceiptChargeLineDTO.quantityText"));

    // 金额展示（保留 2 位小数）
    API_DTO_FIELD_DEFAULT(String, amountText, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeReceiptChargeLineDTO.amountText"));

    // 费用类别（字典翻译，如 "西药费"/"检查费"）
    API_DTO_FIELD_DEFAULT(String, chargeCategoryText, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeReceiptChargeLineDTO.chargeCategoryText"));
};

/*
   小票打印数据（slip 子对象）
*/
class SlipDTO : public oatpp::DTO
{
    DTO_INIT(SlipDTO, DTO);

    // --- 患者和机构信息 ---
    // 就诊 ID
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.encounterId"));

    // 患者姓名
    API_DTO_FIELD_DEFAULT(String, patientName, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.patientName"));

    // 机构名称（typeEnum=HOSPITAL，statusEnum=ACTIVE）
    API_DTO_FIELD_DEFAULT(String, hospitalName, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.hospitalName"));

    // 性别展示
    API_DTO_FIELD_DEFAULT(String, genderEnumText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.genderEnumText"));

    // 年龄文本（计算值）
    API_DTO_FIELD_DEFAULT(String, ageText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.ageText"));

    // --- 费用字段 ---
    // 费用性质（取第一项收费行的合同名称；无则取自费账户关联合同）
    API_DTO_FIELD_DEFAULT(String, feeNatureText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.feeNatureText"));

    // --- 费用分解字段 ---
    // 合计金额（优先取支付流水 displayAmount，否则累加收费行）
    API_DTO_FIELD_DEFAULT(String, totalAmountText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.totalAmountText"));

    // 统筹支付
    API_DTO_FIELD_DEFAULT(String, poolingPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.poolingPayText"));

    // 基金支付
    API_DTO_FIELD_DEFAULT(String, fundPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.fundPayText"));

    // 个人账户支付
    API_DTO_FIELD_DEFAULT(String, accountPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.accountPayText"));

    // 起付线（当前固定空字符串）
    API_DTO_FIELD_DEFAULT(String, deductibleLineText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.deductibleLineText"));

    // 现金支付
    API_DTO_FIELD_DEFAULT(String, cashPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.cashPayText"));

    // 微信支付
    API_DTO_FIELD_DEFAULT(String, wechatPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.wechatPayText"));

    // 支付宝支付
    API_DTO_FIELD_DEFAULT(String, alipayPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.alipayPayText"));

    // 个账支付后余额
    API_DTO_FIELD_DEFAULT(String, accountBalanceText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.accountBalanceText"));

    // --- 操作人&时间字段 ---
    // 收费员姓名（由 fin_payment_reconciliation.enterer_id 查找）
    API_DTO_FIELD_DEFAULT(String, cashierName, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.cashierName"));

    // 收费时间格式
    API_DTO_FIELD_DEFAULT(String, chargeTimeText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.chargeTimeText"));

    // --- 收费项目行 ---
    API_DTO_FIELD_DEFAULT(Vector<Object<ChargeReceiptChargeLineDTO>>, chargeItemLines, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.chargeItemLines"));
};

/*
   打印门诊收费小票响应
*/
class PrintChargeReceiptResponseDTO : public oatpp::DTO
{
    DTO_INIT(PrintChargeReceiptResponseDTO, DTO);

    // 小票打印数据
    API_DTO_FIELD_DEFAULT(Object<SlipDTO>, slip, ZH_WORDS_GETTER("lhfc.charging.charge.PrintChargeReceiptResponseDTO.slip"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_CHARGE_RECEIPT_DTO_
