#pragma once
#ifndef _SETTINGSQUERY_H_
#define _SETTINGSQUERY_H_
#include "domain/GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class SettingsQuery : public PageQuery {
	DTO_INIT(SettingsQuery, PageQuery);
	API_DTO_FIELD_REQUIRE(String, keywords,ZH_WORDS_GETTER("settings.field.keywords"),false);
};
#include OATPP_CODEGEN_END(DTO);
#endif
