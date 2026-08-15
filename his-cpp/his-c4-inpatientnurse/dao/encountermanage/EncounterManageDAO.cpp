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
std::vector<std::string> splitIds(const oatpp::String& value)
{
	std::vector<std::string> ids;
	std::string source = value.getValue("");
	std::size_t begin = 0;
	while (begin <= source.size()) {
		auto end = source.find(',', begin);
		std::string id = source.substr(begin, end == std::string::npos ? std::string::npos : end - begin);
		auto first = id.find_first_not_of(" \t\r\n");
		auto last = id.find_last_not_of(" \t\r\n");
		if (first != std::string::npos) ids.emplace_back(id.substr(first, last - first + 1));
		if (end == std::string::npos) break;
		begin = end + 1;
	}
	return ids;
}

void appendIdList(const char* column, const std::vector<std::string>& ids, std::string& sql, SqlParams& params)
{
	if (ids.empty()) return;
	sql += std::string(" AND ") + column + " IN (";
	for (std::size_t i = 0; i < ids.size(); ++i) {
		if (i) sql += ',';
		sql += '?';
		SQLPARAMS_PUSH(params, "s", std::string, ids[i]);
	}
	sql += ')';
}

template<class T>
void appendEncounterFilter(const T& query, std::string& sql, SqlParams& params)
{
	if (query->keyword && !query->keyword->empty()) {
		sql += " AND (p.name LIKE CONCAT('%',?,'%') OR p.py_str LIKE CONCAT('%',?,'%') OR p.wb_str LIKE CONCAT('%',?,'%') OR bed.name LIKE CONCAT('%',?,'%'))";
		for (int i = 0; i < 4; ++i) SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
	}
	auto houseIds = splitIds(query->houseIds);
	auto wardIds = splitIds(query->wardIds);
	if (!houseIds.empty()) {
		appendIdList("house.id", houseIds, sql, params);
	} else if (!wardIds.empty()) {
		appendIdList("ward.id", wardIds, sql, params);
	} else {
		if (query->wardId && !query->wardId->empty()) {
			sql += " AND ward.id = ?";
			SQLPARAMS_PUSH(params, "s", std::string, query->wardId.getValue(""));
		}
		if (query->houseId && !query->houseId->empty()) {
			sql += " AND house.id = ?";
			SQLPARAMS_PUSH(params, "s", std::string, query->houseId.getValue(""));
		}
	}
	if (query->nurseStationStatusFilter == "SETTLED_DISCHARGED") {
		sql += " AND e.status_enum IN ('COMPLETED','DISCHARGED')";
	} else if (query->nurseStationStatusFilter && !query->nurseStationStatusFilter->empty()) {
		sql += " AND e.status_enum = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->nurseStationStatusFilter.getValue(""));
	} else {
		sql += " AND e.status_enum IN ('REGISTERED','IN_PROGRESS','ON_HOLD','PENDING')";
	}

	const auto executionDate = query->nurseStationExecutionDate.getValue("");
	const auto taskFilter = query->nurseStationTaskFilter.getValue("");
	const auto appendDate = [&](const char* alias, const char* planColumn, const char* fallbackColumn) {
		if (!executionDate.empty()) {
			sql += std::string(" AND DATE(COALESCE(") + alias + "." + planColumn + "," +
				alias + "." + fallbackColumn + "))=?";
			SQLPARAMS_PUSH(params, "s", std::string, executionDate);
		}
	};
	if (taskFilter == "pending-audit" || taskFilter == "pending-execute") {
		const char* status = taskFilter == "pending-audit" ? "ACTIVE" : "VERIFIED";
		sql += " AND (EXISTS (SELECT 1 FROM med_medication_request task_mr WHERE task_mr.encounter_id=e.id AND task_mr.is_deleted=0 AND task_mr.status_enum='";
		sql += status;
		sql += "' AND COALESCE(task_mr.generate_source_enum,'')<>'NURSE_PRICING'";
		appendDate("task_mr", "effective_dose_start", "req_authored_time");
		sql += ") OR EXISTS (SELECT 1 FROM wkf_service_request task_sr WHERE task_sr.encounter_id=e.id AND task_sr.is_deleted=0 AND task_sr.status_enum='";
		sql += status;
		sql += "' AND COALESCE(task_sr.generate_source_enum,'')<>'NURSE_PRICING'";
		appendDate("task_sr", "occurrence_start_time", "authored_time");
		sql += "))";
	} else if (taskFilter == "processed") {
		if (!executionDate.empty()) {
			sql += " AND (EXISTS (SELECT 1 FROM med_medication_request any_mr WHERE any_mr.encounter_id=e.id AND any_mr.is_deleted=0";
			appendDate("any_mr", "effective_dose_start", "req_authored_time");
			sql += ") OR EXISTS (SELECT 1 FROM wkf_service_request any_sr WHERE any_sr.encounter_id=e.id AND any_sr.is_deleted=0";
			appendDate("any_sr", "occurrence_start_time", "authored_time");
			sql += "))";
		}
		sql += " AND NOT EXISTS (SELECT 1 FROM med_medication_request task_mr WHERE task_mr.encounter_id=e.id AND task_mr.is_deleted=0 AND task_mr.status_enum IN ('ACTIVE','VERIFIED')";
		appendDate("task_mr", "effective_dose_start", "req_authored_time");
		sql += ") AND NOT EXISTS (SELECT 1 FROM wkf_service_request task_sr WHERE task_sr.encounter_id=e.id AND task_sr.is_deleted=0 AND task_sr.status_enum IN ('ACTIVE','VERIFIED')";
		appendDate("task_sr", "occurrence_start_time", "authored_time");
		sql += ")";
	} else if (!executionDate.empty()) {
		sql += " AND (EXISTS (SELECT 1 FROM med_medication_request task_mr WHERE task_mr.encounter_id=e.id AND task_mr.is_deleted=0";
		appendDate("task_mr", "effective_dose_start", "req_authored_time");
		sql += ") OR EXISTS (SELECT 1 FROM wkf_service_request task_sr WHERE task_sr.encounter_id=e.id AND task_sr.is_deleted=0";
		appendDate("task_sr", "occurrence_start_time", "authored_time");
		sql += "))";
	}
}

