/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/26 00:00:00

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
#include "NursingDAO.h"
#include "NursingMapper.h"


#define NURSING_COLUMNS \
    "id, tenant_id, created_at, updated_at, created_by, " \
    "updated_by, is_deleted, deleted_at, deleted_by, version, " \
    "patient_id, encounter_id, recorded_at, type_enum, " \
    "bus_no, location_id, baby_no, recorder_id, remark"

#define DETAIL_COLUMNS \
    "id, tenant_id, created_at, updated_at, created_by, " \
    "updated_by, is_deleted, deleted_at, deleted_by, version, " \
	    "vital_sign_id, term_code, item_code, item_value, units, remark"

namespace
{
	class NursingStringMapper : public Mapper<std::string>
	{
	public:
		std::string mapper(ResultSet* resultSet) const override
		{
			return resultSet->getString(1);
		}
	};
}

std::string NursingDAO::queryConditionBuilder(const NursingRecordQuery::Wrapper& query, SqlParams& params)
{
    std::stringstream sqlCondition;

    sqlCondition << " WHERE is_deleted = 0 AND type_enum = 'NURSING_RECORD'";


    if (query->encounterId)
    {
        sqlCondition << " AND encounter_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
    }


    if (query->patientId)
    {
        sqlCondition << " AND patient_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
    }


    if (query->recordedAtFrom)
    {
        sqlCondition << " AND recorded_at >= ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->recordedAtFrom.getValue(""));
    }


    if (query->recordedAtTo)
    {
        sqlCondition << " AND recorded_at <= ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->recordedAtTo.getValue(""));
    }

    return sqlCondition.str();
}

uint64_t NursingDAO::count(const NursingRecordQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM dia_vital_sign ";
    sql += queryConditionBuilder(query, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

std::list<VitalSignDO> NursingDAO::selectByCondition(const NursingRecordQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT " NURSING_COLUMNS " FROM dia_vital_sign ";
    sql += queryConditionBuilder(query, params);

   
    bool asc = query->sortAsc ? query->sortAsc.getValue(false) : false;
    sql += asc ? " ORDER BY recorded_at ASC, id ASC"
               : " ORDER BY recorded_at DESC, id DESC";

    return sqlSession->executeQuery<VitalSignDO>(sql, NursingMapper(), params);
}

std::shared_ptr<PaperHeaderData> NursingDAO::selectPaperHeader(const std::string& encounterId, const std::string& patientId)
{
   
    if (encounterId.empty() || patientId.empty())
    {
        return std::make_shared<PaperHeaderData>();
    }

    SqlParams params;
    const std::string sql =
        "SELECT "
        "  p.bus_no AS inpatient_no, "
        "  p.name AS patient_name, "
        "  p.gender_enum AS gender, "
        "  TIMESTAMPDIFF(YEAR, p.birth_date, CURDATE()) AS age, "
        "  e.start_time AS admit_at, "
        "  e.end_time AS discharge_at, "
        "  ward.name AS ward_name, "
        "  bed.name AS bed_no "
        "FROM adm_encounter e "
        "JOIN adm_patient p ON p.id=e.patient_id AND p.is_deleted=0 "
        "LEFT JOIN adm_encounter_location bed_rel "
        "  ON bed_rel.encounter_id=e.id AND bed_rel.form_enum='BED' "
        "  AND bed_rel.status_enum='ACTIVE' AND bed_rel.is_deleted=0 "
        "LEFT JOIN adm_location bed ON bed.id=bed_rel.location_id AND bed.is_deleted=0 "
        "LEFT JOIN adm_location house ON house.id=bed.parent_id AND house.is_deleted=0 "
        "LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0 "
        "WHERE e.id=? AND e.patient_id=? AND e.is_deleted=0 LIMIT 1";

    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, patientId);

    auto results = sqlSession->executeQuery<std::shared_ptr<PaperHeaderData>>(sql, PaperHeaderMapper(), params);
    if (!results.empty())
    {
        return results.front();
    }
    return std::make_shared<PaperHeaderData>();
}

std::list<VitalSignDetailDO> NursingDAO::selectByVitalSignIds(const std::list<std::string>& ids)
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
    return sqlSession->executeQuery<VitalSignDetailDO>(sql, NursingDetailMapper(), params);
}

