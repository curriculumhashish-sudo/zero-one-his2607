/*
 Copyright Zero One Star. All rights reserved.

 @Author: fa-mian-tuan
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
#include "NurseDAO2.h"
#include "NurseMapper2.h"

std::string NurseDAO2::queryConditionBuilder(const NurseQuery2::Wrapper& query, SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";
	if (query->name_code) {
		sqlCondition << " AND `bus_no`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->name_code.getValue(""));
	}
	if (query->execu_project) {
		sqlCondition << " AND `category_enum`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->execu_project.getValue(""));
	}
	if (query->use_project) {
		sqlCondition << " AND `content_json` LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->use_project.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t NurseDAO2::count(const NurseQuery2::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM wkf_service_request ";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<NurseDO2> NurseDAO2::selectWithPage(const NurseQuery2::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT id, bus_no, status_enum, content_json, execution_count, performer_id, category_enum FROM wkf_service_request ";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY `id` DESC ";
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);
	return sqlSession->executeQuery<NurseDO2>(sql, NurseMapper2(), params);
}
