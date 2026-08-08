#pragma once

#ifndef _QUIT_NUMBER_DTO_
#define _QUIT_NUMBER_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Confirm quit number DTO (receives frontend parameters)
 */
class QuitNumberDTO : public oatpp::DTO {
    DTO_INIT(QuitNumberDTO, DTO);

    // Visit number
    DTO_FIELD(String, visitNumber);
    DTO_FIELD_INFO(visitNumber) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.visitNumber");
        info->required = true;
    }

    // Patient name
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.patientName");
        info->required = true;
    }

    // Phone number
    DTO_FIELD(String, phone);
    DTO_FIELD_INFO(phone) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.phone");
        info->required = true;
    }

    // Registration department
    DTO_FIELD(String, department);
    DTO_FIELD_INFO(department) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.department");
        info->required = true;
    }

    // Attending doctor
    DTO_FIELD(String, doctor);
    DTO_FIELD_INFO(doctor) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.doctor");
        info->required = true;
    }

    // Registration type
    DTO_FIELD(String, registrationType);
    DTO_FIELD_INFO(registrationType) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.registrationType");
        info->required = true;
    }

    // Registration time
    DTO_FIELD(String, registrationTime);
    DTO_FIELD_INFO(registrationTime) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.registrationTime");
        info->required = true;
    }

    // Refund amount
    DTO_FIELD(Float64, refundAmount);
    DTO_FIELD_INFO(refundAmount) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.refundAmount");
        info->required = true;
    }

    // Current status
    DTO_FIELD(String, status);
    DTO_FIELD_INFO(status) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.status");
        info->required = true;
    }

    // Encounter ID
    DTO_FIELD(String, encounterId);
    DTO_FIELD_INFO(encounterId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.encounterId");
        info->required = false;
    }

    // Version
    DTO_FIELD(Int32, version);
    DTO_FIELD_INFO(version) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.version");
        info->required = false;
    }

    // Slot ID
    DTO_FIELD(String, slotId);
    DTO_FIELD_INFO(slotId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.slotId");
        info->required = false;
    }

    // Charge item ID
    DTO_FIELD(String, chargeItemId);
    DTO_FIELD_INFO(chargeItemId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.chargeItemId");
        info->required = false;
    }

    // refundRequestId
    DTO_FIELD(String, refundRequestId);
    DTO_FIELD_INFO(refundRequestId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.refundRequestId");
        info->required = false;
    }

    // Account ID
    DTO_FIELD(String, accountId);
    DTO_FIELD_INFO(accountId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.accountId");
        info->required = false;
    }

    // Enterer ID
    DTO_FIELD(String, entererId);
    DTO_FIELD_INFO(entererId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.entererId");
        info->required = false;
    }

    // Payment method enum
    DTO_FIELD(String, payEnum);
    DTO_FIELD_INFO(payEnum) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.payEnum");
        info->required = false;
    }

    // Patient ID
    DTO_FIELD(String, patientId);
    DTO_FIELD_INFO(patientId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.patientId");
        info->required = false;
    }

    // Payment serial number
    DTO_FIELD(String, paymentNo);
    DTO_FIELD_INFO(paymentNo) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.paymentNo");
        info->required = false;
    }

    // Charge item IDs
    DTO_FIELD(String, refundingChargeItemIds);
    DTO_FIELD_INFO(refundingChargeItemIds) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.refundingChargeItemIds");
        info->required = false;
    }

    // Detail ID
    DTO_FIELD(String, detailId);
    DTO_FIELD_INFO(detailId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.detailId");
        info->required = false;
    }

    // Invoice ID
    DTO_FIELD(String, invoiceId);
    DTO_FIELD_INFO(invoiceId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.invoiceId");
        info->required = false;
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_QUIT_NUMBER_DTO_