// ---------------------------------------------------------------------------
// countByBusNoPrefix — atomic query used by Service layer for bus_no sequence
// ---------------------------------------------------------------------------
uint64_t NursingDAO::countByBusNoPrefix(const std::string& prefix)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM dia_vital_sign "
                      "WHERE type_enum = 'NURSING_RECORD' AND bus_no LIKE ?";
    SQLPARAMS_PUSH(params, "s", std::string, prefix + "%");
    return sqlSession->executeQueryNumerical(sql, params);
}

// ---------------------------------------------------------------------------
// selectBySlot — find existing record at the same encounter+patient+date+time
// ---------------------------------------------------------------------------
std::list<VitalSignDO> NursingDAO::selectBySlot(const std::string& encounterId,
                                                 const std::string& patientId,
                                                 const std::string& recordDate,
                                                 const std::string& recordTime)
{
    SqlParams params;
    std::string sql = "SELECT " NURSING_COLUMNS " FROM dia_vital_sign "
                      "WHERE is_deleted = 0 AND type_enum = 'NURSING_RECORD'"
                      " AND encounter_id = ?"
                      " AND patient_id = ?"
                      " AND recorded_at = ?";
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, patientId);

    std::string recordedAt = recordDate;
    if (!recordTime.empty())
        recordedAt += " " + recordTime + ":00";
    else if (!recordDate.empty())
        recordedAt += " 00:00:00";
    SQLPARAMS_PUSH(params, "s", std::string, recordedAt);

    return sqlSession->executeQuery<VitalSignDO>(sql, NursingMapper(), params);
}

std::list<VitalSignDO> NursingDAO::selectRecord(
	const std::string& vitalSignId, const std::string& encounterId,
	const std::string& patientId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, vitalSignId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	return sqlSession->executeQuery<VitalSignDO>(
		"SELECT " NURSING_COLUMNS " FROM dia_vital_sign "
		"WHERE id=? AND encounter_id=? AND patient_id=? "
		"AND type_enum='NURSING_RECORD' AND is_deleted=0 LIMIT 1",
		NursingMapper(), params);
}

// ---------------------------------------------------------------------------
// softDeleteById — soft-delete a main record; BaseDAO delete marks is_deleted
// ---------------------------------------------------------------------------
int NursingDAO::softDeleteById(const std::string& id)
{
    std::string sql = "UPDATE dia_vital_sign SET is_deleted = 1 WHERE id = ?";
    return sqlSession->executeUpdate(sql, "%s", id);
}

int NursingDAO::updateMainRecord(const VitalSignDO& data, int64_t expectedVersion)
{
	SqlParams params;
	SQLPARAMS_PUSH(params,"s",std::string,data.getPatientId());
	SQLPARAMS_PUSH(params,"s",std::string,data.getEncounterId());
	SQLPARAMS_PUSH(params,"dt",std::string,data.getRecordedAt());
	SQLPARAMS_PUSH(params,"s",std::string,data.getTypeEnum());
	SQLPARAMS_PUSH(params,"s",std::string,data.getBusNo());
	SQLPARAMS_PUSH(params,"s",std::string,data.getCreatedBy());
	SQLPARAMS_PUSH(params,"s",std::string,data.getRecorderId());
	SQLPARAMS_PUSH(params,"s",std::string,data.getRemark());
	SQLPARAMS_PUSH(params,"s",std::string,data.getId());
	SQLPARAMS_PUSH(params,"ll",int64_t,expectedVersion);
	return sqlSession->executeUpdate(
        "UPDATE dia_vital_sign SET patient_id=?,encounter_id=?,recorded_at=?,type_enum=?,"
        "bus_no=?,updated_at=NOW(6),updated_by=?,is_deleted=0,deleted_at=NULL,deleted_by=NULL,"
		"recorder_id=?,remark=?,version=version+1 WHERE id=? AND version=? AND is_deleted=0",
		params);
}

