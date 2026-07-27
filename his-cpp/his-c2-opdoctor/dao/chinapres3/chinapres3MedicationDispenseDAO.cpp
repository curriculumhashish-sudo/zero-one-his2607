/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c2-opdoctor
 @Date: 2025/07/26

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
#include "chinapres3MedicationDispenseDAO.h"

std::string chinapres3MedicationDispenseDAO::queryConditionBuilder(
	const chinapres3MedicationDispenseQuery::Wrapper& query, SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";

	if (query->medReqId) {
		sqlCondition << " AND med_req_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->medReqId.getValue(""));
	}
	if (query->patientId) {
		sqlCondition << " AND patient_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
	}
	if (query->encounterId) {
		sqlCondition << " AND encounter_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
	}
	if (query->busNo) {
		sqlCondition << " AND bus_no=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->busNo.getValue(""));
	}
	if (query->statusEnum) {
		sqlCondition << " AND status_enum=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->statusEnum.getValue(""));
	}

	return sqlCondition.str();
}

uint64_t chinapres3MedicationDispenseDAO::count(const chinapres3MedicationDispenseQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM med_medication_dispense";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<Ptrchinapres3MedicationDispenseDO> chinapres3MedicationDispenseDAO::selectWithPage(
	const chinapres3MedicationDispenseQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT id,med_req_id,patient_id,encounter_id,bus_no,status_enum,"
		"dispense_enum,location_id,create_by,create_at,update_by,update_at FROM med_medication_dispense";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY IFNULL(`update_at`, `create_at`) DESC, `id` DESC";
	sql += " LIMIT " + std::to_string((query->pageIndex - 1) * query->pageSize)
		+ "," + std::to_string(query->pageSize);
	chinapres3MedicationDispenseMapper mapper;
	return sqlSession->executeQuery<Ptrchinapres3MedicationDispenseDO>(sql, mapper, params);
}

Ptrchinapres3MedicationDispenseDO chinapres3MedicationDispenseDAO::selectById(const std::string& id)
{
	SqlParams params;
	std::string sql = "SELECT id,med_req_id,patient_id,encounter_id,bus_no,status_enum,"
		"dispense_enum,location_id,create_by,create_at,update_by,update_at FROM med_medication_dispense WHERE id=?";
	SQLPARAMS_PUSH(params, "s", std::string, id);
	chinapres3MedicationDispenseMapper mapper;
	auto list = sqlSession->executeQuery<Ptrchinapres3MedicationDispenseDO>(sql, mapper, params);
	if (list.empty()) return nullptr;
	return list.front();
}
