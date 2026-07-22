#pragma once
#pragma once
#ifndef _SETTINGSLISTJSONVO_H_
#define _SETTINGSLISTJSONVO_H_
#include "domain/dto/settings/SettingsDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class SettingsListJsonVO :public ListJsonVO<SettingsDTO::Wrapper> {
	DTO_INIT(SettingsListJsonVO, ListJsonVO<SettingsDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif