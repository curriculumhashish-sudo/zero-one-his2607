#pragma once
#ifndef __REFUNDAPPLYDTO_H_
#define __REFUNDAPPLYDTO_H_

#include "domain/dto/doctor/Refund2ApplyItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Refund2ApplyDTO :public oatpp::DTO
{
    DTO_INIT(Refund2ApplyDTO, DTO);

    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("cyclectrl2.patient.encounterId"));

    API_DTO_FIELD_DEFAULT(String, reason, ZH_WORDS_GETTER("cyclectrl2.patient.reason"));

    API_DTO_FIELD_DEFAULT(String, idempotencyKey, ZH_WORDS_GETTER("cyclectrl2.patient.idempotencyKey"));

    API_DTO_FIELD_DEFAULT(List<Object<Refund2ApplyItemDTO>>, apply, ZH_WORDS_GETTER("cyclectrl2.patient.apply"));

    API_DTO_FIELD_DEFAULT(Float64, applySum, ZH_WORDS_GETTER("cyclectrl2.patient.applySum"));

};

#include OATPP_CODEGEN_END(DTO)

#endif
