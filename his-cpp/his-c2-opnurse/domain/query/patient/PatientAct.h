#ifndef _PATIENTACTQUERY_H_
#define _PATIENTACTQUERY_H_
#include"../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientActQuery : public oatpp::DTO
{
	DTO_INIT(PatientActQuery, DTO);
	// Whether executed
	API_DTO_FIELD_REQUIRE(Int32, act, ZH_WORDS_GETTER("patient1.field.act"), false);
	// Status
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("patient1.field.status"), true);
	// Order content
	API_DTO_FIELD_REQUIRE(String, orderContent, ZH_WORDS_GETTER("patient1.field.orderContent"), true);
};
#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENTACTQUERY_H_
