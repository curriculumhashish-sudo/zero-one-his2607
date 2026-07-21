#pragma once
#ifndef __REFUNDAPPLYDTO_H_
#define __REFUNDAPPLYDTO_H_

#include "domain/dto/doctor/PrescriptionDetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RefundApplyDTO :public oatpp::DTO
{
    DTO_INIT(RefundApplyDTO, DTO);

    // ´¦·½µ¥
    API_DTO_FIELD_DEFAULT(List<Object<PrescriptionDetailDTO>>, Prescription, ZH_WORDS_GETTER("patient.refund.Prescription"));

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_QUERY_H_
