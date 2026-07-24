/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c4
 @Date: 2026/07/18 22:40:00

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
#include "DispenseDAO.h"
#include "DispenseMapper.h"

std::string DispenseDAO::queryConditionBuilder(const DispenseSummaryQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	// only rows that already belong to a summary bill
	sqlCondition << " WHERE is_deleted=0 AND summary_no IS NOT NULL";
	if (query->statusEnum && !query->statusEnum->empty()) {
		sqlCondition << " AND status_enum=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->statusEnum.getValue(""));
	}
	if (query->dispenseDate && !query->dispenseDate->empty()) {
		sqlCondition << " AND DATE(planned_dispense_time)=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->dispenseDate.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t DispenseDAO::countSummary(const DispenseSummaryQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(DISTINCT summary_no) FROM med_medication_dispense ";
	// append query condition
	sql += queryConditionBuilder(query, params);
	// execute query
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<DispenseSummaryDO> DispenseDAO::selectSummaryWithPage(const DispenseSummaryQuery::Wrapper& query)
{
	SqlParams params;
	string sql =
		"SELECT summary_no, status_enum, MAX(des_location_id), COUNT(*), "
		"IFNULL(DATE_FORMAT(MAX(planned_dispense_time), '%Y-%m-%d %H:%i:%s'), ''), "
		"IFNULL(DATE_FORMAT(MAX(dispense_time), '%Y-%m-%d %H:%i:%s'), '') "
		"FROM med_medication_dispense ";
	// append query condition
	sql += queryConditionBuilder(query, params);
	// aggregate one row per summary bill
	sql += " GROUP BY summary_no, status_enum ORDER BY summary_no DESC ";
	// append pagination
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// execute query
	return sqlSession->executeQuery<DispenseSummaryDO>(sql, DispenseSummaryMapper(), params);
}

std::list<DispenseDetailDO> DispenseDAO::selectDetailBySummaryNo(const std::string& summaryNo)
{
	string sql =
		"SELECT d.id, IFNULL(m.name, ''), d.quantity, d.unit_code "
		"FROM med_medication_dispense d "
		"LEFT JOIN med_medication_definition m ON d.medication_id = m.id AND m.is_deleted=0 "
		"WHERE d.is_deleted=0 AND d.summary_no=? "
		"ORDER BY d.id ASC";
	return sqlSession->executeQuery<DispenseDetailDO>(sql, DispenseDetailMapper(), "%s", summaryNo);
}
