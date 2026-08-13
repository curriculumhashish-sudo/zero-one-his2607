/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
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
	sqlCondition << " WHERE d.is_deleted=0 AND d.summary_no IS NOT NULL AND d.summary_no<>''";
	if (query->statusEnum && !query->statusEnum->empty()) {
		const auto status = query->statusEnum.getValue("");
		if (status == "PENDING_SUBMIT") sqlCondition << " AND sr.status_enum='PENDING_APPROVAL'";
		else if (status == "SUBMITTED") sqlCondition << " AND sr.status_enum='APPROVAL' AND d.status_enum<>'COMPLETED'";
		else if (status == "DISPENSED") sqlCondition << " AND d.status_enum='COMPLETED'";
		else if (status == "CANCELLED") sqlCondition << " AND sr.is_deleted=1";
	}
	if (query->dispenseDate && !query->dispenseDate->empty()) {
		sqlCondition << " AND DATE(d.planned_dispense_time)=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->dispenseDate.getValue(""));
	}
	if (query->destinationLocationId && !query->destinationLocationId->empty()) {
		sqlCondition << " AND d.des_location_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->destinationLocationId.getValue(""));
	}
	if (query->startDate && !query->startDate->empty()) {
		sqlCondition << " AND DATE(COALESCE(sr.apply_time,sr.created_at))>=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->startDate.getValue(""));
	}
	if (query->endDate && !query->endDate->empty()) {
		sqlCondition << " AND DATE(COALESCE(sr.apply_time,sr.created_at))<=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->endDate.getValue(""));
	}
	if (query->keyword && !query->keyword->empty()) {
		sqlCondition << " AND (d.summary_no LIKE CONCAT('%',?,'%') OR applicant.name LIKE CONCAT('%',?,'%'))";
		SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
		SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t DispenseDAO::countSummary(
	const DispenseSummaryQuery::Wrapper& query, const std::string& userId)
{
	SqlParams params;
	string sql = "SELECT COUNT(DISTINCT d.summary_no) FROM med_medication_dispense d "
		"LEFT JOIN wkf_supply_request sr ON sr.bus_no=d.summary_no AND sr.type_enum='27' "
		"LEFT JOIN adm_practitioner applicant ON applicant.id=sr.applicant_id AND applicant.is_deleted=0 "
		"JOIN adm_encounter e ON e.id=d.encounter_id AND e.is_deleted=0 "
		"LEFT JOIN adm_encounter_location el ON el.encounter_id=e.id AND el.form_enum='BED' "
		"AND el.status_enum='ACTIVE' AND el.is_deleted=0 "
		"LEFT JOIN adm_location bed ON bed.id=el.location_id AND bed.is_deleted=0 "
		"LEFT JOIN adm_location house ON house.id=bed.parent_id AND house.is_deleted=0 "
		"LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0 "
		"JOIN adm_pr_assign assignment ON (assignment.location_id IN(bed.id,house.id,ward.id) "
		"OR assignment.org_id=e.organization_id) AND assignment.is_deleted=0 AND assignment.active_flag=1 "
		"JOIN adm_practitioner logged_practitioner ON logged_practitioner.id=assignment.practitioner_id "
		"AND logged_practitioner.user_id=? AND logged_practitioner.is_deleted=0 "
		"AND logged_practitioner.active_flag=1 ";
	SQLPARAMS_PUSH(params, "s", std::string, userId);
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<DispenseSummaryDO> DispenseDAO::selectSummaryWithPage(
	const DispenseSummaryQuery::Wrapper& query, const std::string& userId)
{
	SqlParams params;
	string sql =
		"SELECT d.summary_no,CASE WHEN MAX(d.status_enum)='COMPLETED' THEN 'DISPENSED' "
		"WHEN MAX(sr.is_deleted)=1 THEN 'CANCELLED' WHEN MAX(sr.status_enum)='APPROVAL' THEN 'SUBMITTED' "
		"ELSE 'PENDING_SUBMIT' END status_enum,CASE WHEN MAX(d.status_enum)='COMPLETED' THEN 'DISPENSED' "
		"WHEN MAX(sr.is_deleted)=1 THEN 'CANCELLED' WHEN MAX(sr.status_enum)='APPROVAL' THEN 'SUBMITTED' "
		"ELSE 'PENDING_SUBMIT' END status_text,MAX(d.des_location_id),MAX(destination.name),"
		"MAX(sr.applicant_id),MAX(applicant.name),IFNULL(DATE_FORMAT(MAX(COALESCE(sr.apply_time,sr.created_at)),'%Y-%m-%d %H:%i:%s'),''),"
		"MAX(d.rec_practitioner_id),COUNT(DISTINCT d.id),"
		"IFNULL(DATE_FORMAT(MAX(d.planned_dispense_time),'%Y-%m-%d %H:%i:%s'),''),"
		"IFNULL(DATE_FORMAT(MAX(d.dispense_time),'%Y-%m-%d %H:%i:%s'),''),MAX(sr.version) "
		"FROM med_medication_dispense d "
		"LEFT JOIN wkf_supply_request sr ON sr.bus_no=d.summary_no AND sr.type_enum='27' "
		"LEFT JOIN adm_location destination ON destination.id=d.des_location_id AND destination.is_deleted=0 "
		"LEFT JOIN adm_practitioner applicant ON applicant.id=sr.applicant_id AND applicant.is_deleted=0 "
		"JOIN adm_encounter e ON e.id=d.encounter_id AND e.is_deleted=0 "
		"LEFT JOIN adm_encounter_location el ON el.encounter_id=e.id AND el.form_enum='BED' "
		"AND el.status_enum='ACTIVE' AND el.is_deleted=0 "
		"LEFT JOIN adm_location bed ON bed.id=el.location_id AND bed.is_deleted=0 "
		"LEFT JOIN adm_location house ON house.id=bed.parent_id AND house.is_deleted=0 "
		"LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0 "
		"JOIN adm_pr_assign assignment ON (assignment.location_id IN(bed.id,house.id,ward.id) "
		"OR assignment.org_id=e.organization_id) AND assignment.is_deleted=0 AND assignment.active_flag=1 "
		"JOIN adm_practitioner logged_practitioner ON logged_practitioner.id=assignment.practitioner_id "
		"AND logged_practitioner.user_id=? AND logged_practitioner.is_deleted=0 "
		"AND logged_practitioner.active_flag=1 ";
	SQLPARAMS_PUSH(params, "s", std::string, userId);
	sql += queryConditionBuilder(query, params);
	sql += " GROUP BY d.summary_no ORDER BY d.summary_no DESC LIMIT ?,?";
	SQLPARAMS_PUSH(params, "ull", uint64_t,
		(query->pageIndex.getValue(1)-1)*query->pageSize.getValue(10));
	SQLPARAMS_PUSH(params, "ull", uint64_t, query->pageSize.getValue(10));
	return sqlSession->executeQuery<DispenseSummaryDO>(sql, DispenseSummaryMapper(), params);
}

std::list<DispenseDetailDO> DispenseDAO::selectDetailBySummaryNo(const std::string& summaryNo)
{
	string sql =
		"SELECT MIN(d.id),d.medication_id,IFNULL(m.name,''),SUM(d.quantity),d.unit_code,"
		"COALESCE(m.total_volume,''),CONCAT(SUM(d.quantity),' ',d.unit_code),"
		"COALESCE(MAX(ci.unit_price),0),COALESCE(SUM(ci.total_price),0) "
		"FROM med_medication_dispense d "
		"LEFT JOIN med_medication_definition m ON d.medication_id = m.id AND m.is_deleted=0 "
		"LEFT JOIN adm_charge_item ci ON ci.dispense_id=d.id AND ci.is_deleted=0 "
		"WHERE d.is_deleted=0 AND d.summary_no=? "
		"GROUP BY d.medication_id,m.name,m.total_volume,d.unit_code ORDER BY MIN(d.id) ASC";
	return sqlSession->executeQuery<DispenseDetailDO>(sql, DispenseDetailMapper(), "%s", summaryNo);
}
