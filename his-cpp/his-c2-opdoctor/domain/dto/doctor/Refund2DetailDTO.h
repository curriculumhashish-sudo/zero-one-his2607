#pragma once
#ifndef __REFUNDDETAILDTO_H_
#define __REFUNDDETAILDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Refund2DetailDTO :public oatpp::DTO
{
    DTO_INIT(Refund2DetailDTO, DTO);

    API_DTO_FIELD_DEFAULT(String, patientName, ZH_WORDS_GETTER("cyclectrl2.patient.patientName"));
    API_DTO_FIELD_DEFAULT(String, paymentNo, ZH_WORDS_GETTER("cyclectrl2.patient.paymentNo"));
    API_DTO_FIELD_DEFAULT(String, itemName, ZH_WORDS_GETTER("cyclectrl2.patient.itemName"));
    API_DTO_FIELD_DEFAULT(String, itemCode, ZH_WORDS_GETTER("cyclectrl2.patient.itemCode"));
    API_DTO_FIELD_DEFAULT(String, itemType, ZH_WORDS_GETTER("cyclectrl2.patient.itemType"));
    API_DTO_FIELD_DEFAULT(Float64, quantity, ZH_WORDS_GETTER("cyclectrl2.patient.quantity"));
    API_DTO_FIELD_DEFAULT(String, quantityUnit, ZH_WORDS_GETTER("cyclectrl2.patient.quantityUnit"));
    API_DTO_FIELD_DEFAULT(Float64, amount, ZH_WORDS_GETTER("cyclectrl2.patient.amount"));
    API_DTO_FIELD_DEFAULT(String, chargeTime, ZH_WORDS_GETTER("cyclectrl2.patient.chargeTime"));
    API_DTO_FIELD_DEFAULT(String, chargeStatus, ZH_WORDS_GETTER("cyclectrl2.patient.chargeStatus"));
    API_DTO_FIELD_DEFAULT(String, chargeStatusName, ZH_WORDS_GETTER("cyclectrl2.patient.chargeStatusName"));
    API_DTO_FIELD_DEFAULT(String, refundStatus, ZH_WORDS_GETTER("cyclectrl2.patient.refundStatus"));
    API_DTO_FIELD_DEFAULT(String, refundStatusName, ZH_WORDS_GETTER("cyclectrl2.patient.refundStatusName"));
    API_DTO_FIELD_DEFAULT(Float64, refundableQuantity, ZH_WORDS_GETTER("cyclectrl2.patient.refundableQuantity"));
    API_DTO_FIELD_DEFAULT(Float64, refundableAmount, ZH_WORDS_GETTER("cyclectrl2.patient.refundableAmount"));
    API_DTO_FIELD_DEFAULT(Float64, refundedAmount, ZH_WORDS_GETTER("cyclectrl2.patient.refundedAmount"));
    API_DTO_FIELD_DEFAULT(String, canRefund, ZH_WORDS_GETTER("cyclectrl2.patient.canRefund"));
    API_DTO_FIELD_DEFAULT(String, cannotRefundReason, ZH_WORDS_GETTER("cyclectrl2.patient.cannotRefundReason"));

};

#include OATPP_CODEGEN_END(DTO)

#endif
