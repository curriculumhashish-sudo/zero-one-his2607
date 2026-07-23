#ifndef _PATIENTCONSUMABLEDTO_H_
#define _PATIENTCONSUMABLEDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientConsumableDTO :public oatpp::DTO
{
	DTO_INIT(PatientConsumableDTO, DTO);
	// Consumable name
	API_DTO_FIELD_REQUIRE(String, consumableName, ZH_WORDS_GETTER("patientconsumable1.field.consumableName"), true);
	// Model specification
	API_DTO_FIELD_REQUIRE(String, modelSpec, ZH_WORDS_GETTER("patientconsumable1.field.modelSpec"), true);
	// Quantity
	API_DTO_FIELD_REQUIRE(String, quantity, ZH_WORDS_GETTER("patientconsumable1.field.quantity"), true);
	// Status
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("patientconsumable1.field.status"), true);
	// Consumable room
	API_DTO_FIELD_REQUIRE(String, consumableRoom, ZH_WORDS_GETTER("patientconsumable1.field.consumableRoom"), true);
	// isEmpty
	API_DTO_FIELD_REQUIRE(Int32, isEmpty, ZH_WORDS_GETTER("patientconsumable1.field.isEmpty"), true);

};
#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENTCONSUMABLEDTO_H_
