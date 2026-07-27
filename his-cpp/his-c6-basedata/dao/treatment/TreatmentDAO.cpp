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
	if (query->name) {

		sqlCondition << " AND `name`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->name.getValue(""));
	}
	return sqlCondition.str();
}
// 统计数据条数
uint64_t TreatmentDAO::count(const TreatmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM  wkfActivity_definition";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}
// 分页查询数据
std::list<TreatmentDO> TreatmentDAO::selectWithPage(const TreatmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT id,name,sex,age FROM wkfActivity_definition ";
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
int32_t removeData(std::list<std::string>& ids)
{
	if (ids.empty()) return 0;

	// 1. 拼接 IN(?,?,?) 占位符
	std::string sql = "UPDATE wkfActivity_definition SET is_deleted = 1, deleted_at = NOW() WHERE id IN (";
	SqlParams params;

	bool first = true;
	for (const auto& item : ids)
	{
		if (!first) sql += ",";
		first = false;
		sql += "?";
		params.addString(item); // 逐个添加字符串参数
	}
	sql += ") AND is_deleted = 0";

	// 传入SqlParams容器执行
	return sqlSession->executeUpdate(sql, params);
}