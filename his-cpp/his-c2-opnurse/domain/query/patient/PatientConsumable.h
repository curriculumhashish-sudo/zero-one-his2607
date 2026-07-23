#ifndef _PATIENTCONSUMQUERY_H_
#define _PATIENTCONSUMQUERY_H_
#include"../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientConsumQuery : public oatpp::DTO
{
	DTO_INIT(PatientConsumQuery, DTO);
	// Consumable room
	API_DTO_FIELD_REQUIRE(String, consumableRoom, ZH_WORDS_GETTER("patientconsumable1.field.consumableRoom"), false);

};
#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENTCONSUMQUERY_H_
