/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
 @Date: 2026/07/27 00:00:00

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
#include "OutpatientTrendDAO.h"
#include "OutpatientTrendMapper.h"

std::string OutpatientTrendDAO::queryConditionBuilder(const OutpatientTrendQuery::Wrapper& query, SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";
	// Exclude soft-deleted records
	sqlCondition << " AND is_deleted = 0";
	// Encounter start time lower bound
	if (query->dateFrom) {
		sqlCondition << " AND start_time >=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->dateFrom.getValue(""));
	}
	// Encounter start time upper bound
	if (query->dateTo) {
		sqlCondition << " AND start_time <=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->dateTo.getValue(""));
	}
	// Organization filter (data isolation)
	if (query->organizationId) {
		sqlCondition << " AND organization_id = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->organizationId.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t OutpatientTrendDAO::count(const OutpatientTrendQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM adm_encounter ";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<GetTrendDO> OutpatientTrendDAO::selectEncounters(const OutpatientTrendQuery::Wrapper& query)
{
	SqlParams params;
	// Column names match real adm_encounter table fields, aligned with GetTrendDO definition
	std::string sql =
		"SELECT id, patient_id, start_time, end_time, organization_id, is_deleted, status_enum "
		"FROM adm_encounter ";
	sql += queryConditionBuilder(query, params);
	// Sort by encounter start time ascending for trend chart ordering
	sql += " ORDER BY start_time ASC ";
	// Pagination with null-safe defaults (consistent with DiseaseDistributionDAO pattern)
	auto pageIndex = query->pageIndex.getValue(1);
	auto pageSize = query->pageSize.getValue(10);
	sql += " LIMIT " + std::to_string((pageIndex - 1) * pageSize) + "," + std::to_string(pageSize);

	return sqlSession->executeQuery<GetTrendDO>(sql, OutpatientTrendMapper(), params);
}