void appendUserScope(const std::string& userId, std::string& sql, SqlParams& params)
{
	sql += R"SQL(
		AND EXISTS (
			SELECT 1
			FROM adm_practitioner access_practitioner
			JOIN adm_pr_assign access_assignment
				ON access_assignment.practitioner_id=access_practitioner.id
				AND access_assignment.is_deleted=0
				AND access_assignment.active_flag=1
			WHERE access_practitioner.user_id=?
				AND access_practitioner.is_deleted=0
				AND access_practitioner.active_flag=1
				AND (access_assignment.location_id IN (bed.id,house.id,ward.id)
					OR access_assignment.org_id=e.organization_id)
		)
	)SQL";
	SQLPARAMS_PUSH(params, "s", std::string, userId);
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

uint64_t EncounterManageDAO::countPatients(
	const PagePatientQueueQuery::Wrapper& query, const std::string& userId)
{
	std::string sql = std::string("SELECT COUNT(DISTINCT e.id)") + BASE_FROM;
	SqlParams params; appendEncounterFilter(query, sql, params);
	appendUserScope(userId, sql, params);
	if (query->startDate && !query->startDate->empty()) { sql += " AND e.start_time>=?"; SQLPARAMS_PUSH(params,"dt",std::string,query->startDate.getValue("")); }
	if (query->endDate && !query->endDate->empty()) { sql += " AND e.start_time<=?"; SQLPARAMS_PUSH(params,"dt",std::string,query->endDate.getValue("")); }
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PatientQueueItemDTO::Wrapper> EncounterManageDAO::selectPatients(
	const PagePatientQueueQuery::Wrapper& query, const std::string& userId)
{
	const std::string ageSuffix = ZH_WORDS_GETTER("business-text.encounter.age-suffix");
	const std::string diagnosisSeparator = ZH_WORDS_GETTER("business-text.encounter.diagnosis-separator");
	SqlParams params;
	const std::string executionDate=query->nurseStationExecutionDate.getValue("");
	std::string medTaskDate,serviceTaskDate;
	if(!executionDate.empty()){
		medTaskDate=" AND DATE(COALESCE(mr.effective_dose_start,mr.req_authored_time))=?";
		serviceTaskDate=" AND DATE(COALESCE(sr.occurrence_start_time,sr.authored_time))=?";
		SQLPARAMS_PUSH(params,"s",std::string,executionDate);
		SQLPARAMS_PUSH(params,"s",std::string,executionDate);
	}
	std::string sql = "SELECT e.id encounter_id,e.bus_no encounter_bus_no,e.patient_id,p.name patient_name,p.gender_enum,"
		"CONCAT(TIMESTAMPDIFF(YEAR,p.birth_date,CURDATE()),'" + ageSuffix + "') age_text,p.py_str pinyin_code,p.wb_str wubi_code,"
		"e.status_enum,e.priority_enum,e.start_time,e.end_time,e.organization_id,org.name organization_name,"
		"bed.id bed_id,bed.name bed_name,ward.id ward_id,ward.name ward_name,house.id house_id,house.name house_name,"
		"COALESCE((SELECT GROUP_CONCAT(d.description ORDER BY d.primary_flag DESC,d.rank SEPARATOR '" + diagnosisSeparator + "') FROM adm_encounter_diagnosis d WHERE d.encounter_id=e.id AND d.is_deleted=0),'') diagnosis_summary,"
		"COALESCE((SELECT pr.name FROM adm_encounter_participant ep JOIN adm_practitioner pr ON pr.id=ep.practitioner_id AND pr.is_deleted=0 WHERE ep.encounter_id=e.id AND ep.is_deleted=0 AND ep.status_enum='ACTIVE' AND ep.role_code IN ('ATTENDING_DOCTOR','DOCTOR') LIMIT 1),'') doctor_name,"
		"COALESCE((SELECT pr.name FROM adm_encounter_participant ep JOIN adm_practitioner pr ON pr.id=ep.practitioner_id AND pr.is_deleted=0 WHERE ep.encounter_id=e.id AND ep.is_deleted=0 AND ep.status_enum='ACTIVE' AND ep.role_code IN ('PRIMARY_NURSE','NURSE') LIMIT 1),'') nurse_name,"
		"COALESCE((SELECT a.id FROM adm_account a WHERE a.encounter_id=e.id AND a.is_deleted=0 ORDER BY a.default_flag DESC LIMIT 1),'') account_id,"
		"COALESCE((SELECT a.balance FROM adm_account a WHERE a.encounter_id=e.id AND a.is_deleted=0 ORDER BY a.default_flag DESC LIMIT 1),0) account_balance,"
		"COALESCE((SELECT SUM(ci.total_price) FROM adm_charge_item ci WHERE ci.encounter_id=e.id AND ci.is_deleted=0 AND ci.status_enum='PENDING_SETTLEMENT'),0) pending_settlement_total,"
		"COALESCE((SELECT SUM(ci.total_price) FROM adm_charge_item ci WHERE ci.encounter_id=e.id AND ci.is_deleted=0 AND ci.status_enum='SETTLED'),0) settled_total,"
		"COALESCE((SELECT a.balance FROM adm_account a WHERE a.encounter_id=e.id AND a.is_deleted=0 ORDER BY a.default_flag DESC LIMIT 1),0)-COALESCE((SELECT SUM(ci.total_price) FROM adm_charge_item ci WHERE ci.encounter_id=e.id AND ci.is_deleted=0 AND ci.status_enum='PENDING_SETTLEMENT'),0) available_balance,"
		"((SELECT COUNT(*) FROM med_medication_request mr WHERE mr.encounter_id=e.id AND mr.is_deleted=0 AND mr.status_enum='ACTIVE' AND COALESCE(mr.generate_source_enum,'')<>'NURSE_PRICING'"+medTaskDate+")+(SELECT COUNT(*) FROM wkf_service_request sr WHERE sr.encounter_id=e.id AND sr.is_deleted=0 AND sr.status_enum='ACTIVE' AND COALESCE(sr.generate_source_enum,'')<>'NURSE_PRICING'"+serviceTaskDate+")) pending_audit_count,"
		"((SELECT COUNT(*) FROM med_medication_request mr WHERE mr.encounter_id=e.id AND mr.is_deleted=0 AND mr.status_enum='VERIFIED'"+medTaskDate+")+(SELECT COUNT(*) FROM wkf_service_request sr WHERE sr.encounter_id=e.id AND sr.is_deleted=0 AND sr.status_enum='VERIFIED'"+serviceTaskDate+")) pending_execute_count" + std::string(BASE_FROM);
	if(!executionDate.empty()){
		SQLPARAMS_PUSH(params,"s",std::string,executionDate);
		SQLPARAMS_PUSH(params,"s",std::string,executionDate);
	}
	appendEncounterFilter(query, sql, params);
	appendUserScope(userId, sql, params);
	if (query->startDate && !query->startDate->empty()) { sql += " AND e.start_time>=?"; SQLPARAMS_PUSH(params,"dt",std::string,query->startDate.getValue("")); }
	if (query->endDate && !query->endDate->empty()) { sql += " AND e.start_time<=?"; SQLPARAMS_PUSH(params,"dt",std::string,query->endDate.getValue("")); }
	const std::string sortBy = query->sortBy.getValue("");
	const std::string direction = query->sortDirection.getValue("DESC") == "ASC" ? " ASC" : " DESC";
	if (sortBy.empty()) {
		sql += " ORDER BY COALESCE(ward.display_order,0),COALESCE(house.display_order,0),"
			"COALESCE(bed.display_order,0),bed.name,p.name,e.id";
	} else {
		const std::string sort = sortBy == "patientName" ? "p.name" :
			(sortBy == "bedName" ?
			 "ward.display_order,house.display_order,bed.display_order,bed.name" : "e.start_time");
		sql += " ORDER BY " + sort + direction + ", e.id ASC";
	}
	sql += " LIMIT ?,?";
	SQLPARAMS_PUSH(params,"ull",uint64_t,(query->pageIndex.getValue(1)-1)*query->pageSize.getValue(10));
	SQLPARAMS_PUSH(params,"ull",uint64_t,query->pageSize.getValue(10));
	return sqlSession->executeQuery<PatientQueueItemDTO::Wrapper>(sql, PatientQueueMapper(), params);
}

uint64_t EncounterManageDAO::countPatients(
	const PatientStatisticsQuery::Wrapper& query, const std::string& userId)
{
	std::string sql=std::string("SELECT COUNT(DISTINCT e.id)")+BASE_FROM; SqlParams p;
	appendEncounterFilter(query,sql,p); appendUserScope(userId,sql,p);
	if (query->startDate && !query->startDate->empty()) { sql += " AND e.start_time>=?"; SQLPARAMS_PUSH(p,"dt",std::string,query->startDate.getValue("")); }
	if (query->endDate && !query->endDate->empty()) { sql += " AND e.start_time<=?"; SQLPARAMS_PUSH(p,"dt",std::string,query->endDate.getValue("")); }
	return sqlSession->executeQueryNumerical(sql,p);
}
uint64_t EncounterManageDAO::countPendingAudit(
	const PatientStatisticsQuery::Wrapper& query, const std::string& userId)
{
	SqlParams p;
	const std::string executionDate=query->nurseStationExecutionDate.getValue("");
	std::string medDate,serviceDate;
	if(!executionDate.empty()){medDate=" AND DATE(COALESCE(mr.effective_dose_start,mr.req_authored_time))=?";serviceDate=" AND DATE(COALESCE(sr.occurrence_start_time,sr.authored_time))=?";SQLPARAMS_PUSH(p,"s",std::string,executionDate);SQLPARAMS_PUSH(p,"s",std::string,executionDate);}
	std::string filter=std::string(BASE_FROM); appendEncounterFilter(query,filter,p); appendUserScope(userId,filter,p);
	if (query->startDate && !query->startDate->empty()) { filter += " AND e.start_time>=?"; SQLPARAMS_PUSH(p,"dt",std::string,query->startDate.getValue("")); }
	if (query->endDate && !query->endDate->empty()) { filter += " AND e.start_time<=?"; SQLPARAMS_PUSH(p,"dt",std::string,query->endDate.getValue("")); }
	std::string sql="SELECT COALESCE(SUM((SELECT COUNT(*) FROM med_medication_request mr WHERE mr.encounter_id=e.id AND mr.is_deleted=0 AND mr.status_enum='ACTIVE' AND COALESCE(mr.generate_source_enum,'')<>'NURSE_PRICING'"+medDate+")+(SELECT COUNT(*) FROM wkf_service_request sr WHERE sr.encounter_id=e.id AND sr.is_deleted=0 AND sr.status_enum='ACTIVE' AND COALESCE(sr.generate_source_enum,'')<>'NURSE_PRICING'"+serviceDate+")),0)"+filter;
	return sqlSession->executeQueryNumerical(sql,p);
}
uint64_t EncounterManageDAO::countPendingExecute(
	const PatientStatisticsQuery::Wrapper& query, const std::string& userId)
{
	SqlParams p;
	const std::string executionDate=query->nurseStationExecutionDate.getValue("");
	std::string medDate,serviceDate;
	if(!executionDate.empty()){medDate=" AND DATE(COALESCE(mr.effective_dose_start,mr.req_authored_time))=?";serviceDate=" AND DATE(COALESCE(sr.occurrence_start_time,sr.authored_time))=?";SQLPARAMS_PUSH(p,"s",std::string,executionDate);SQLPARAMS_PUSH(p,"s",std::string,executionDate);}
	std::string filter=std::string(BASE_FROM); appendEncounterFilter(query,filter,p); appendUserScope(userId,filter,p);
	if (query->startDate && !query->startDate->empty()) { filter += " AND e.start_time>=?"; SQLPARAMS_PUSH(p,"dt",std::string,query->startDate.getValue("")); }
	if (query->endDate && !query->endDate->empty()) { filter += " AND e.start_time<=?"; SQLPARAMS_PUSH(p,"dt",std::string,query->endDate.getValue("")); }
	std::string sql="SELECT COALESCE(SUM((SELECT COUNT(*) FROM med_medication_request mr WHERE mr.encounter_id=e.id AND mr.is_deleted=0 AND mr.status_enum='VERIFIED'"+medDate+")+(SELECT COUNT(*) FROM wkf_service_request sr WHERE sr.encounter_id=e.id AND sr.is_deleted=0 AND sr.status_enum='VERIFIED'"+serviceDate+")),0)"+filter;
	return sqlSession->executeQueryNumerical(sql,p);
}

std::list<WardNodeDTO::Wrapper> EncounterManageDAO::selectWards(const std::string& userId)
{
	return sqlSession->executeQuery<WardNodeDTO::Wrapper>(
		R"SQL(
			SELECT ward.id ward_id,ward.name ward_name,COALESCE(ward.display_order,0) display_order
			FROM adm_location ward
			WHERE ward.is_deleted=0 AND ward.form_enum='WARD'
				AND EXISTS (
					SELECT 1 FROM adm_practitioner p
					JOIN adm_pr_assign a ON a.practitioner_id=p.id
						AND a.is_deleted=0 AND a.active_flag=1
					LEFT JOIN adm_location scope_location ON scope_location.id=a.location_id
						AND scope_location.is_deleted=0
					LEFT JOIN adm_location scope_parent ON scope_parent.id=scope_location.parent_id
						AND scope_parent.is_deleted=0
					WHERE p.user_id=? AND p.is_deleted=0 AND p.active_flag=1
						AND ward.id IN (scope_location.id,scope_location.parent_id,scope_parent.parent_id)
				)
			ORDER BY COALESCE(ward.display_order,0),ward.name
		)SQL",
		WardNodeMapper(), "%s", userId);
}

std::list<HouseNodeDTO::Wrapper> EncounterManageDAO::selectHouses(
	const std::string& wardId, const std::string& userId)
{
	return sqlSession->executeQuery<HouseNodeDTO::Wrapper>(
		R"SQL(
			SELECT house.id house_id,house.name house_name,COALESCE(house.display_order,0) display_order
			FROM adm_location house
			WHERE house.is_deleted=0 AND house.form_enum='HOUSE' AND house.parent_id=?
				AND EXISTS (
					SELECT 1 FROM adm_practitioner p
					JOIN adm_pr_assign a ON a.practitioner_id=p.id
						AND a.is_deleted=0 AND a.active_flag=1
					LEFT JOIN adm_location scope_location ON scope_location.id=a.location_id
						AND scope_location.is_deleted=0
					WHERE p.user_id=? AND p.is_deleted=0 AND p.active_flag=1
						AND (a.location_id=house.id OR a.location_id=house.parent_id
							OR scope_location.parent_id=house.id)
				)
			ORDER BY COALESCE(house.display_order,0),house.name
		)SQL",
		HouseNodeMapper(), "%s%s", wardId, userId);
}
