#pragma once
#ifndef _SETTINGSCONTROLLER_H_
#define _SETTINGSCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/settings/SettingsQuery.h"
#include "domain/query/settings/ConfigItemQuery.h"
#include "domain/vo/settings/SettingsPageJsonVO.h"
#include "domain/vo/settings/SettingsListJsonVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("settings.api.tag")

class SettingsController : public oatpp::web::server::api::ApiController {
	API_ACCESS_DECLARE(SettingsController);
public:

	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("settings.api.query_list"), querySettingsList, SettingsQuery, SettingsPageJsonVO::Wrapper, API_TAG);
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("settings.api.query_items"), queryConfigItems, ConfigItemQuery, SettingsListJsonVO::Wrapper, API_TAG);


	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/settings/list", querySettingsList, SettingsQuery, executeQuerySettingsList(query));
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/settings/items", queryConfigItems, ConfigItemQuery, executeQueryConfigItems(query));

private:

	SettingsPageJsonVO::Wrapper executeQuerySettingsList(const SettingsQuery::Wrapper& query);
	SettingsListJsonVO::Wrapper executeQueryConfigItems(const ConfigItemQuery::Wrapper& query);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif