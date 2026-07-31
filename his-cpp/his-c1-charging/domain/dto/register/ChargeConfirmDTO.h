#pragma once

#ifndef _CHARGE_CONFIRM_DTO_
#define _CHARGE_CONFIRM_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 确认缴费DTO
 */
class ChargeConfirmDTO : public oatpp::DTO {
    DTO_INIT(ChargeConfirmDTO, DTO);

    // 就诊号
    DTO_FIELD(String, visitNumber);
    DTO_FIELD_INFO(visitNumber) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.visitNumber");
    }

    // Patient ID field
    DTO_FIELD(String, patientId);
    DTO_FIELD_INFO(patientId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.patientId");
    }

    // Patient name field
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.patientName");
    }

    // Phone field
    DTO_FIELD(String, phone);
    DTO_FIELD_INFO(phone) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.phone");
    }

    // money field
    DTO_FIELD(Float64, amount);
    DTO_FIELD_INFO(amount) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.amount");
    }

    // 支付方式草稿号
    DTO_FIELD(String, paymentDraft);
    DTO_FIELD_INFO(paymentDraft) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.paymentDraft");
    }

    // 挂号类型
    DTO_FIELD(String, registrationType);
    DTO_FIELD_INFO(registrationType) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.registrationType");
    }

    // 支付方式 1-cash, 2-wechat, 3-alipay, 4-bankcard
    DTO_FIELD(Int32, paymentMethod);
    DTO_FIELD_INFO(paymentMethod) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.paymentMethod");
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_CHARGE_CONFIRM_DTO_