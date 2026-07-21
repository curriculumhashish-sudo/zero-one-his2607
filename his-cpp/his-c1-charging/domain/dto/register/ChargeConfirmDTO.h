#pragma once

#ifndef _CHARGE_CONFIRM_DTO_
#define _CHARGE_CONFIRM_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 定义确认收费DTO
 */
class ChargeConfirmDTO : public oatpp::DTO {
    DTO_INIT(ChargeConfirmDTO, DTO);

    // 就诊号字段
    DTO_FIELD(String, visitNumber);
    DTO_FIELD_INFO(visitNumber) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.visitNumber");
    }

    // 患者姓名字段
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.patientName");
    }

    // 联系电话字段
    DTO_FIELD(String, phone);
    DTO_FIELD_INFO(phone) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.phone");
    }

    // 应收金额字段
    DTO_FIELD(Float64, amount);
    DTO_FIELD_INFO(amount) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.amount");
    }

    // 支付草稿号字段
    DTO_FIELD(String, paymentDraft);
    DTO_FIELD_INFO(paymentDraft) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.paymentDraft");
    }

    // 挂号类型字段
    DTO_FIELD(String, registrationType);
    DTO_FIELD_INFO(registrationType) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.registrationType");
    }

    // 支付方式字段:现金、微信、支付宝、银行卡 1-cash, 2-wechat, 3-alipay, 4-bankcard
    DTO_FIELD(Int32, paymentMethod);
    DTO_FIELD_INFO(paymentMethod) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.paymentMethod");
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_CHARGE_CONFIRM_DTO_