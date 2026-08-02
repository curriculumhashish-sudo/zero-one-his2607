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
    std::string sql =
        "SELECT "
        "  p.bus_no AS inpatient_no, "
        "  p.name AS patient_name, "
        "  p.gender_enum AS gender, "
        "  TIMESTAMPDIFF(YEAR, p.birth_date, CURDATE()) AS age, "
        "  e.start_time AS admit_at, "
        "  e.end_time AS discharge_at, "
        "  loc_room.name AS ward_name, "
        "  loc_bed.name AS bed_no "
        "FROM adm_patient p "
        "LEFT JOIN adm_encounter e ON e.id = ? "
        "LEFT JOIN adm_encounter_location el_room "
        "  ON e.id = el_room.encounter_id "
        "  AND el_room.form_enum = 'ROOM' AND el_room.status_enum = 'ACTIVE' "
        "LEFT JOIN adm_location loc_room ON el_room.location_id = loc_room.id "
        "LEFT JOIN adm_encounter_location el_bed "
        "  ON e.id = el_bed.encounter_id "
        "  AND el_bed.form_enum = 'BED' AND el_bed.status_enum = 'ACTIVE' "
        "LEFT JOIN adm_location loc_bed ON el_bed.location_id = loc_bed.id "
        "WHERE p.id = ?";

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

// ---------------------------------------------------------------------------
// softDeleteById — soft-delete a main record; BaseDAO delete marks is_deleted
// ---------------------------------------------------------------------------
int NursingDAO::softDeleteById(const std::string& id)
{
    std::string sql = "UPDATE dia_vital_sign SET is_deleted = 1 WHERE id = ?";
    return sqlSession->executeUpdate(sql, "%s", id);
}

// ---------------------------------------------------------------------------
// deleteByVitalSignId — FK-based delete not covered by BaseDAO (PK-only)
// ---------------------------------------------------------------------------
int NursingDAO::deleteByVitalSignId(const std::string& vitalSignId)
{
    std::string sql = "DELETE FROM dia_vital_sign_detail WHERE vital_sign_id = ?";
    return sqlSession->executeUpdate(sql, "%s", vitalSignId);
}
