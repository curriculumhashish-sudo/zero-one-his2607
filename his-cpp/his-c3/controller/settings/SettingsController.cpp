#include "stdafx.h"
#include "SettingsController.h"
#include "../../service/settings/SettingsService.h"


SettingsPageJsonVO::Wrapper SettingsController::executeQuerySettingsList(const SettingsQuery::Wrapper& query)
{
	auto result = SettingsService().queryPage(query);
	auto jvo = SettingsPageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

SettingsListJsonVO::Wrapper SettingsController::executeQueryConfigItems(const ConfigItemQuery::Wrapper& query)
{

	auto result = SettingsService().queryByName(query);
	auto jvo = SettingsListJsonVO::createShared();
	jvo->success(result);
	return jvo;
}




