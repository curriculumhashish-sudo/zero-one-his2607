#pragma once
#ifndef __REFUNDAPPLYDTO_H_
#define __REFUNDAPPLYDTO_H_

#include "domain/dto/doctor/Prescription2DetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Refund2ApplyDTO :public oatpp::DTO
{
    DTO_INIT(Refund2ApplyDTO, DTO);

    // ´¦·½µ¥
    API_DTO_FIELD_DEFAULT(List<Object<PrescriptionDetailDTO>>, Prescription, ZH_WORDS_GETTER("cyclectrl2.prescription.tag"));

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_QUERY_H_
