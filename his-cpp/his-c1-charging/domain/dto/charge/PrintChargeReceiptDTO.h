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
    API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("lhfc.charging.charge.PrintChargeReceiptRequestDTO.encounterId"), false, "");

    // 支付流水主键（场景 A/B 都传；为空时自动取该就诊最近一次成功支付）
    API_DTO_FIELD(String, paymentId, ZH_WORDS_GETTER("lhfc.charging.charge.PrintChargeReceiptRequestDTO.paymentId"), false, "");
};

/*
   收费项目逐行（chargeItemLines 子对象）
*/
class ChargeReceiptChargeLineDTO : public oatpp::DTO
{
    DTO_INIT(ChargeReceiptChargeLineDTO, DTO);

    // 项目名称
    API_DTO_FIELD(String, itemName, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeReceiptChargeLineDTO.itemName"), true, "注射用头孢呋辛钠");

    // 数量展示（如 "1 次"）
    API_DTO_FIELD(String, quantityText, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeReceiptChargeLineDTO.quantityText"), true, "1 瓶");

    // 金额展示（保留 2 位小数）
    API_DTO_FIELD(String, amountText, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeReceiptChargeLineDTO.amountText"), true, "7.97");

    // 费用类别（字典翻译，如 "西药费"/"检查费"）
    API_DTO_FIELD(String, chargeCategoryText, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeReceiptChargeLineDTO.chargeCategoryText"), true, "1001");
};

/*
   小票打印数据（slip 子对象）
*/
class SlipDTO : public oatpp::DTO
{
    DTO_INIT(SlipDTO, DTO);

    // --- 患者和机构信息 ---
    // 就诊 ID
    API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.encounterId"), false, "337209205188464640");

    // 患者姓名
    API_DTO_FIELD(String, patientName, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.patientName"), true, "李先生");

    // 机构名称（typeEnum=HOSPITAL，statusEnum=ACTIVE）
    API_DTO_FIELD(String, hospitalName, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.hospitalName"), true, "测试医院");

    // 性别展示
    API_DTO_FIELD(String, genderEnumText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.genderEnumText"), true, "男性");

    // 年龄文本（计算值）
    API_DTO_FIELD(String, ageText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.ageText"), true, "25岁");

    // --- 费用字段 ---
    // 费用性质（取第一项收费行的合同名称；无则取自费账户关联合同）
    API_DTO_FIELD(String, feeNatureText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.feeNatureText"), true, "自费");

    // --- 费用分解字段 ---
    // 合计金额（优先取支付流水 displayAmount，否则累加收费行）
    API_DTO_FIELD(String, totalAmountText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.totalAmountText"), true, "17.31");

    // 统筹支付
    API_DTO_FIELD(String, poolingPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.poolingPayText"), true, "");

    // 基金支付
    API_DTO_FIELD(String, fundPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.fundPayText"), true, "");

    // 个人账户支付
    API_DTO_FIELD(String, accountPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.accountPayText"), true, "");

    // 起付线（当前固定空字符串）
    API_DTO_FIELD(String, deductibleLineText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.deductibleLineText"), true, "");

    // 现金支付
    API_DTO_FIELD(String, cashPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.cashPayText"), true, "17.31");

    // 微信支付
    API_DTO_FIELD(String, wechatPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.wechatPayText"), true, "");

    // 支付宝支付
    API_DTO_FIELD(String, alipayPayText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.alipayPayText"), true, "");

    // 个账支付后余额
    API_DTO_FIELD(String, accountBalanceText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.accountBalanceText"), true, "");

    // --- 操作人&时间字段 ---
    // 收费员姓名（由 fin_payment_reconciliation.enterer_id 查找）
    API_DTO_FIELD(String, cashierName, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.cashierName"), true, "admin");

    // 收费时间格式
    API_DTO_FIELD(String, chargeTimeText, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.chargeTimeText"), true, "2026-07-19 20:31:05");

    // --- 收费项目行 ---
    API_DTO_FIELD(Vector<Object<ChargeReceiptChargeLineDTO>>, chargeItemLines, ZH_WORDS_GETTER("lhfc.charging.charge.SlipDTO.chargeItemLines"), true, {});
};

/*
   打印门诊收费小票响应
*/
class PrintChargeReceiptResponseDTO : public oatpp::DTO
{
    DTO_INIT(PrintChargeReceiptResponseDTO, DTO);

    // 小票打印数据
    API_DTO_FIELD(Object<SlipDTO>, slip, ZH_WORDS_GETTER("lhfc.charging.charge.PrintChargeReceiptResponseDTO.slip"), true, {});
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_CHARGE_RECEIPT_DTO_
