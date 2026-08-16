#pragma once
#ifndef _SETTINGSDTO_H_
#define _SETTINGSDTO_H_
#include "domain/GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class SettingsDTO :public oatpp::DTO {
	DTO_INIT(SettingsDTO,DTO)
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("settings.field.id"), false);
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("settings.field.name"), false);
	API_DTO_FIELD_REQUIRE(String, value, ZH_WORDS_GETTER("settings.field.value"), false);
	API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("settings.field.description"), false);
	API_DTO_FIELD_REQUIRE(String, provider_name, ZH_WORDS_GETTER("settings.field.provider_name"), false);
	API_DTO_FIELD_REQUIRE(String, provider_key, ZH_WORDS_GETTER("settings.field.provider_key"), false);
	API_DTO_FIELD_REQUIRE(String, tenant_id, ZH_WORDS_GETTER("settings.field.tenant_id"), false);
};
class SettingsPageDTO :public PageDTO<SettingsDTO::Wrapper> {
	DTO_INIT(SettingsPageDTO, PageDTO<SettingsDTO::Wrapper>);
};
#include OATPP_CODEGEN_END(DTO)
#endif