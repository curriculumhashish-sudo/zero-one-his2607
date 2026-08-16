#pragma once
#ifndef _SETTINGS_VO_
#define _SETTINGS_VO_
#include "domain/dto/Settings/SettingsDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class SettingsPageJsonVO :public JsonVO<SettingsPageDTO::Wrapper> {
	DTO_INIT(SettingsPageJsonVO, JsonVO<SettingsPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif