/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 14:26:52

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "TreatmentDAO.h"
#include "TreatmentMapper.h"

std::string TreatmentDAO::queryConditionBuilder(const TreatmentQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";
	if (query->keywords) {

		sqlCondition << " AND `keywords`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
	}
	return sqlCondition.str();
}
// 统计数据条数
uint64_t TreatmentDAO::count(const TreatmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM  wkf_activity_definition";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}
// 分页查询数据
std::list<TreatmentDO> TreatmentDAO::selectWithPage(const TreatmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT * FROM wkf_activity_definition ";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 构建排序语句
	sql += " ORDER BY IFNULL(`update_time`, `create_time`) DESC, `id` DESC ";
	// 构建分页条件
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// 执行查询
	return sqlSession->executeQuery<TreatmentDO>(sql, TreatmentMapper(), params);
}
// 删除
int32_t TreatmentDAO::removeData(std::list<std::string>& ids)
{
	if (ids.empty()) return 0;

	std::string sql = "UPDATE wkf_activity_definition SET is_deleted = 1, deleted_at = NOW() WHERE id IN (";
	SqlParams params;
	bool first = true;

	for (const auto& item : ids)
	{
		if (!first) sql += ",";
		first = false;
		sql += "?";
		// 添加字符串参数
		SQLPARAMS_PUSH(params, "s", std::string, item);
	}
	sql += ") AND is_deleted = 0";

	return sqlSession->executeUpdate(sql, params);
}