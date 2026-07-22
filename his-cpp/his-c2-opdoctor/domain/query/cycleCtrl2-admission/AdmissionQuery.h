#pragma once
#ifndef _AdmissionQuery_H_
#define _AdmissionQuery_H_

#include"../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class Admission2Query :public oatpp::DTO
{
	DTO_INIT(Admission2Query, DTO);

	//入院科室
	API_DTO_FIELD_REQUIRE(String, admissionDepartment, ZH_WORDS_GETTER("cyclectrl2.admission..admissionDepartment"),true);

	//入院病区
	API_DTO_FIELD_REQUIRE(String, admissionWard, ZH_WORDS_GETTER("cyclectrl2.admission.admissionWard"),true);
	
	//入院主诊断
	API_DTO_FIELD_REQUIRE(String, admissionDiagnosis, ZH_WORDS_GETTER("cyclectrl2.admission.admissionDiagnosis"),true);

	//诊断描述
	API_DTO_FIELD_REQUIRE(String, diagnosisDesc, ZH_WORDS_GETTER("cyclectrl2.admission.diagnosisDesc"),false);

	//入院类型
	API_DTO_FIELD_REQUIRE(String, admissionType, ZH_WORDS_GETTER("cyclectrl2.admission.admissionType"),true);

	//入院方式
	API_DTO_FIELD_REQUIRE(String, admissionMethod, ZH_WORDS_GETTER("cyclectrl2.admission.admissionMethod"),true);

	//病情优先级
	API_DTO_FIELD_REQUIRE(String, priority, ZH_WORDS_GETTER("cyclectrl2.admission.priority"),true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_QUERY_H_
