#ifndef _PATIENTCONSUMABLEDTO_H_
#define _PATIENTCONSUMABLEDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientConsumableDTO :public oatpp::DTO
{
	DTO_INIT(PatientConsumableDTO, DTO);
	// ÏûºÄÏîÄ¿ 
	API_DTO_FIELD_REQUIRE(String, ConsumableProject, ZH_WORDS_GETTER("patient.field.ConsumableProject"), false);

};
#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENTCONSUMABLEDTO_H_
