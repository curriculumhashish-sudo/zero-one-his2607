#pragma once

#ifndef _PREPAYMENT_DTO_
#define _PREPAYMENT_DTO_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Payment detail item (shared by add and refund)
class PrepaymentPaymentDetailDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentPaymentDetailDTO, DTO);

    // Payment channel code: CASH, WECHAT, ALIPAY, BANK_CARD
    API_DTO_FIELD(String, payChannelCode,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentPaymentDetailDTO.payChannelCode.description"),
        true, "CASH");

    // Payment amount
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentPaymentDetailDTO.amount.description"),
        true, 1000.00);
};

// Add prepayment request DTO
class PrepaymentAddDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentAddDTO, DTO);

    // Inpatient number
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // Payment details array
    DTO_FIELD(oatpp::List<PrepaymentPaymentDetailDTO::Wrapper>, paymentDetails);
    DTO_FIELD_INFO(paymentDetails) {
        info->description = ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.paymentDetails.description");
    };

    // Payment draft ID (from pre-settle workflow)
    API_DTO_FIELD(String, paymentDraftId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.paymentDraftId.description"),
        false, "");

    // Operator ID
    API_DTO_FIELD(String, operatorId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.operatorId.description"),
        true, "");

    // Operator name
    API_DTO_FIELD(String, operatorName,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.operatorName.description"),
        false, "");

    // Remark
    API_DTO_FIELD(String, remark,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.remark.description"),
        false, "");
};

// Refund prepayment request DTO
class PrepaymentRefundDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentRefundDTO, DTO);

    // Inpatient number
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // Payment details array
    DTO_FIELD(oatpp::List<PrepaymentPaymentDetailDTO::Wrapper>, paymentDetails);
    DTO_FIELD_INFO(paymentDetails) {
        info->description = ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.paymentDetails.description");
    };

    // Payment draft ID (from pre-settle workflow)
    API_DTO_FIELD(String, paymentDraftId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.paymentDraftId.description"),
        false, "");

    // Operator ID
    API_DTO_FIELD(String, operatorId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.operatorId.description"),
        true, "");

    // Operator name
    API_DTO_FIELD(String, operatorName,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.operatorName.description"),
        false, "");

    // Refund reason
    API_DTO_FIELD(String, reason,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.reason.description"),
        false, "");

    // Remark
    API_DTO_FIELD(String, remark,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.remark.description"),
        false, "");
};

// Add prepayment response DTO
class PrepaymentAddResponseDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentAddResponseDTO, DTO);

    // Payment ID
    API_DTO_FIELD(String, paymentId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.paymentId.description"),
        true, "pay-001");

    // Transaction number
    API_DTO_FIELD(String, transactionNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.transactionNo.description"),
        true, "TXN202608040001");

    // Encounter ID
    API_DTO_FIELD(String, encounterId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.encounterId.description"),
        false, "");

    // Inpatient number
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // Total amount
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.amount.description"),
        true, 1000.00);

    // Account balance after transaction
    API_DTO_FIELD(Float64, balance,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.balance.description"),
        true, 1000.00);

    // Status enum
    API_DTO_FIELD(String, statusEnum,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.statusEnum.description"),
        true, "SUCCESS");

    // Transaction time
    API_DTO_FIELD(String, createTime,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.createTime.description"),
        true, "2026-07-20 10:30:00");
};

// Refund prepayment response DTO
class PrepaymentRefundResponseDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentRefundResponseDTO, DTO);

    // Payment ID
    API_DTO_FIELD(String, paymentId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.paymentId.description"),
        true, "pay-002");

    // Transaction number
    API_DTO_FIELD(String, transactionNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.transactionNo.description"),
        true, "TXN202608040002");

    // Encounter ID
    API_DTO_FIELD(String, encounterId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.encounterId.description"),
        false, "");

    // Inpatient number
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // Total refund amount
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.amount.description"),
        true, 200.00);

    // Account balance after refund
    API_DTO_FIELD(Float64, balance,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.balance.description"),
        true, 800.00);

    // Status enum
    API_DTO_FIELD(String, statusEnum,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.statusEnum.description"),
        true, "SUCCESS");

    // Transaction time
    API_DTO_FIELD(String, createTime,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.createTime.description"),
        true, "2026-07-20 10:35:00");
};

// Prepayment record DTO (for list query)
class PrepaymentRecordDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentRecordDTO, DTO);

    // Prepayment transaction ID
    API_DTO_FIELD(String, prepaymentId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.prepaymentId.description"),
        true, "demo-prepay-001");

    // Inpatient number
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // Patient name
    API_DTO_FIELD(String, patientName,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.patientName.description"),
        true, ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.patientName.value"));

    // Amount
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.amount.description"),
        true, 1000.00);

    // Type: ADD-charge, REFUND-refund
    API_DTO_FIELD(String, prepayType,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.type.description"),
        true, "ADD");

    // Method
    API_DTO_FIELD(String, method,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.method.description"),
        true, ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.method.value"));

    // Operator
    API_DTO_FIELD(String, operatorName,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.operator.description"),
        true, "");

    // Transaction time
    API_DTO_FIELD(String, createTime,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.createTime.description"),
        true, "2026-07-20 10:30:00");
};

// Prepayment page DTO (for paginated query response)
class PrepaymentPageDTO : public PageDTO<PrepaymentRecordDTO::Wrapper>
{
    DTO_INIT(PrepaymentPageDTO, PageDTO<PrepaymentRecordDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PREPAYMENT_DTO_
