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
#include "BedDAO.h"
#include "BedMapper.h"

namespace
{
	class StringMapper : public Mapper<std::string>
	{
	public:
		std::string mapper(ResultSet* resultSet) const override
		{
			return resultSet->getString(1);
		}
	};
}

std::string BedDAO::queryConditionBuilder(const BedQuery::Wrapper& query,
	const std::string& userId, SqlParams& params)
{
    std::stringstream sql;
    // Only beds
    sql << " WHERE l.form_enum = 'BED' AND l.is_deleted = 0"
           " AND l.status_enum IN ('ENABLE','ACTIVE')"
           " AND l.operational_enum IN ('VACANT','AVAILABLE','NOT_IN_USE')";
    // Optional status filter
    if (query->status_enum) {
        sql << " AND l.status_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->status_enum.getValue(""));
    }
	// parent_id represents the room that directly owns the bed.
    if (query->parent_id) {
        sql << " AND l.parent_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->parent_id.getValue(""));
    }
    // Optional department filter
    if (query->organization_id) {
        sql << " AND l.organization_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->organization_id.getValue(""));
    }
	// Only return beds managed by the current nurse.
	sql << " AND EXISTS (SELECT 1 FROM adm_practitioner access_practitioner "
		"JOIN adm_pr_assign access_assignment ON "
		"access_assignment.practitioner_id=access_practitioner.id "
		"AND access_assignment.is_deleted=0 AND access_assignment.active_flag=1 "
		"LEFT JOIN adm_location access_house ON access_house.id=l.parent_id "
		"AND access_house.is_deleted=0 "
		"WHERE access_practitioner.user_id=? AND access_practitioner.is_deleted=0 "
		"AND access_practitioner.active_flag=1 AND "
		"(access_assignment.location_id IN (l.id,access_house.id,access_house.parent_id) "
		"OR access_assignment.org_id=l.organization_id))";
	SQLPARAMS_PUSH(params, "s", std::string, userId);
	// Free = no valid reservation on that date.
    sql << " AND NOT EXISTS (SELECT 1 FROM bkg_bed_reservation_ext r"
           " WHERE r.location_id = l.id AND r.is_deleted = 0";
    if (query->intended_admission_date) {
        sql << " AND r.intended_admission_date = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->intended_admission_date.getValue(""));
    }
    sql << ") AND NOT EXISTS (SELECT 1 FROM adm_encounter_location occupied"
		" WHERE occupied.location_id=l.id AND occupied.form_enum='BED'"
		" AND occupied.status_enum='ACTIVE' AND occupied.is_deleted=0)";
    return sql.str();
}

uint64_t BedDAO::countFree(
	const BedQuery::Wrapper& query, const std::string& userId)
{
    SqlParams params;
    // COUNT(DISTINCT l.id): one bed may be stored as duplicate rows in adm_location,
    // we must not count it twice.
    std::string sql = "SELECT COUNT(DISTINCT l.id) FROM adm_location l ";
    sql += queryConditionBuilder(query, userId, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

std::list<BedDTO::Wrapper> BedDAO::selectFreeWithPage(
	const BedQuery::Wrapper& query, const std::string& userId)
{
    SqlParams params;
	std::string sql =
		"SELECT l.id bed_id,l.name bed_name,l.parent_id,l.form_enum,l.status_enum,"
		"l.organization_id,l.operational_enum,l.version,COALESCE(l.display_order,0) display_order,"
		"house.id house_id,house.name house_name,ward.id ward_id,ward.name ward_name "
		"FROM adm_location l "
		"LEFT JOIN adm_location house ON house.id=l.parent_id AND house.is_deleted=0 "
		"LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0 ";
    sql += queryConditionBuilder(query, userId, params);
    // GROUP BY l.id: collapse duplicate bed rows (same id stored twice in adm_location)
    // so the free-bed list shows each bed only once.
    sql += " GROUP BY l.id";
	sql += " ORDER BY COALESCE(ward.display_order,0),COALESCE(house.display_order,0),"
		"COALESCE(l.display_order,0),l.name,l.id ";
    sql += " LIMIT " + std::to_string((query->pageIndex - 1) * query->pageSize) + "," + std::to_string(query->pageSize);
	return sqlSession->executeQuery<BedDTO::Wrapper>(sql, BedMapper(), params);
}

int BedDAO::reserve(const BedReservationDO& data)
{
    // Unique index (location_id, intended_admission_date) on bkg_bed_reservation_ext
    // guarantees no duplicate reservation on the same bed + date. A duplicate insert
    // throws, which the service layer catches and reports as "already reserved".
    return insert(data);
}

bool BedDAO::canReserve(
    const std::string& patientId,
    const std::string& locationId,
    const std::string& intendedAdmissionDate)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, locationId);
    SQLPARAMS_PUSH(params, "s", std::string, patientId);
    SQLPARAMS_PUSH(params, "s", std::string, intendedAdmissionDate);
    return sqlSession->executeQueryNumerical(
        "SELECT COUNT(*) FROM adm_patient p JOIN adm_location l ON l.id=? "
        "AND l.form_enum='BED' AND l.status_enum IN ('ENABLE','ACTIVE') "
        "AND l.operational_enum IN ('VACANT','AVAILABLE','NOT_IN_USE') "
        "AND l.is_deleted=0 "
        "WHERE p.id=? AND p.is_deleted=0 AND NOT EXISTS ("
        "SELECT 1 FROM bkg_bed_reservation_ext r WHERE r.location_id=l.id "
		"AND r.intended_admission_date=? AND r.is_deleted=0) AND NOT EXISTS ("
		"SELECT 1 FROM adm_encounter_location occupied WHERE occupied.location_id=l.id "
		"AND occupied.form_enum='BED' AND occupied.status_enum='ACTIVE' "
		"AND occupied.is_deleted=0)",
        params) == 1;
}

std::string BedDAO::resolveEncounterId(
	const std::string& appointmentId, const std::string& patientId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	SQLPARAMS_PUSH(params, "s", std::string, appointmentId);
	SQLPARAMS_PUSH(params, "s", std::string, appointmentId);
	SQLPARAMS_PUSH(params, "s", std::string, appointmentId);
	return sqlSession->executeQueryOne<std::string>(R"SQL(
		SELECT e.id
		FROM adm_encounter e
		LEFT JOIN wkf_appointment_queue appointment
			ON appointment.encounter_id=e.id AND appointment.is_deleted=0
		WHERE e.patient_id=? AND e.class_enum='IMP' AND e.is_deleted=0
			AND e.status_enum='REGISTERED'
			AND (e.id=? OR appointment.id=?)
		ORDER BY CASE WHEN e.id=? THEN 0 ELSE 1 END,e.created_at DESC
		LIMIT 1
	)SQL", StringMapper(), params);
}

std::string BedDAO::selectEncounterStatusForUpdate(const std::string& encounterId)
{
	return sqlSession->executeQueryOne<std::string>(
		"SELECT status_enum FROM adm_encounter WHERE id=? AND class_enum='IMP' "
		"AND is_deleted=0 FOR UPDATE", StringMapper(), "%s", encounterId);
}

std::string BedDAO::selectActiveBedForUpdate(const std::string& encounterId)
{
	return sqlSession->executeQueryOne<std::string>(
		"SELECT location_id FROM adm_encounter_location WHERE encounter_id=? "
		"AND form_enum='BED' AND status_enum='ACTIVE' AND is_deleted=0 "
		"LIMIT 1 FOR UPDATE", StringMapper(), "%s", encounterId);
}

std::string BedDAO::selectBedOperationalStatusForUpdate(const std::string& bedId)
{
	return sqlSession->executeQueryOne<std::string>(
		"SELECT operational_enum FROM adm_location WHERE id=? AND form_enum='BED' "
		"AND status_enum IN ('ENABLE','ACTIVE') AND is_deleted=0 FOR UPDATE",
		StringMapper(), "%s", bedId);
}

int BedDAO::occupyBed(const std::string& bedId, const std::string& operatorId)
{
	return sqlSession->executeUpdate(
		"UPDATE adm_location SET operational_enum=CASE "
		"WHEN operational_enum='NOT_IN_USE' THEN 'IN_USE' ELSE 'OCCUPIED' END,"
		"updated_at=NOW(6),"
		"updated_by=?,version=version+1 WHERE id=? AND form_enum='BED' "
		"AND status_enum IN ('ENABLE','ACTIVE') "
		"AND operational_enum IN ('VACANT','AVAILABLE','NOT_IN_USE') AND is_deleted=0",
		"%s%s", operatorId, bedId);
}

int BedDAO::createBedTrack(const std::string& encounterId,
	const std::string& bedId, const std::string& operatorId)
{
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO adm_encounter_location(
			id,encounter_id,location_id,form_enum,status_enum,start_time,
			created_at,created_by,updated_at,updated_by,version,is_deleted)
		VALUES(REPLACE(UUID(),'-',''),?,?,'BED','ACTIVE',NOW(6),
			NOW(6),?,NOW(6),?,0,0)
	)SQL", "%s%s%s%s", encounterId, bedId, operatorId, operatorId);
}

int BedDAO::markEncounterAdmitted(
	const std::string& encounterId, const std::string& operatorId)
{
	return sqlSession->executeUpdate(
		"UPDATE adm_encounter SET status_enum='IN_PROGRESS',start_time=COALESCE(start_time,NOW(6)),"
		"updated_at=NOW(6),updated_by=?,version=version+1 WHERE id=? "
		"AND class_enum='IMP' AND status_enum='REGISTERED' AND is_deleted=0",
		"%s%s", operatorId, encounterId);
}

int BedDAO::consumeReservation(const std::string& appointmentId,
	const std::string& patientId, const std::string& bedId,
	const std::string& operatorId)
{
	return sqlSession->executeUpdate(
		"UPDATE bkg_bed_reservation_ext SET is_deleted=1,deleted_at=NOW(6),"
		"deleted_by=?,updated_at=NOW(6),updated_by=?,version=version+1 "
		"WHERE appointment_id=? AND patient_id=? AND location_id=? AND is_deleted=0",
		"%s%s%s%s%s", operatorId, operatorId, appointmentId, patientId, bedId);
}
