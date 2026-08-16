#include "stdafx.h"
#include "SettingsDAO.h"
#include "SettingsMapper.h"

std::string SettingsDAO::queryConditionBuilder(const SettingsQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE is_deleted=0";
	sqlCondition << " AND provider_name='G'";
	if (query->keywords) {
		sqlCondition << " AND (name LIKE CONCAT('%',?,'%') OR description LIKE CONCAT('%',?,'%'))";
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t SettingsDAO::count(const SettingsQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM whale_settings ";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<SettingsDO> SettingsDAO::selectWithPage(const SettingsQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT id,name,value,description,provider_name,provider_key,tenant_id FROM whale_settings ";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY name ASC ";
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);
	return sqlSession->executeQuery<SettingsDO>(sql, SettingsMapper(), params);
}

std::list<SettingsDO> SettingsDAO::selectByName(const std::string& name)
{
	SqlParams params;
	string sql = "SELECT id,name,value,description,provider_name,provider_key,tenant_id FROM whale_settings WHERE is_deleted=0 AND provider_name='G' AND name LIKE CONCAT('%',?,'%')";
	SQLPARAMS_PUSH(params, "s", std::string, name);
	return sqlSession->executeQuery<SettingsDO>(sql, SettingsMapper(), params);
}