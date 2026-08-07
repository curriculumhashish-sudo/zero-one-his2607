#pragma once

#ifndef _GET_CHARGE_ITEM_DTO_
#define _GET_CHARGE_ITEM_DTO_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)
/*
   待收费明细行（items 子对象）
*/
class GetChargeItemRowDTO : public oatpp::DTO
{
    DTO_INIT(GetChargeItemRowDTO, DTO);

    // 收费明细主键
    API_DTO_FIELD(String, chargeItemId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.chargeItemId.description"), true, "337053386882945024");

    // 来源业务表名
    API_DTO_FIELD(String, serviceTable, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.serviceTable.description"), false, "wkf_device_request");

    // 来源业务主键
    API_DTO_FIELD(String, serviceId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.serviceId.description"), false, "337053366783840256");

    // 收费项目展示名称
    API_DTO_FIELD(String, itemName, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.itemName.description"), false, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.itemName.value"));

    // 收费明细业务流水号
    API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.busNo.description"), false, "CI2026-07-190014");

    // 单价
    API_DTO_FIELD(Float64, unitPrice, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.unitPrice.description"), false, 0.35);

    // 数量值
    API_DTO_FIELD(Float64, quantityValue, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.quantityValue.description"), false, 1.0);

    // 数量单位 code
    API_DTO_FIELD(String, quantityUnit, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.quantityUnit.description"), false, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.quantityUnit.value"));

    // 数量展示文本（不含单位）
    API_DTO_FIELD(String, quantityDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.quantityDisplay.description"), false, "1");

    // 费用分类编码
    API_DTO_FIELD(String, chargeCategoryCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.chargeCategoryCode.description"), false, "11");

    // 费用分类展示名
    API_DTO_FIELD(String, chargeCategoryDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.chargeCategoryDisplay.description"), false, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.chargeCategoryDisplay.value"));

    // 收费状态持久化码
    API_DTO_FIELD(String, statusCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.statusCode.description"), true, "SETTLED");

    // 收费状态展示文案
    API_DTO_FIELD(String, statusText, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.statusText.description"), false, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.statusText.value"));

    // 行总价
    API_DTO_FIELD(Float64, totalPrice, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.totalPrice.description"), false, 0.35);

    // 行应收金额（= totalPrice）
    API_DTO_FIELD(Float64, amount, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.amount.description"), false, 0.0);

    // 医保支付金额（无医保时为 0.0）
    API_DTO_FIELD(Float64, insuranceAmount, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.insuranceAmount.description"), false, 0.0);

    // 自费支付金额（= totalPrice - insuranceAmount）
    API_DTO_FIELD(Float64, selfPayAmount, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.selfPayAmount.description"), false, 0.0);

    // 收款人系统用户主键
    API_DTO_FIELD(String, cashierUserId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.cashierUserId.description"), false, "308791351298756608");

    // 收款人姓名
    API_DTO_FIELD(String, cashierName, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.cashierName.description"), false, "admin");

    // 折扣率展示文本
    API_DTO_FIELD(String, discountRateDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.discountRateDisplay.description"), false, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.discountRateDisplay.value"));

    // 记账账户主键
    API_DTO_FIELD(String, accountId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.accountId.description"), false, "337053213544943616");

    // 费用性质合同名称（经 account_id → contract_no 关联）
    API_DTO_FIELD(String, contractName, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.contractName.description"), false, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.contractName.value"));

    // 费用性质合同主键
    API_DTO_FIELD(String, contractId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.contractId.description"), false, "3100000000000000001");

    // 费用性质展示
    API_DTO_FIELD(String, feeNatureDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.feeNatureDisplay.description"), false, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.feeNatureDisplay.value"));

    // 医疗类别
    API_DTO_FIELD(String, medTypeCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.medTypeCode.description"), false, "");

    // 医保目录编码
    API_DTO_FIELD(String, ybNo, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.ybNo.description"), false, "");

    // 医保编码展示
    API_DTO_FIELD(String, medicalInsuranceCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.medicalInsuranceCode.description"), false, "");
};

/*
   获取患者缴费项响应
*/
class GetChargeItemDTO : public oatpp::DTO
{
    DTO_INIT(GetChargeItemDTO, DTO);

    // 就诊主键
    API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemDTO.encounterId.description"), true, "337053213532360704");

    // 待收费明细列表
    API_DTO_FIELD(Vector<Object<GetChargeItemRowDTO>>, items, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemDTO.items.description"), true, {Object<GetChargeItemRowDTO>(GetChargeItemRowDTO::createShared())});
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_GET_CHARGE_ITEM_DTO_
