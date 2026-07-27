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
#include "chinapres3ChargeItemDAO.h"

std::string chinapres3ChargeItemDAO::queryConditionBuilder(
	const chinapres3ChargeItemQuery::Wrapper& query, SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";

	if (query->serviceTable) {
		sqlCondition << " AND service_table=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->serviceTable.getValue(""));
	}
	if (query->serviceId) {
		sqlCondition << " AND service_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->serviceId.getValue(""));
	}
	if (query->prescriptionNo) {
		sqlCondition << " AND prescription_no=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->prescriptionNo.getValue(""));
	}
	if (query->patientId) {
		sqlCondition << " AND patient_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
	}
	if (query->encounterId) {
		sqlCondition << " AND encounter_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
	}

	return sqlCondition.str();
}

uint64_t chinapres3ChargeItemDAO::count(const chinapres3ChargeItemQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM adm_charge_item";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<Ptrchinapres3ChargeItemDO> chinapres3ChargeItemDAO::selectWithPage(
	const chinapres3ChargeItemQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT id,service_table,service_id,definition_id,patient_id,encounter_id,"
		"prescription_no,total_price,quantity_value,quantity_unit,unit_price,account_id,org_id,"
		"condition_id,encounter_diagnosis_id,generate_source_enum,context_enum,"
		"create_by,create_at,update_by,update_at FROM adm_charge_item";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY IFNULL(`update_at`, `create_at`) DESC, `id` DESC";
	sql += " LIMIT " + std::to_string((query->pageIndex - 1) * query->pageSize)
		+ "," + std::to_string(query->pageSize);
	chinapres3ChargeItemMapper mapper;
	return sqlSession->executeQuery<Ptrchinapres3ChargeItemDO>(sql, mapper, params);
}

Ptrchinapres3ChargeItemDO chinapres3ChargeItemDAO::selectById(const std::string& id)
{
	SqlParams params;
	std::string sql = "SELECT id,service_table,service_id,definition_id,patient_id,encounter_id,"
		"prescription_no,total_price,quantity_value,quantity_unit,unit_price,account_id,org_id,"
		"condition_id,encounter_diagnosis_id,generate_source_enum,context_enum,"
		"create_by,create_at,update_by,update_at FROM adm_charge_item WHERE id=?";
	SQLPARAMS_PUSH(params, "s", std::string, id);
	chinapres3ChargeItemMapper mapper;
	auto list = sqlSession->executeQuery<Ptrchinapres3ChargeItemDO>(sql, mapper, params);
	if (list.empty()) return nullptr;
	return list.front();
}
