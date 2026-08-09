#pragma once
#ifndef __ADMISSION_DIAGNOSIS_DTO_H_
#define __ADMISSION_DIAGNOSIS_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class AdmissionDiagnosisDTO : public oatpp::DTO
{
    DTO_INIT(AdmissionDiagnosisDTO, DTO);

    API_DTO_FIELD_DEFAULT(String, catalogId, ZH_WORDS_GETTER("adm-diag.catalogId"));
    API_DTO_FIELD_DEFAULT(String, code, ZH_WORDS_GETTER("adm-diag.code"));
    API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("adm-diag.name"));
    API_DTO_FIELD_DEFAULT(String, diagnosisRecordId, ZH_WORDS_GETTER("adm-diag.diagnosisRecordId"));
    API_DTO_FIELD_DEFAULT(String, description, ZH_WORDS_GETTER("adm-diag.description"));
    API_DTO_FIELD_DEFAULT(Boolean, primary, ZH_WORDS_GETTER("adm-diag.primary"));
};

#include OATPP_CODEGEN_END(DTO)

#endif
