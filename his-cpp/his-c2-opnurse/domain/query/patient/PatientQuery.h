#ifndef _PATIENTQUEUE_
#define _PATIENTQUEUE_
#include"../../GlobalInclude.h"
#include"domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientQuery :public PageQuery
{
	DTO_INIT(PatientQuery, PageQuery);
	// Patient name
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("patient.field.name"), false);
	// Date
	API_DTO_FIELD_REQUIRE(String, date, ZH_WORDS_GETTER("patient.field.date"), false);

};
#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENTQUEUE_
