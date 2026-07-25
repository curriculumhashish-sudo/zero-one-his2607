#pragma once

#ifndef _PREPAYMENT_DTO_
#define _PREPAYMENT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
   收预交金请求DTO
*/
class PrepaymentAddDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentAddDTO, DTO);

    // 住院号
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // 收款金额
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.amount.description"),
        true, 1000.00);

    // 支付方式（1-现金,2-微信,3-支付宝,4-银行卡）
    API_DTO_FIELD(String, paymentMethod,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.paymentMethod.description"),
        true, "1");

    // 收款操作员
    API_DTO_FIELD(String, operatorName,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.operator.description"),
        true, "");

    // 备注
    API_DTO_FIELD(String, remark,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddDTO.remark.description"),
        false, "");
};

/*
   退预交金请求DTO
*/
class PrepaymentRefundDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentRefundDTO, DTO);

    // 住院号
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // 退款金额
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.amount.description"),
        true, 200.00);

    // 退款方式（1-现金,2-微信,3-支付宝,4-银行卡）
    API_DTO_FIELD(String, refundMethod,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.refundMethod.description"),
        true, "1");

    // 退款操作员
    API_DTO_FIELD(String, operatorName,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.operator.description"),
        true, "");

    // 退款原因
    API_DTO_FIELD(String, reason,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.reason.description"),
        false, "");

    // 备注
    API_DTO_FIELD(String, remark,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundDTO.remark.description"),
        false, "");
};

/*
   收预交金响应DTO
*/
class PrepaymentAddResponseDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentAddResponseDTO, DTO);

    // 预交金流水号
    API_DTO_FIELD(String, prepaymentId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.prepaymentId.description"),
        true, "demo-prepay-001");

    // 住院号
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // 收款金额
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.amount.description"),
        true, 1000.00);

    // 支付方式
    API_DTO_FIELD(String, paymentMethod,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.paymentMethod.description"),
        true, ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.paymentMethod.value"));

    // 收款时间
    API_DTO_FIELD(String, createTime,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentAddResponseDTO.createTime.description"),
        true, "2026-07-20 10:30:00");
};

/*
   退预交金响应DTO
*/
class PrepaymentRefundResponseDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentRefundResponseDTO, DTO);

    // 预交金流水号
    API_DTO_FIELD(String, prepaymentId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.prepaymentId.description"),
        true, "demo-prepay-002");

    // 住院号
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // 退款金额
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.amount.description"),
        true, 200.00);

    // 退款方式
    API_DTO_FIELD(String, refundMethod,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.refundMethod.description"),
        true, ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.refundMethod.value"));

    // 退款时间
    API_DTO_FIELD(String, createTime,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRefundResponseDTO.createTime.description"),
        true, "2026-07-20 10:35:00");
};

/*
   预交金流水记录DTO（列表查询用）
*/
class PrepaymentRecordDTO : public oatpp::DTO
{
    DTO_INIT(PrepaymentRecordDTO, DTO);

    // 预交金流水号
    API_DTO_FIELD(String, prepaymentId,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.prepaymentId.description"),
        true, "demo-prepay-001");

    // 住院号
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.inpatientNo.description"),
        true, "ZY202607190001");

    // 患者姓名
    API_DTO_FIELD(String, patientName,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.patientName.description"),
        true, ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.patientName.value"));

    // 金额
    API_DTO_FIELD(Float64, amount,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.amount.description"),
        true, 1000.00);

    // 类型（ADD-收,REFUND-退）
    API_DTO_FIELD(String, prepayType,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.type.description"),
        true, "ADD");

    // 支付/退款方式
    API_DTO_FIELD(String, method,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.method.description"),
        true, ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.method.value"));

    // 操作员
    API_DTO_FIELD(String, operatorName,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.operator.description"),
        true, "");

    // 操作时间
    API_DTO_FIELD(String, createTime,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.createTime.description"),
        true, "2026-07-20 10:30:00");
};

/*
   预交金分页DTO（列表查询用）
*/
class PrepaymentPageDTO : public PageDTO<PrepaymentRecordDTO::Wrapper>
{
    DTO_INIT(PrepaymentPageDTO, PageDTO<PrepaymentRecordDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PREPAYMENT_DTO_
