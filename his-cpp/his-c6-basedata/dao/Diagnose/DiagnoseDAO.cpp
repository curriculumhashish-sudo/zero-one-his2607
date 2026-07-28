/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/24

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
#include "DiagnoseDAO.h"
#include "DiagnoseSelectMapper.h"

namespace
{
	void appendSourceFilter(
		std::stringstream& sqlCondition,
		SqlParams& params,
		const std::vector<std::string>& sourceCodes)
	{
		sqlCondition << " AND `source_enum` IN (";
		for (std::size_t index = 0; index < sourceCodes.size(); ++index)
		{
			if (index != 0)
			{
				sqlCondition << ",";
			}
			sqlCondition << "?";
			SQLPARAMS_PUSH(params, "s", std::string, sourceCodes[index]);
		}
		sqlCondition << ")";
	}
}

std::string DiagnoseDAO::queryConditionBuilder(
	const DiagnoseSelectQuery::Wrapper& query,
	SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE `is_deleted`=0 AND `status_enum`='ACTIVE'";

	const std::string keyword = query->keyword.getValue("");
	if (!keyword.empty())
	{
		sqlCondition
			<< " AND (LOWER(`name`) LIKE CONCAT('%',LOWER(?),'%')"
			<< " OR LOWER(`condition_code`) LIKE CONCAT('%',LOWER(?),'%')"
			<< " OR LOWER(`py_str`) LIKE CONCAT('%',LOWER(?),'%')"
			<< " OR LOWER(`wb_str`) LIKE CONCAT('%',LOWER(?),'%'))";
		for (int index = 0; index < 4; ++index)
		{
			SQLPARAMS_PUSH(params, "s", std::string, keyword);
		}
	}

	const std::string diagnoseKindCode = query->diagnoseKindCode.getValue("");
	if (diagnoseKindCode == "1")
	{
		appendSourceFilter(
			sqlCondition,
			params,
			{"general_dx", "chronic_spec", "disease_pkg", "paid_dx",
			 "day_surgery_dx", "tumor_morph"});
	}
	else if (diagnoseKindCode == "2")
	{
		appendSourceFilter(sqlCondition, params, {"tcm_dx", "tcm_syndrome"});
	}
	return sqlCondition.str();
}

uint64_t DiagnoseDAO::count(const DiagnoseSelectQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM `cln_condition_definition`";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<ClnConditionDefinitionDO> DiagnoseDAO::selectWithPage(
	const DiagnoseSelectQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT `id`,`condition_code`,`name`,`py_str`,`wb_str`,"
		"`source_enum`,COALESCE(`yb_flag`,0),`status_enum` "
		"FROM `cln_condition_definition`";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY COALESCE(`updated_at`,`created_at`) DESC,`id` DESC";

	const uint64_t pageIndex = query->pageIndex.getValue(1);
	const uint64_t pageSize = query->pageSize.getValue(10);
	const uint64_t offset = (pageIndex - 1) * pageSize;
	sql += " LIMIT " + std::to_string(offset) + "," + std::to_string(pageSize);
	return sqlSession->executeQuery<ClnConditionDefinitionDO>(
		sql,
		DiagnoseSelectMapper(),
		params);
}
