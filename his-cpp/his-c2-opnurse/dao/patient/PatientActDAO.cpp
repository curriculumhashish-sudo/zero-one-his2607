/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/24 18:30:00

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
#include "PatientActDAO.h"
#include <sstream>

ServiceRequestResult ServiceRequestResultMapper::mapper(ResultSet* rs) const
{
	ServiceRequestResult result;
	// Map base ServiceRequestDO fields
	result.request.setId(rs->getString("id"));
	if (!rs->isNull("patient_id"))
		result.request.setPatientId(rs->getString("patient_id"));
	if (!rs->isNull("encounter_id"))
		result.request.setEncounterId(rs->getString("encounter_id"));
	if (!rs->isNull("status_enum"))
		result.request.setStatusEnum(rs->getString("status_enum"));
	if (!rs->isNull("entrust_content"))
		result.request.setEntrustContent(rs->getString("entrust_content"));
	result.request.setPerformFlag(rs->getInt("perform_flag"));
	if (!rs->isNull("activity_id"))
		result.request.setActivityId(rs->getString("activity_id"));
	if (!rs->isNull("category_enum"))
		result.request.setCategoryEnum(rs->getString("category_enum"));
	if (!rs->isNull("quantity"))
		result.request.setQuantity(rs->getDouble("quantity"));
	if (!rs->isNull("unit_code"))
		result.request.setUnitCode(rs->getString("unit_code"));
	if (!rs->isNull("authored_time"))
		result.request.setAuthoredTime(rs->getString("authored_time"));
	if (!rs->isNull("remark"))
		result.request.setRemark(rs->getString("remark"));
	if (!rs->isNull("total_price")) {
		std::ostringstream oss;
		oss.setf(std::ios::fixed);
		oss.precision(2);
		oss << rs->getDouble("total_price");
		result.request.setTotalPrice(oss.str());
	}

	// Map extended joined fields
	if (!rs->isNull("request_id"))
		result.requestId = rs->getString("request_id");
	if (!rs->isNull("request_type_name"))
		result.requestTypeName = rs->getString("request_type_name");
	if (!rs->isNull("specification"))
		result.specification = rs->getString("specification");
	if (!rs->isNull("status_name"))
		result.statusName = rs->getString("status_name");
	result.plannedCount = rs->getInt("planned_count");
	result.executedCount = rs->getInt("executed_count");
	if (!rs->isNull("executor_name"))
		result.executorName = rs->getString("executor_name");
	if (!rs->isNull("executed_at"))
		result.executedAt = rs->getString("executed_at");
	result.version = rs->getInt("version");
	if (!rs->isNull("print_types"))
		result.printTypes = rs->getString("print_types");
	return result;
}

std::list<ServiceRequestResult> PatientActDAO::queryServiceRequests(
	const std::string& patientId, const std::string& encounterId,
	const std::string& categoryEnum, const std::string& statusEnum,
	int performFlag)
{
	std::ostringstream sql;
	sql << "SELECT sr.id, sr.patient_id, sr.encounter_id, sr.status_enum, "
		<< "sr.entrust_content, sr.perform_flag, sr.activity_id, "
		<< "sr.category_enum, sr.quantity, sr.unit_code, sr.authored_time, sr.remark, "
		<< "ci.total_price, "
		<< "sr.request_id, "
		<< "cd.display_name AS request_type_name, "
		<< "cid.specification, "
		<< "st.display_name AS status_name, "
		<< "CAST(sr.quantity AS SIGNED) AS planned_count, "
		<< "COALESCE(er.performed_count, 0) AS executed_count, "
		<< "er.executor_name, "
		<< "er.executed_at, "
		<< "COALESCE(sr.version, 1) AS version, "
		<< "COALESCE(sr.print_types, '') AS print_types "
		<< "FROM wkf_service_request sr "
		<< "LEFT JOIN adm_charge_item ci ON ci.service_table = 'wkf_service_request' "
		<< "AND ci.service_id = sr.id "
		<< "LEFT JOIN adm_charge_item_definition cid ON sr.activity_id = cid.id "
		<< "LEFT JOIN adm_code_dict cd ON cd.code = sr.category_enum "
		<< "AND cd.category = 'service_request_category' "
		<< "LEFT JOIN adm_code_dict st ON st.code = sr.status_enum "
		<< "AND st.category = 'service_request_status' "
		<< "LEFT JOIN wkf_execution_record er ON er.request_id = sr.id "
		<< "AND er.request_table = 'wkf_service_request' "
		<< "WHERE sr.is_deleted = 0 ";

	SqlParams params;
	if (!patientId.empty()) {
		sql << "AND sr.patient_id = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, patientId);
	}
	if (!encounterId.empty()) {
		sql << "AND sr.encounter_id = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	}
	if (!categoryEnum.empty()) {
		sql << "AND sr.category_enum = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, categoryEnum);
	}
	if (!statusEnum.empty()) {
		sql << "AND sr.status_enum = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
	}
	if (performFlag >= 0) {
		sql << "AND sr.perform_flag = ? ";
		SQLPARAMS_PUSH(params, "i", int, performFlag);
	}

	sql << "ORDER BY sr.authored_time DESC";

	ServiceRequestResultMapper mapper;
	return sqlSession->executeQuery<ServiceRequestResult>(sql.str(), mapper, params);
}
