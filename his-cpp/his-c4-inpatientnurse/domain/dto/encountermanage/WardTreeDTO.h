#pragma once
#ifndef _WARD_TREE_DTO_H_
#define _WARD_TREE_DTO_H_
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class HouseNodeDTO : public oatpp::DTO
{
	DTO_INIT(HouseNodeDTO, DTO);
	API_DTO_FIELD_REQUIRE(String, houseId, ZH_WORDS_GETTER("encountermanage.field.houseId"), true);
	API_DTO_FIELD_REQUIRE(String, houseName, ZH_WORDS_GETTER("encountermanage.field.houseName"), true);
	API_DTO_FIELD_REQUIRE(Int32, displayOrder, ZH_WORDS_GETTER("encountermanage.field.displayOrder"), true);
public:
	HouseNodeDTO() { displayOrder = 0; }
};
class WardNodeDTO : public oatpp::DTO
{
	DTO_INIT(WardNodeDTO, DTO);
	API_DTO_FIELD_REQUIRE(String, wardId, ZH_WORDS_GETTER("encountermanage.field.wardId"), true);
	API_DTO_FIELD_REQUIRE(String, wardName, ZH_WORDS_GETTER("encountermanage.field.wardName"), true);
	API_DTO_FIELD_REQUIRE(Int32, displayOrder, ZH_WORDS_GETTER("encountermanage.field.displayOrder"), true);
	API_DTO_FIELD_REQUIRE(List<HouseNodeDTO::Wrapper>, houseNodes, ZH_WORDS_GETTER("encountermanage.field.houseNodes"), true);
public:
	WardNodeDTO() { displayOrder = 0; houseNodes = List<HouseNodeDTO::Wrapper>::createShared(); }
};
class WardTreeDTO : public oatpp::DTO
{
	DTO_INIT(WardTreeDTO, DTO);
	API_DTO_FIELD_REQUIRE(List<WardNodeDTO::Wrapper>, wardNodes, ZH_WORDS_GETTER("encountermanage.field.wardNodes"), true);
public:
	WardTreeDTO() { wardNodes = List<WardNodeDTO::Wrapper>::createShared(); }
};
#include OATPP_CODEGEN_END(DTO)
#endif
