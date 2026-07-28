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

// Explicit column list matching VitalSignDO field order
#define NURSING_COLUMNS \
    "id, tenant_id, created_at, updated_at, created_by, " \
    "updated_by, is_deleted, deleted_at, deleted_by, version, " \
    "patient_id, encounter_id, recorded_at, type_enum, " \
    "bus_no, location_id, baby_no, recorder_id, remark"

#define DETAIL_COLUMNS \
    "id, tenant_id, created_at, updated_at, created_by, " \
    "updated_by, is_deleted, deleted_at, deleted_by, version, " \
    "vital_sign_id, term_code, item_code, item_value, units, remark"

// Minimal mapper for paper-header query results; returns raw data struct
namespace {
    class PaperHeaderMapper : public Mapper<std::shared_ptr<PaperHeaderData>>
    {
    public:
        std::shared_ptr<PaperHeaderData> mapper(ResultSet* rs) const override
        {
            auto d = std::make_shared<PaperHeaderData>();
            d->inpatientNo = rs->getString("inpatient_no").c_str();
            d->patientName = rs->getString("patient_name").c_str();
            d->gender = rs->getString("gender").c_str();
            d->age = rs->getInt("age");
            d->admitAt = rs->getString("admit_at").c_str();
            d->dischargeAt = rs->getString("discharge_at").c_str();
            d->wardName = rs->getString("ward_name").c_str();
            d->bedNo = rs->getString("bed_no").c_str();
            return d;
        }
    };
}

std::string NursingDAO::queryConditionBuilder(const NursingRecordQuery::Wrapper& query, SqlParams& params)
{
    std::stringstream sqlCondition;
    // Base filter: exclude soft-deleted, restrict to NURSING_RECORD type
    sqlCondition << " WHERE is_deleted = 0 AND type_enum = 'NURSING_RECORD'";

    // Filter by encounter_id
    if (query->encounterId)
    {
        sqlCondition << " AND encounter_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
    }

    // Filter by patient_id
    if (query->patientId)
    {
        sqlCondition << " AND patient_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
    }

    // Filter by recorded_at range start
    if (query->recordedAtFrom)
    {
        sqlCondition << " AND recorded_at >= ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->recordedAtFrom.getValue(""));
    }

    // Filter by recorded_at range end
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

    // Sort order: ascending or descending by recorded_at
    bool asc = query->sortAsc ? query->sortAsc.getValue(false) : false;
    sql += asc ? " ORDER BY recorded_at ASC, id ASC"
               : " ORDER BY recorded_at DESC, id DESC";

    return sqlSession->executeQuery<VitalSignDO>(sql, NursingMapper(), params);
}

std::shared_ptr<PaperHeaderData> NursingDAO::selectPaperHeader(const std::string& encounterId, const std::string& patientId)
{
    // At the call sites, encounterId/patientId are already validated non-empty strings.
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

int NursingDAO::deleteByEncounterAndPatient(const std::string& encounterId, const std::string& patientId)
{
    // First delete detail rows for matching vital sign records
    std::string sqlDetail =
        "DELETE d FROM dia_vital_sign_detail d "
        "INNER JOIN dia_vital_sign v ON d.vital_sign_id = v.id "
        "WHERE v.encounter_id = ? AND v.patient_id = ?";
    sqlSession->executeUpdate(sqlDetail, "%s%s", encounterId, patientId);

    // Then delete main vital sign records
    std::string sqlMain =
        "DELETE FROM dia_vital_sign "
        "WHERE encounter_id = ? AND patient_id = ?";
    return sqlSession->executeUpdate(sqlMain, "%s%s", encounterId, patientId);
}

int NursingDAO::insertDetails(const std::list<VitalSignDetailDO>& details)
{
    if (details.empty()) return 0;
    return insertBatch(details);
}

std::list<VitalSignDetailDO> NursingDAO::selectByVitalSignIds(const std::list<std::string>& ids)
{
    if (ids.empty()) return {};

    // Build IN clause with parameterized placeholders
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
