#pragma once

#ifndef _PREPAYMENT_DTO_
#define _PREPAYMENT_DTO_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Add prepayment request DTO
class PrepaymentAddDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentAddDTO, DTO);

    // Inpatient number
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // Amount received
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.amount.description"),
        true, 1000.00);

    // Payment method: 1-Cash, 2-WeChat, 3-Alipay, 4-Bank Card
    API_DTO_FIELD(String, paymentMethod,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.paymentMethod.description"),
        true, "1");

    // Operator
    API_DTO_FIELD(String, operatorName,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.operator.description"),
        true, "");

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

    // Refund amount
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.amount.description"),
        true, 200.00);

    // Refund method: 1-Cash, 2-WeChat, 3-Alipay, 4-Bank Card
    API_DTO_FIELD(String, refundMethod,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.refundMethod.description"),
        true, "1");

    // Operator
    API_DTO_FIELD(String, operatorName,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.operator.description"),
        true, "");

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

    // Prepayment transaction ID
    API_DTO_FIELD(String, prepaymentId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.prepaymentId.description"),
        true, "demo-prepay-001");

    // Inpatient number
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // Amount received
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.amount.description"),
        true, 1000.00);

    // Payment method
    API_DTO_FIELD(String, paymentMethod,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.paymentMethod.description"),
        true, ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.paymentMethod.value"));

    // Transaction time
    API_DTO_FIELD(String, createTime,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.createTime.description"),
        true, "2026-07-20 10:30:00");
};

// Refund prepayment response DTO
class PrepaymentRefundResponseDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentRefundResponseDTO, DTO);

    // Prepayment transaction ID
    API_DTO_FIELD(String, prepaymentId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.prepaymentId.description"),
        true, "demo-prepay-002");

    // Inpatient number
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // Refund amount
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.amount.description"),
        true, 200.00);

    // Refund method
    API_DTO_FIELD(String, refundMethod,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.refundMethod.description"),
        true, ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.refundMethod.value"));

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
