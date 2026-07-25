#ifndef _PATIENTACTQUERY_H_
#define _PATIENTACTQUERY_H_
#include"../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientActQuery : public oatpp::DTO
{
	DTO_INIT(PatientActQuery, DTO);
	// Patient ID
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("patientact.field.patientId"), true);
	// Encounter ID
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("patientact.field.encounterId"), true);
	// Whether executed
	API_DTO_FIELD_REQUIRE(Int32, act, ZH_WORDS_GETTER("patient1.field.act"), false);
	// Status
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("patient1.field.status"), false);
	// Order content
	API_DTO_FIELD_REQUIRE(String, orderContent, ZH_WORDS_GETTER("patient1.field.orderContent"), false);
};
#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENTACTQUERY_H_
