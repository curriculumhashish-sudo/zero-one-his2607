/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/28 00:00:00

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
#include "TemperatureDAO.h"
#include "TemperatureMapper.h"


#define TS_COLUMNS \
    "id, tenant_id, created_at, updated_at, created_by, " \
    "updated_by, is_deleted, deleted_at, deleted_by, version, " \
    "patient_id, encounter_id, recorded_at, type_enum, " \
    "bus_no, location_id, baby_no, recorder_id, remark"

#define DETAIL_COLUMNS \
    "id, tenant_id, created_at, updated_at, created_by, " \
    "updated_by, is_deleted, deleted_at, deleted_by, version, " \
    "vital_sign_id, term_code, item_code, item_value, units, remark"


std::string TemperatureDAO::queryConditionBuilder(const std::string& encounterId,
                                                   const std::string& patientId,
                                                   const std::string& dateFrom,
                                                   const std::string& dateTo,
                                                   SqlParams& params)
{
    std::stringstream sqlCondition;

    sqlCondition << " WHERE is_deleted = 0 AND type_enum = 'TEMPERATURE_SHEET'";

    if (!encounterId.empty())
    {
        sqlCondition << " AND encounter_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    }
    if (!patientId.empty())
    {
        sqlCondition << " AND patient_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, patientId);
    }
    if (!dateFrom.empty())
    {
        sqlCondition << " AND recorded_at >= ?";
        SQLPARAMS_PUSH(params, "s", std::string, dateFrom);
    }
    if (!dateTo.empty())
    {
        sqlCondition << " AND recorded_at <= ?";
        SQLPARAMS_PUSH(params, "s", std::string, dateTo);
    }

    return sqlCondition.str();
}


std::string TemperatureDAO::slotConditionBuilder(const std::string& encounterId,
                                                  const std::string& patientId,
                                                  const std::string& recordDate,
                                                  const std::string& timeSlot,
                                                  SqlParams& params)
{
    std::stringstream sqlCondition;
    sqlCondition << " WHERE is_deleted = 0 AND type_enum = 'TEMPERATURE_SHEET'";

    if (!encounterId.empty())
    {
        sqlCondition << " AND encounter_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    }
    if (!patientId.empty())
    {
        sqlCondition << " AND patient_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, patientId);
    }
    if (!recordDate.empty())
    {
        // Match recorded_at starting with the date prefix (e.g. "2026-07-28%")
        sqlCondition << " AND recorded_at LIKE ?";
        SQLPARAMS_PUSH(params, "s", std::string, recordDate + "%");
    }
    if (!timeSlot.empty())
    {
        // Match the hour portion: recorded_at contains " HH:" where HH is the timeslot
        sqlCondition << " AND SUBSTRING(recorded_at, 12, 2) = ?";
        SQLPARAMS_PUSH(params, "s", std::string, timeSlot);
    }

    return sqlCondition.str();
}

// ---------------------------------------------------------------------------
uint64_t TemperatureDAO::count(const std::string& encounterId, const std::string& patientId,
                                const std::string& dateFrom, const std::string& dateTo)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM dia_vital_sign ";
    sql += queryConditionBuilder(encounterId, patientId, dateFrom, dateTo, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

// ---------------------------------------------------------------------------
std::list<VitalSignDO> TemperatureDAO::selectByCondition(const std::string& encounterId,
                                                          const std::string& patientId,
                                                          const std::string& dateFrom,
                                                          const std::string& dateTo)
{
    SqlParams params;
    std::string sql = "SELECT " TS_COLUMNS " FROM dia_vital_sign ";
    sql += queryConditionBuilder(encounterId, patientId, dateFrom, dateTo, params);
    // Temperature sheets are chronological — oldest first
    sql += " ORDER BY recorded_at ASC, id ASC";
    return sqlSession->executeQuery<VitalSignDO>(sql, TemperatureMapper(), params);
}

// ---------------------------------------------------------------------------
bool TemperatureDAO::existsBySlot(const std::string& encounterId, const std::string& patientId,
                                   const std::string& recordDate, const std::string& timeSlot)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM dia_vital_sign ";
    sql += slotConditionBuilder(encounterId, patientId, recordDate, timeSlot, params);
    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

// ---------------------------------------------------------------------------
std::list<VitalSignDO> TemperatureDAO::selectBySlot(const std::string& encounterId,
                                                     const std::string& patientId,
                                                     const std::string& recordDate,
                                                     const std::string& timeSlot)
{
    SqlParams params;
    std::string sql = "SELECT " TS_COLUMNS " FROM dia_vital_sign ";
    sql += slotConditionBuilder(encounterId, patientId, recordDate, timeSlot, params);
    sql += " ORDER BY recorded_at ASC";
    return sqlSession->executeQuery<VitalSignDO>(sql, TemperatureMapper(), params);
}


uint64_t TemperatureDAO::countByBusNoPrefix(const std::string& prefix)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM dia_vital_sign "
                      "WHERE type_enum = 'TEMPERATURE_SHEET' AND bus_no LIKE ?";
    SQLPARAMS_PUSH(params, "s", std::string, prefix + "%");
    return sqlSession->executeQueryNumerical(sql, params);
}

std::list<VitalSignDetailDO> TemperatureDAO::selectByVitalSignIds(const std::list<std::string>& ids)
{
    if (ids.empty()) return {};

    SqlParams params;
    std::stringstream inClause;
    bool first = true;
    for (auto& id : ids)
    {
        if (first) first = false;
        else inClause << ",";
        inClause << "?";
        SQLPARAMS_PUSH(params, "s", std::string, id);
    }

    std::string sql = "SELECT " DETAIL_COLUMNS " FROM dia_vital_sign_detail "
                      "WHERE is_deleted = 0 AND vital_sign_id IN (" + inClause.str() + ")";
    return sqlSession->executeQuery<VitalSignDetailDO>(sql, TemperatureDetailMapper(), params);
}

// ---------------------------------------------------------------------------
// deleteByVitalSignId — FK-based delete not covered by BaseDAO (PK-only).
// Kept in DAO as a justified extension per skill convention.
// ---------------------------------------------------------------------------
int TemperatureDAO::deleteByVitalSignId(const std::string& vitalSignId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, vitalSignId);
	return sqlSession->executeUpdate(
		"UPDATE dia_vital_sign_detail SET is_deleted=1,deleted_at=NOW(6),"
		"updated_at=NOW(6),version=version+1 WHERE vital_sign_id=? AND is_deleted=0",
		params);
}

int TemperatureDAO::updateMainRecord(const VitalSignDO& data, int64_t expectedVersion)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "dt", std::string, data.getRecordedAt());
	SQLPARAMS_PUSH(params, "s", std::string, data.getRecorderId());
	SQLPARAMS_PUSH(params, "s", std::string, data.getUpdatedBy());
	SQLPARAMS_PUSH(params, "s", std::string, data.getRemark());
	SQLPARAMS_PUSH(params, "s", std::string, data.getId());
	SQLPARAMS_PUSH(params, "s", std::string, data.getEncounterId());
	SQLPARAMS_PUSH(params, "s", std::string, data.getPatientId());
	SQLPARAMS_PUSH(params, "ll", int64_t, expectedVersion);
	return sqlSession->executeUpdate(
		"UPDATE dia_vital_sign SET recorded_at=?,recorder_id=?,updated_by=?,"
		"remark=?,updated_at=NOW(6),version=version+1 WHERE id=? AND encounter_id=? AND patient_id=? "
		"AND type_enum='TEMPERATURE_SHEET' AND version=? AND is_deleted=0",
		params);
}

