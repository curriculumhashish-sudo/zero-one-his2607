/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#include "EncounterManageDAO.h"
#include "EncounterManageMapper.h"

namespace {
template<class T>
void appendEncounterFilter(const T& query, std::string& sql, SqlParams& params)
{
	if (query->keyword && !query->keyword->empty()) {
		sql += " AND (p.name LIKE CONCAT('%',?,'%') OR p.py_str LIKE CONCAT('%',?,'%') OR p.wb_str LIKE CONCAT('%',?,'%') OR bed.name LIKE CONCAT('%',?,'%'))";
		for (int i = 0; i < 4; ++i) SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
	}
	if (query->wardId && !query->wardId->empty()) {
		sql += " AND ward.id = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->wardId.getValue(""));
	}
	if (query->houseId && !query->houseId->empty()) {
		sql += " AND house.id = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->houseId.getValue(""));
	}
	if (query->nurseStationStatusFilter && !query->nurseStationStatusFilter->empty()) {
		sql += " AND e.status_enum = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->nurseStationStatusFilter.getValue(""));
	}
}

const char* BASE_FROM =
	" FROM adm_encounter e JOIN adm_patient p ON p.id=e.patient_id AND p.is_deleted=0 "
	" LEFT JOIN adm_organization org ON org.id=e.organization_id AND org.is_deleted=0 "
	" LEFT JOIN adm_encounter_location el ON el.encounter_id=e.id AND el.is_deleted=0 AND el.status_enum='ACTIVE' AND el.form_enum='BED' "
	" LEFT JOIN adm_location bed ON bed.id=el.location_id AND bed.is_deleted=0 "
	" LEFT JOIN adm_location house ON house.id=bed.parent_id AND house.is_deleted=0 "
	" LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0 "
	" WHERE e.is_deleted=0 AND e.class_enum='IMP' ";
}

