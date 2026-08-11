#pragma once
#ifndef _SETTINGSSERVICE_H_
#define _SETTINGSSERVICE_H_

#include "domain/query/Settings/SettingsQuery.h"
#include "domain/query/Settings/ConfigItemQuery.h"
#include "domain/dto/Settings/SettingsDTO.h"

class SettingsService
{
public:
	SettingsPageDTO::Wrapper queryPage(const SettingsQuery::Wrapper& query);
	oatpp::List<SettingsDTO::Wrapper> queryByName(const ConfigItemQuery::Wrapper& query);
};

#endif