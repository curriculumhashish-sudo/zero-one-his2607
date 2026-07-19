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
    API_DTO_FIELD_DEFAULT(String, chargeItemId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.chargeItemId"));

    // 来源业务表名
    API_DTO_FIELD_DEFAULT(String, serviceTable, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.serviceTable"));

    // 来源业务主键
    API_DTO_FIELD_DEFAULT(String, serviceId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.serviceId"));

    // 收费项目展示名称
    API_DTO_FIELD_DEFAULT(String, itemName, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.itemName"));

    // 收费明细业务流水号
    API_DTO_FIELD_DEFAULT(String, busNo, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.busNo"));

    // 单价
    API_DTO_FIELD_DEFAULT(Float64, unitPrice, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.unitPrice"));

    // 数量值
    API_DTO_FIELD_DEFAULT(Float64, quantityValue, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.quantityValue"));

    // 数量单位 code
    API_DTO_FIELD_DEFAULT(String, quantityUnit, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.quantityUnit"));

    // 数量展示文本（不含单位）
    API_DTO_FIELD_DEFAULT(String, quantityDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.quantityDisplay"));

    // 费用分类编码
    API_DTO_FIELD_DEFAULT(String, chargeCategoryCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.chargeCategoryCode"));

    // 费用分类展示名
    API_DTO_FIELD_DEFAULT(String, chargeCategoryDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.chargeCategoryDisplay"));

    // 收费状态持久化码
    API_DTO_FIELD_DEFAULT(String, statusCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.statusCode"));

    // 收费状态展示文案
    API_DTO_FIELD_DEFAULT(String, statusText, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.statusText"));

    // 行总价
    API_DTO_FIELD_DEFAULT(Float64, totalPrice, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.totalPrice"));

    // 收款人系统用户主键
    API_DTO_FIELD_DEFAULT(String, cashierUserId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.cashierUserId"));

    // 收款人姓名
    API_DTO_FIELD_DEFAULT(String, cashierName, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.cashierName"));

    // 折扣率展示文本
    API_DTO_FIELD_DEFAULT(String, discountRateDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.discountRateDisplay"));

    // 记账账户主键
    API_DTO_FIELD_DEFAULT(String, accountId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.accountId"));

    // 费用性质合同名称（经 account_id → contract_no 关联）
    API_DTO_FIELD_DEFAULT(String, contractName, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.contractName"));

    // 费用性质合同主键
    API_DTO_FIELD_DEFAULT(String, contractId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.contractId"));

    // 费用性质展示
    API_DTO_FIELD_DEFAULT(String, feeNatureDisplay, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.feeNatureDisplay"));

    // 医疗类别
    API_DTO_FIELD_DEFAULT(String, medTypeCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.medTypeCode"));

    // 医保目录编码
    API_DTO_FIELD_DEFAULT(String, ybNo, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.ybNo"));

    // 医保编码展示
    API_DTO_FIELD_DEFAULT(String, medicalInsuranceCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.medicalInsuranceCode"));
};

/*
   获取患者缴费项响应
*/
class GetChargeItemDTO : public oatpp::DTO
{
    DTO_INIT(GetChargeItemDTO, DTO);

    // 就诊主键
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemDTO.encounterId"));

    // 待收费明细列表
    API_DTO_FIELD_DEFAULT(Vector<Object<GetChargeItemRowDTO>>, items, ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemDTO.items"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_GET_CHARGE_ITEM_DTO_
