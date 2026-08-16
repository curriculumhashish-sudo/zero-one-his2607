#pragma once
#ifndef  _DRUGLISTDTO_H_
#define  _DRUGLISTDTO_H_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
class DrugListDTO:public oatpp::DTO
{
	DTO_INIT(DrugListDTO, DTO);
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("Drug.field.id"), true);
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("Drug.field.code"), true);
	//关键词
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("Drug.field.name"), true);
	//名称
	API_DTO_FIELD_REQUIRE(String, guige, ZH_WORDS_GETTER("Drug.field.guige"), true);
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("Drug.field.status"), true);
	API_DTO_FIELD_REQUIRE(String, kinds, ZH_WORDS_GETTER("Drug.field.kinds"), true);
	API_DTO_FIELD_REQUIRE(String, changjia, ZH_WORDS_GETTER("Drug.field.changjia"), true);
	// API_DTO_FIELD_REQUIRE(String, price, ZH_WORDS_GETTER("Drug.field.price"), true);
};
class DrugListPageDTO :public PageDTO< DrugListDTO::Wrapper>
{
	DTO_INIT(DrugListPageDTO,PageDTO<DrugListDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !
