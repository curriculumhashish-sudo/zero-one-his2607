#pragma once
#ifndef __REFUNDAPPLYDTO_H_
#define __REFUNDAPPLYDTO_H_

#include "domain/dto/doctor/RefundDetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RefundApplyDTO :public oatpp::DTO
{
    DTO_INIT(RefundApplyDTO, DTO);

    // 患者id
    API_DTO_FIELD_DEFAULT(String, patientNo, ZH_WORDS_GETTER("patient.detail.patientNo"));

    // 勾选项
    API_DTO_FIELD_DEFAULT(List<Object<RefundDetailDTO>>, apply, ZH_WORDS_GETTER("patient.refund.apply"));

    // 勾选数量
    API_DTO_FIELD_DEFAULT(Int32, applyNo, ZH_WORDS_GETTER("patient.refund.applyNo"));

    // 已选合计费用
    API_DTO_FIELD_DEFAULT(Float64, applySum, ZH_WORDS_GETTER("patient.refund.applySum"));

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_QUERY_H_
