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
#include "chinapres3MedicationRequestDAO.h"

std::string chinapres3MedicationRequestDAO::queryConditionBuilder(
	const chinapres3MedicationRequestQuery::Wrapper& query, SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";

	// 处方号模糊查询
	if (query->prescriptionNo) {
		sqlCondition << " AND `prescription_no` LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->prescriptionNo.getValue(""));
	}

	// 患者ID精准匹配
	if (query->patientId) {
		sqlCondition << " AND patient_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
	}

	// 就诊ID精准匹配
	if (query->encounterId) {
		sqlCondition << " AND encounter_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
	}

	// 处方状态筛选
	if (query->statusEnum) {
		sqlCondition << " AND status_enum=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->statusEnum.getValue(""));
	}

	return sqlCondition.str();
}

uint64_t chinapres3MedicationRequestDAO::count(const chinapres3MedicationRequestQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM med_medication_request";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<Ptrchinapres3MedicationRequestDO> chinapres3MedicationRequestDAO::selectWithPage(
	const chinapres3MedicationRequestQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT id,prescription_no,patient_id,encounter_id,status_enum,"
		"rx_type_code,create_by,create_at,update_by,update_at FROM med_medication_request";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY IFNULL(`update_at`, `create_at`) DESC, `id` DESC";
	sql += " LIMIT " + std::to_string((query->pageIndex - 1) * query->pageSize)
		+ "," + std::to_string(query->pageSize);
	chinapres3MedicationRequestMapper mapper;
	return sqlSession->executeQuery<Ptrchinapres3MedicationRequestDO>(sql, mapper, params);
}

Ptrchinapres3MedicationRequestDO chinapres3MedicationRequestDAO::selectById(const std::string& id)
{
	SqlParams params;
	std::string sql = "SELECT id,prescription_no,patient_id,encounter_id,status_enum,"
		"rx_type_code,create_by,create_at,update_by,update_at FROM med_medication_request WHERE id=?";
	SQLPARAMS_PUSH(params, "s", std::string, id);
	chinapres3MedicationRequestMapper mapper;
	auto list = sqlSession->executeQuery<Ptrchinapres3MedicationRequestDO>(sql, mapper, params);
	if (list.empty()) return nullptr;
	return list.front();
}