std::shared_ptr<TemperaturePaperHeaderData> TemperatureDAO::selectPaperHeader(
	const std::string& encounterId, const std::string& patientId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	const std::string sql =
		"SELECT p.bus_no inpatient_no,p.name patient_name,p.gender_enum gender,"
		"CAST(TIMESTAMPDIFF(YEAR,p.birth_date,CURDATE()) AS CHAR) age,e.start_time admit_at,"
		"e.end_time discharge_at,ward.name ward_name,bed.name bed_no "
		"FROM adm_encounter e JOIN adm_patient p ON p.id=e.patient_id AND p.is_deleted=0 "
		"LEFT JOIN adm_encounter_location bed_rel ON bed_rel.encounter_id=e.id "
		"AND bed_rel.form_enum='BED' AND bed_rel.status_enum='ACTIVE' AND bed_rel.is_deleted=0 "
		"LEFT JOIN adm_location bed ON bed.id=bed_rel.location_id AND bed.is_deleted=0 "
		"LEFT JOIN adm_location house ON house.id=bed.parent_id AND house.is_deleted=0 "
		"LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0 "
		"WHERE e.id=? AND e.patient_id=? AND e.is_deleted=0 LIMIT 1";
	auto rows = sqlSession->executeQuery<std::shared_ptr<TemperaturePaperHeaderData>>(
		sql, TemperaturePaperHeaderMapper(), params);
	return rows.empty() ? std::make_shared<TemperaturePaperHeaderData>() : rows.front();
}

std::list<std::string> TemperatureDAO::selectSurgeryDates(const std::string& encounterId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	return sqlSession->executeQuery<std::string>(
		"SELECT DISTINCT DATE(occurrence_time) FROM cln_procedure "
		"WHERE encounter_id=? AND occurrence_time IS NOT NULL AND is_deleted=0 "
		"AND status_enum NOT IN ('CANCELLED','STOPPED') ORDER BY DATE(occurrence_time)",
		TemperatureStringMapper(), params);
}

std::list<std::string> TemperatureDAO::selectIdempotencyResult(
	const std::string& operation, const std::string& idempotencyKey)
{
	return sqlSession->executeQuery<std::string>(R"SQL(
		SELECT changes FROM whale_audit_logs
		WHERE entity_type='NURSE_STATION_TEMPERATURE_SHEET'
			AND entity_id=? AND operation_type=?
		ORDER BY created_at DESC LIMIT 1
	)SQL", TemperatureStringMapper(), "%s%s", idempotencyKey, operation);
}

int TemperatureDAO::insertIdempotencyResult(
	const std::string& auditId, const std::string& operation,
	const std::string& idempotencyKey, const std::string& operatorId,
	const std::string& fingerprint, const std::string& resultData)
{
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO whale_audit_logs(
			id,entity_type,entity_id,operation_type,user_id,changes,created_at)
		VALUES(?,'NURSE_STATION_TEMPERATURE_SHEET',?,?,?,?,NOW())
	)SQL", "%s%s%s%s%s", auditId, idempotencyKey, operation, operatorId,
		fingerprint + "\n--RESULT--\n" + resultData);
}
