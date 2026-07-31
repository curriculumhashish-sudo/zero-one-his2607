#include"stdafx.h"
#include"FrequencyDAO.h"
#include"FrequencyMapper.h"

static const char* FREQ_COLUMNS =
"id, tenant_id, created_at, updated_at, created_by, updated_by, "
"is_deleted, deleted_at, deleted_by, version, "
"rate_code, `name`, cycle_type, day_count, day_interval, day_times, "
"week_cycle_flag, total_execution_count, execution_period, execution_period_unit, "
"applicable_scenes, allow_cross_day, active_flag, sort_no ";

std::string FrequencyDAO::queryConditionBuilder(const FrequencyQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	// 基础条件：排除软删除记录
	sqlCondition << " WHERE is_deleted = 0";

	// 按启用/停用状态筛选
	if (query->status) {
		sqlCondition << " AND active_flag = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->status.getValue(""));
	}

	// 按关键字模糊匹配（频次名称 或 频次编码）
	if (query->keywords) {
		sqlCondition << " AND (`name` LIKE CONCAT('%',?,'%') OR rate_code LIKE CONCAT('%',?,'%'))";
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
	}

	return sqlCondition.str();
}

uint64_t FrequencyDAO::count(const FrequencyQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM cus_frequency ";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<CusFrequencyDO> FrequencyDAO::selectWithPage(const FrequencyQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT ";
	sql += FREQ_COLUMNS;
	sql += " FROM cus_frequency "; // 选用 * 匹配 24 列 Mapper
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY IFNULL(sort_no, 999999) ASC, IFNULL(updated_at, created_at) DESC, id DESC ";
	sql += " LIMIT " + std::to_string((query->pageIndex - 1) * query->pageSize)
		+ "," + std::to_string(query->pageSize);

	return sqlSession->executeQuery<CusFrequencyDO>(sql, CusFrequencyMapper(), params);
}

std::list<CusFrequencyDO> FrequencyDAO::selectByName(const string& name)
{
	string sql = "SELECT ";
	sql += FREQ_COLUMNS;
	sql += "FROM cus_frequency "
		"WHERE is_deleted = 0 AND `name` LIKE CONCAT('%',?,'%')";
	return sqlSession->executeQuery<CusFrequencyDO>(sql, CusFrequencyMapper(), "%s", name);
}

PtrCusFrequencyDO FrequencyDAO::selectById(std::string id)
{
	string sql = "SELECT ";
	sql += FREQ_COLUMNS;
	sql += "FROM cus_frequency "
		"WHERE is_deleted = 0 AND id = ?";
	auto res = sqlSession->executeQueryOne<PtrCusFrequencyDO>(sql, PtrCusFrequencyMapper(), "%s", id);
	return res;
}