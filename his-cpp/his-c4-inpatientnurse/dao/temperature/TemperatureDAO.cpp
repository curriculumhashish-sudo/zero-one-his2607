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
#include "SimpleDateTimeFormat.h"

// Column list matching VitalSignDO field order (same table as nursing: dia_vital_sign)
#define TS_COLUMNS \
    "id, tenant_id, created_at, updated_at, created_by, " \
    "updated_by, is_deleted, deleted_at, deleted_by, version, " \
    "patient_id, encounter_id, recorded_at, type_enum, " \
    "bus_no, location_id, baby_no, recorder_id, remark"

#define DETAIL_COLUMNS \
    "id, tenant_id, created_at, updated_at, created_by, " \
    "updated_by, is_deleted, deleted_at, deleted_by, version, " \
    "vital_sign_id, term_code, item_code, item_value, units, remark"

// ---------------------------------------------------------------------------
// queryConditionBuilder — date-range filter for temperature sheet grid
// ---------------------------------------------------------------------------
std::string TemperatureDAO::queryConditionBuilder(const std::string& encounterId,
                                                   const std::string& patientId,
                                                   const std::string& dateFrom,
                                                   const std::string& dateTo,
                                                   SqlParams& params)
{
    std::stringstream sqlCondition;
    // Base filter: exclude soft-deleted, restrict to TEMPERATURE_SHEET type
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

// ---------------------------------------------------------------------------
// slotConditionBuilder — exact slot match (date + 2-digit hour)
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// generateBusNo — TS + yyyyMMdd + 3-digit sequence
// ---------------------------------------------------------------------------
std::string TemperatureDAO::generateBusNo()
{
    std::string today = SimpleDateTimeFormat::format();
    std::string datePart = today.substr(0, 10);
    // Remove dashes: "2026-07-28" → "20260728"
    datePart.erase(std::remove(datePart.begin(), datePart.end(), '-'), datePart.end());

    // Count today's records to derive the sequence number
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM dia_vital_sign "
                      "WHERE type_enum = 'TEMPERATURE_SHEET' AND bus_no LIKE ?";
    SQLPARAMS_PUSH(params, "s", std::string, "TS" + datePart + "%");
    uint64_t cnt = sqlSession->executeQueryNumerical(sql, params);

    std::stringstream busNo;
    busNo << "TS" << datePart << std::setw(3) << std::setfill('0') << (cnt + 1);
    return busNo.str();
}

int TemperatureDAO::insertDetails(const std::list<VitalSignDetailDO>& details)
{
    if (details.empty()) return 0;
    return insertBatch(details);
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

int TemperatureDAO::deleteByVitalSignId(const std::string& vitalSignId)
{
    std::string sql = "DELETE FROM dia_vital_sign_detail WHERE vital_sign_id = ?";
    return sqlSession->executeUpdate(sql, "%s", vitalSignId);
}