// ---------------------------------------------------------------------------
// deleteByVitalSignId — FK-based delete not covered by BaseDAO (PK-only)
// ---------------------------------------------------------------------------
int NursingDAO::deleteByVitalSignId(const std::string& vitalSignId)
{
	std::string sql = "UPDATE dia_vital_sign_detail SET is_deleted=1,deleted_at=NOW(6),"
		"updated_at=NOW(6),version=version+1 WHERE vital_sign_id=? AND is_deleted=0";
	return sqlSession->executeUpdate(sql, "%s", vitalSignId);
}

int NursingDAO::softDeleteRecord(
	const std::string& vitalSignId,const std::string& encounterId,
	const std::string& patientId,int64_t expectedVersion,
	const std::string& operatorId,const std::string& reason)
{
	return sqlSession->executeUpdate(
		"UPDATE dia_vital_sign SET is_deleted=1,deleted_at=NOW(6),deleted_by=?,"
		"updated_at=NOW(6),updated_by=?,remark=CASE WHEN ?='' THEN remark ELSE ? END,"
		"version=version+1 WHERE id=? AND encounter_id=? AND patient_id=? "
		"AND type_enum='NURSING_RECORD' AND version=? AND is_deleted=0",
		"%s%s%s%s%s%s%s%lld",operatorId,operatorId,reason,reason,vitalSignId,
		encounterId,patientId,expectedVersion);
}

bool NursingDAO::validRecordedAt(const std::string& encounterId,
	const std::string& patientId, const std::string& recordedAt)
{
	return sqlSession->executeQueryNumerical(R"SQL(
		SELECT COUNT(*) FROM adm_encounter
		WHERE id=? AND patient_id=? AND class_enum='IMP' AND is_deleted=0
			AND start_time<=? AND ?<=NOW()
			AND (end_time IS NULL OR ?<=end_time)
	)SQL", "%s%s%dt%dt%dt", encounterId, patientId,
		recordedAt, recordedAt, recordedAt) == 1;
}

std::list<std::string> NursingDAO::selectIdempotencyResult(
	const std::string& operation, const std::string& idempotencyKey)
{
	return sqlSession->executeQuery<std::string>(R"SQL(
		SELECT changes FROM whale_audit_logs
		WHERE entity_type='NURSE_STATION_NURSING_RECORD'
			AND entity_id=? AND operation_type=?
		ORDER BY created_at DESC LIMIT 1
	)SQL", NursingStringMapper(), "%s%s", idempotencyKey, operation);
}

int NursingDAO::insertIdempotencyResult(const std::string& auditId,
	const std::string& operation, const std::string& idempotencyKey,
	const std::string& operatorId, const std::string& fingerprint,
	const std::string& resultData)
{
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO whale_audit_logs(
			id,entity_type,entity_id,operation_type,user_id,changes,created_at)
		VALUES(?,'NURSE_STATION_NURSING_RECORD',?,?,?,?,NOW())
	)SQL", "%s%s%s%s%s", auditId, idempotencyKey, operation, operatorId,
		fingerprint + "\n--RESULT--\n" + resultData);
}

std::string NursingDAO::selectOperatorDisplay(const std::string& userId)
{
	const auto value = sqlSession->executeQueryOne<std::string>(R"SQL(
		SELECT COALESCE(NULLIF(name,''),id) FROM adm_practitioner
		WHERE user_id=? AND is_deleted=0 AND active_flag=1
		ORDER BY id LIMIT 1
	)SQL", NursingStringMapper(), "%s", userId);
	return value.empty() ? userId : value;
}
