#pragma once
#ifndef _CONFIGITEMQUERY_H_
#define _CONFIGITEMQUERY_H_
#include "domain/GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class ConfigItemQuery :public oatpp::DTO {
	DTO_INIT(ConfigItemQuery, DTO);
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("settings.field.name"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif