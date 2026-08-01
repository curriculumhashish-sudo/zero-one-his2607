#ifndef _PATIENTCONSUMABLEDTO_H_
#define _PATIENTCONSUMABLEDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientConsumableDTO : public oatpp::DTO
{
	DTO_INIT(PatientConsumableDTO, DTO);
	// Dispense item ID (device request primary key)
	API_DTO_FIELD_REQUIRE(String, dispenseItemId, ZH_WORDS_GETTER("patient1.field.dispenseItemId"), true);
	// Material ID (device definition primary key)
	API_DTO_FIELD_REQUIRE(String, materialId, ZH_WORDS_GETTER("patient1.field.materialId"), true);
	// Material display name
	API_DTO_FIELD_REQUIRE(String, materialName, ZH_WORDS_GETTER("patient1.field.materialName"), true);
	// Model/specification text
	API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("patient1.field.specification"), true);
	// Quantity with unit (e.g. "2x支")
	API_DTO_FIELD_REQUIRE(String, quantity, ZH_WORDS_GETTER("patient1.field.quantity"), true);
	// Unit display name
	API_DTO_FIELD_REQUIRE(String, unitName, ZH_WORDS_GETTER("patient1.field.unitName"), true);
	// Room name (where consumable is used)
	API_DTO_FIELD_REQUIRE(String, roomName, ZH_WORDS_GETTER("patient1.field.roomName"), true);
	// Status enum code
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("patient1.field.status"), true);
	// Status display name
	API_DTO_FIELD_REQUIRE(String, statusName, ZH_WORDS_GETTER("patient1.field.statusName"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENTCONSUMABLEDTO_H_
