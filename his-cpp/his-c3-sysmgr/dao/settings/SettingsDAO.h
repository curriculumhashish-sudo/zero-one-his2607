#pragma once
#ifndef _SETTINGSDAO_H_
#define _SETTINGSDAO_H_
#include "BaseDAO.h"
#include "../../domain/do/settings/SettingsDO.h"
#include "../../domain/query/Settings/SettingsQuery.h"

class SettingsDAO : public BaseDAO {
private:
	inline std::string queryConditionBuilder(const SettingsQuery::Wrapper& query, SqlParams& params);
public:
	uint64_t count(const SettingsQuery::Wrapper& query);
	std::list<SettingsDO> selectWithPage(const SettingsQuery::Wrapper& query);
	std::list<SettingsDO> selectByName(const std::string& name);
};

#endif