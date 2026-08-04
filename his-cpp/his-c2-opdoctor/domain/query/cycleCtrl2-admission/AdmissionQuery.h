#pragma once
#ifndef _AdmissionQuery_H_
#define _AdmissionQuery_H_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class Admission2Query : public oatpp::DTO
{
    DTO_INIT(Admission2Query, DTO);

    API_DTO_FIELD_DEFAULT(String, num, ZH_WORDS_GETTER("cyclectrl2.patient.patientNo"));
    API_DTO_FIELD_REQUIRE(String, admissionDepartment, ZH_WORDS_GETTER("cyclectrl2.admission.admissionDepartment"), true);
    API_DTO_FIELD_DEFAULT(String, admissionWard, ZH_WORDS_GETTER("cyclectrl2.admission.admissionWard"));
    API_DTO_FIELD_DEFAULT(String, diagnosisType, ZH_WORDS_GETTER("cyclectrl2.admission.diagnosisType"));
    API_DTO_FIELD_REQUIRE(String, admissionDiagnosis, ZH_WORDS_GETTER("cyclectrl2.admission.admissionDiagnosis"), true);
    API_DTO_FIELD_DEFAULT(String, diagnosisSyndrome, ZH_WORDS_GETTER("cyclectrl2.admission.diagnosisSyndrome"));
    API_DTO_FIELD_DEFAULT(String, diagnosisDesc, ZH_WORDS_GETTER("cyclectrl2.admission.diagnosisDesc"));
    API_DTO_FIELD_DEFAULT(String, admissionType, ZH_WORDS_GETTER("cyclectrl2.admission.admissionType"));
    API_DTO_FIELD_DEFAULT(String, admissionMethod, ZH_WORDS_GETTER("cyclectrl2.admission.admissionMethod"));
    API_DTO_FIELD_DEFAULT(String, priority, ZH_WORDS_GETTER("cyclectrl2.admission.priority"));
};

#include OATPP_CODEGEN_END(DTO)

#endif