uint64_t EncounterManageDAO::countPatients(const PagePatientQueueQuery::Wrapper& query)
{
	std::string sql = std::string("SELECT COUNT(DISTINCT e.id)") + BASE_FROM;
	SqlParams params; appendEncounterFilter(query, sql, params);
	if (query->startDate && !query->startDate->empty()) { sql += " AND e.start_time>=?"; SQLPARAMS_PUSH(params,"dt",std::string,query->startDate.getValue("")); }
	if (query->endDate && !query->endDate->empty()) { sql += " AND e.start_time<=?"; SQLPARAMS_PUSH(params,"dt",std::string,query->endDate.getValue("")); }
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PatientQueueItemDTO::Wrapper> EncounterManageDAO::selectPatients(const PagePatientQueueQuery::Wrapper& query)
{
	std::string sql = "SELECT e.id encounter_id,e.bus_no encounter_bus_no,e.patient_id,p.name patient_name,p.gender_enum,"
		"CONCAT(TIMESTAMPDIFF(YEAR,p.birth_date,CURDATE()),'岁') age_text,p.py_str pinyin_code,p.wb_str wubi_code,"
		"e.status_enum,e.priority_enum,e.start_time,e.end_time,e.organization_id,org.name organization_name,"
		"bed.id bed_id,bed.name bed_name,ward.id ward_id,ward.name ward_name,house.name house_name,"
		"COALESCE((SELECT GROUP_CONCAT(d.description ORDER BY d.primary_flag DESC,d.rank SEPARATOR '；') FROM adm_encounter_diagnosis d WHERE d.encounter_id=e.id AND d.is_deleted=0),'') diagnosis_summary,"
		"COALESCE((SELECT pr.name FROM adm_encounter_participant ep JOIN adm_practitioner pr ON pr.id=ep.practitioner_id AND pr.is_deleted=0 WHERE ep.encounter_id=e.id AND ep.is_deleted=0 AND ep.status_enum='ACTIVE' AND ep.role_code IN ('ATTENDING_DOCTOR','DOCTOR') LIMIT 1),'') doctor_name,"
		"COALESCE((SELECT pr.name FROM adm_encounter_participant ep JOIN adm_practitioner pr ON pr.id=ep.practitioner_id AND pr.is_deleted=0 WHERE ep.encounter_id=e.id AND ep.is_deleted=0 AND ep.status_enum='ACTIVE' AND ep.role_code IN ('PRIMARY_NURSE','NURSE') LIMIT 1),'') nurse_name,"
		"COALESCE((SELECT a.id FROM adm_account a WHERE a.encounter_id=e.id AND a.is_deleted=0 ORDER BY a.default_flag DESC LIMIT 1),'') account_id,"
		"COALESCE((SELECT a.balance FROM adm_account a WHERE a.encounter_id=e.id AND a.is_deleted=0 ORDER BY a.default_flag DESC LIMIT 1),0) account_balance,"
		"COALESCE((SELECT SUM(ci.total_price) FROM adm_charge_item ci WHERE ci.encounter_id=e.id AND ci.is_deleted=0 AND ci.status_enum='PENDING_SETTLEMENT'),0) pending_settlement_total,"
		"COALESCE((SELECT SUM(ci.total_price) FROM adm_charge_item ci WHERE ci.encounter_id=e.id AND ci.is_deleted=0 AND ci.status_enum='SETTLED'),0) settled_total,"
		"COALESCE((SELECT a.balance FROM adm_account a WHERE a.encounter_id=e.id AND a.is_deleted=0 ORDER BY a.default_flag DESC LIMIT 1),0)-COALESCE((SELECT SUM(ci.total_price) FROM adm_charge_item ci WHERE ci.encounter_id=e.id AND ci.is_deleted=0 AND ci.status_enum='PENDING_SETTLEMENT'),0) available_balance,"
		"((SELECT COUNT(*) FROM med_medication_request mr WHERE mr.encounter_id=e.id AND mr.is_deleted=0 AND mr.status_enum='ACTIVE' AND COALESCE(mr.generate_source_enum,'')<>'NURSE_PRICING')+(SELECT COUNT(*) FROM wkf_service_request sr WHERE sr.encounter_id=e.id AND sr.is_deleted=0 AND sr.status_enum='ACTIVE' AND COALESCE(sr.generate_source_enum,'')<>'NURSE_PRICING')) pending_audit_count,"
		"((SELECT COUNT(*) FROM med_medication_request mr WHERE mr.encounter_id=e.id AND mr.is_deleted=0 AND mr.status_enum='VERIFIED')+(SELECT COUNT(*) FROM wkf_service_request sr WHERE sr.encounter_id=e.id AND sr.is_deleted=0 AND sr.status_enum='VERIFIED')) pending_execute_count" + std::string(BASE_FROM);
	SqlParams params; appendEncounterFilter(query, sql, params);
	if (query->startDate && !query->startDate->empty()) { sql += " AND e.start_time>=?"; SQLPARAMS_PUSH(params,"dt",std::string,query->startDate.getValue("")); }
	if (query->endDate && !query->endDate->empty()) { sql += " AND e.start_time<=?"; SQLPARAMS_PUSH(params,"dt",std::string,query->endDate.getValue("")); }
	std::string sort = query->sortBy.getValue("startTime") == "patientName" ? "p.name" : (query->sortBy.getValue("") == "bedName" ? "bed.name" : "e.start_time");
	sql += " ORDER BY " + sort + (query->sortDirection.getValue("DESC") == "ASC" ? " ASC" : " DESC") + " LIMIT ?,?";
	SQLPARAMS_PUSH(params,"ull",uint64_t,(query->pageIndex.getValue(1)-1)*query->pageSize.getValue(10));
	SQLPARAMS_PUSH(params,"ull",uint64_t,query->pageSize.getValue(10));
	return sqlSession->executeQuery<PatientQueueItemDTO::Wrapper>(sql, PatientQueueMapper(), params);
}

uint64_t EncounterManageDAO::countPatients(const PatientStatisticsQuery::Wrapper& query)
{
	std::string sql=std::string("SELECT COUNT(DISTINCT e.id)")+BASE_FROM; SqlParams p; appendEncounterFilter(query,sql,p); return sqlSession->executeQueryNumerical(sql,p);
}
uint64_t EncounterManageDAO::countPendingAudit(const PatientStatisticsQuery::Wrapper& query)
{
	std::string filter=std::string(BASE_FROM); SqlParams p; appendEncounterFilter(query,filter,p);
	std::string sql="SELECT COALESCE(SUM((SELECT COUNT(*) FROM med_medication_request mr WHERE mr.encounter_id=e.id AND mr.is_deleted=0 AND mr.status_enum='ACTIVE' AND COALESCE(mr.generate_source_enum,'')<>'NURSE_PRICING')+(SELECT COUNT(*) FROM wkf_service_request sr WHERE sr.encounter_id=e.id AND sr.is_deleted=0 AND sr.status_enum='ACTIVE' AND COALESCE(sr.generate_source_enum,'')<>'NURSE_PRICING')),0)"+filter;
	return sqlSession->executeQueryNumerical(sql,p);
}
uint64_t EncounterManageDAO::countPendingExecute(const PatientStatisticsQuery::Wrapper& query)
{
	std::string filter=std::string(BASE_FROM); SqlParams p; appendEncounterFilter(query,filter,p);
	std::string sql="SELECT COALESCE(SUM((SELECT COUNT(*) FROM med_medication_request mr WHERE mr.encounter_id=e.id AND mr.is_deleted=0 AND mr.status_enum='VERIFIED')+(SELECT COUNT(*) FROM wkf_service_request sr WHERE sr.encounter_id=e.id AND sr.is_deleted=0 AND sr.status_enum='VERIFIED')),0)"+filter;
	return sqlSession->executeQueryNumerical(sql,p);
}
