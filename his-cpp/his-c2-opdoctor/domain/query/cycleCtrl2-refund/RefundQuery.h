#pragma once
#ifndef _QUERY_H_
#define _QUERY_H_

#include"../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class Refund2Query :public oatpp::DTO
{
    DTO_INIT(Refund2Query, DTO);
    API_DTO_FIELD_DEFAULT(String, num, ZH_WORDS_GETTER("cyclectrl2.patient.patientNo"));
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("cyclectrl2.patient.encounterId"));
};

#include OATPP_CODEGEN_END(DTO)

#endif
