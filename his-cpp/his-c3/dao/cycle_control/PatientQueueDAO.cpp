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
#include "PatientQueueDAO.h"
#include "PatientQueueMapper.h"

string PatientQueueDAO::queryConditionBuilder(const PatientQueueQuery::Wrapper& query, SqlParams& params)
{
	stringstream sql;
	sql << " WHERE e.class_enum = 'IMP' AND e.is_deleted = 0 AND e.status_enum = 'IN_PROGRESS'";
	if (query->wardId) {
		// 按病区筛选：就诊需有该病区的ACTIVE位置记录
		sql << " AND EXISTS (SELECT 1 FROM adm_encounter_location el_ward"
			" JOIN adm_location l_ward ON l_ward.id = el_ward.location_id"
			" WHERE el_ward.encounter_id = e.id AND el_ward.form_enum = 'WARD'"
			" AND el_ward.status_enum = 'ACTIVE' AND l_ward.id = ?)";
		SQLPARAMS_PUSH(params, "s", string, query->wardId.getValue(""));
	}
	if (query->keyword) {
		string kw = query->keyword.getValue("");
		sql << " AND (e.bus_no LIKE CONCAT('%',?,'%')"
			" OR p.name LIKE CONCAT('%',?,'%')"
			" OR EXISTS (SELECT 1 FROM adm_encounter_location el_kw"
			" JOIN adm_location l_kw ON l_kw.id = el_kw.location_id"
			" WHERE el_kw.encounter_id = e.id AND el_kw.form_enum = 'BED'"
			" AND el_kw.status_enum = 'ACTIVE' AND l_kw.name LIKE CONCAT('%',?,'%')))";
		SQLPARAMS_PUSH(params, "s", string, kw);
		SQLPARAMS_PUSH(params, "s", string, kw);
		SQLPARAMS_PUSH(params, "s", string, kw);
	}
	if (query->statusEnum) {
		sql << " AND e.status_enum = ?";
		SQLPARAMS_PUSH(params, "s", string, query->statusEnum.getValue(""));
	}
	return sql.str();
}

uint64_t PatientQueueDAO::count(const PatientQueueQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM adm_encounter e"
		" JOIN adm_patient p ON p.id = e.patient_id";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

list<PatientQueueDO> PatientQueueDAO::selectWithPage(const PatientQueueQuery::Wrapper& query)
{
	SqlParams params;
	string sql = R"(SELECT
		e.id AS encounter_id,
		p.id AS patient_id,
		p.name AS patient_name,
		p.gender_enum AS gender,
		TIMESTAMPDIFF(YEAR, p.birth_date, CURDATE()) AS age,
		(SELECT c.description FROM adm_encounter_diagnosis ed
			JOIN cln_condition c ON c.id = ed.condition_id
			WHERE ed.encounter_id = e.id AND ed.primary_flag = 1 LIMIT 1) AS diagnosis_summary,
		(SELECT l.name FROM adm_encounter_location el
			JOIN adm_location l ON l.id = el.location_id
			WHERE el.encounter_id = e.id AND el.form_enum = 'BED' AND el.status_enum = 'ACTIVE' LIMIT 1) AS bed_name,
		(SELECT l.id FROM adm_encounter_location el
			JOIN adm_location l ON l.id = el.location_id
			WHERE el.encounter_id = e.id AND el.form_enum = 'BED' AND el.status_enum = 'ACTIVE' LIMIT 1) AS bed_id,
		(SELECT l.id FROM adm_encounter_location el
			JOIN adm_location l ON l.id = el.location_id
			WHERE el.encounter_id = e.id AND el.form_enum = 'WARD' AND el.status_enum = 'ACTIVE' LIMIT 1) AS ward_id,
		(SELECT l.name FROM adm_encounter_location el
			JOIN adm_location l ON l.id = el.location_id
			WHERE el.encounter_id = e.id AND el.form_enum = 'WARD' AND el.status_enum = 'ACTIVE' LIMIT 1) AS ward_name,
		(SELECT l.name FROM adm_encounter_location el
			JOIN adm_location l ON l.id = el.location_id
			WHERE el.encounter_id = e.id AND el.form_enum = 'HOUSE' AND el.status_enum = 'ACTIVE' LIMIT 1) AS house_name,
		(SELECT ac.balance FROM adm_account ac
			WHERE ac.encounter_id = e.id AND ac.is_deleted = 0 AND ac.default_flag = 1 LIMIT 1) AS account_balance,
		e.status_enum AS status_enum,
		e.bus_no AS encounter_bus_no,
		e.start_time AS admit_time,
		e.organization_id AS organization_id,
		(SELECT o.name FROM adm_organization o WHERE o.id = e.organization_id) AS organization_name
		FROM adm_encounter e
		JOIN adm_patient p ON p.id = e.patient_id)";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY e.start_time DESC, e.id DESC";
	sql += " LIMIT " + to_string((query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10));
	sql += "," + to_string(query->pageSize.getValue(10));
	return sqlSession->executeQuery<PatientQueueDO>(sql, PatientQueueMapper(), params);
}
