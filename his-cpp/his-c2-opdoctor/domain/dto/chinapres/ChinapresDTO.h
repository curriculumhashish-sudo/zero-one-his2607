#pragma once

#ifndef _CHINAPRESDTO_H_
#define _CHINAPRESDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapresDTO : public  oatpp::DTO
{
	DTO_INIT(ChinapresDTO, DTO);
	//处方编号
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("chinapres.field.id"), true);
	//处方数量
	API_DTO_FIELD_REQUIRE(String, count, ZH_WORDS_GETTER("chinapres.field.count"),true);
	//哪些药材
	API_DTO_FIELD_REQUIRE(String, herbs, ZH_WORDS_GETTER("chinapres.field.herbs"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_CHINAPRESDTO_H_