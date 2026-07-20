#pragma once

#ifndef _EQUIPMENTQUERY_H_
#define _EQUIPMENTQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class EquipmentQuery : public PageQuery
{
	DTO_INIT(EquipmentQuery, PageQuery);
	
	// ¹Ø¼ü´Ê
	API_DTO_FIELD_REQUIRE(String, keywords, ZH_WORDS_GETTER("equipment.field.keywords"), false);
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("equipment.field.name"), false);
	// Æ÷²Ä·ÖÀà
	API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("equipment.field.classification"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_EQUIPMENTQUERY_H_