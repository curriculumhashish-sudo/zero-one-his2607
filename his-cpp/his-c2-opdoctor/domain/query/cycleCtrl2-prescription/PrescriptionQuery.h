#pragma once
#ifndef _PrescriptionQuery_H_
#define _PrescriptionQuery_H_

#include"../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class Prescription2Query :public oatpp::DTO
{
    DTO_INIT(Prescription2Query, DTO);
    API_DTO_FIELD_DEFAULT(String, num, ZH_WORDS_GETTER("cyclectrl2.patient.patientNo"));
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("cyclectrl2.patient.encounterId"));
    API_DTO_FIELD_DEFAULT(String, type, ZH_WORDS_GETTER("cyclectrl2.patient.timetype"));
    API_DTO_FIELD_DEFAULT(Int32, page, ZH_WORDS_GETTER("cyclectrl2.patient.page"));
    API_DTO_FIELD_DEFAULT(Int32, pageSize, ZH_WORDS_GETTER("cyclectrl2.patient.pageSize"));
};

#include OATPP_CODEGEN_END(DTO)

#endif
