#include "stdafx.h"
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
#include "ProcedureDAO.h"

// ============================================================================
// Mapper: converts a ResultSet row to ClnProcedureDO
// ============================================================================
class ProcedureMapper : public Mapper<PtrClnProcedureDO>
{
public:
    PtrClnProcedureDO mapper(ResultSet* rs) const override
    {
        auto procedure = std::make_shared<ClnProcedureDO>();
        procedure->setId(rs->getString("id"));
        procedure->setTenantId(rs->getString("tenant_id"));
        procedure->setVersion(rs->getInt("version"));
        procedure->setRequestId(rs->getString("request_id"));
        procedure->setRequestTable(rs->getString("request_table"));
        procedure->setStatusEnum(rs->getString("status_enum"));
        procedure->setStatusReasonText(rs->getString("status_reason_text"));
        procedure->setCategoryEnum(rs->getString("category_enum"));
        procedure->setProcedureCode(rs->getString("procedure_code"));
        procedure->setEncounterId(rs->getString("encounter_id"));
        procedure->setPatientId(rs->getString("patient_id"));
        procedure->setOccurrenceTime(rs->getString("occurrence_time"));
        procedure->setRecordedTime(rs->getString("recorded_time"));
        procedure->setRecorderId(rs->getString("recorder_id"));
        procedure->setOrgId(rs->getString("org_id"));
        procedure->setLocationId(rs->getString("location_id"));
        procedure->setGroupId(rs->getString("group_id"));
        procedure->setRefundId(rs->getString("refund_id"));
        procedure->setOccurrenceSlotActive(rs->getBoolean("occurrence_slot_active"));
        return procedure;
    }
};

// ============================================================================
// Query procedures by encounter ID with optional status filter + pagination
// ============================================================================
std::list<PtrClnProcedureDO> ProcedureDAO::queryByEncounter(
    const std::string& encounterId,
    const std::string& statusEnum,
    uint64_t pageIndex,
    uint64_t pageSize)
{
    SqlParams params;
    std::string sql =
        "SELECT id, tenant_id, version, request_id, request_table, "
        "status_enum, status_reason_text, category_enum, procedure_code, "
        "encounter_id, patient_id, occurrence_time, recorded_time, recorder_id, "
        "org_id, location_id, group_id, refund_id, occurrence_slot_active "
        "FROM cln_procedure "
        "WHERE encounter_id = ? AND is_deleted = 0 ";
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);

    if (!statusEnum.empty()) {
        sql += "AND status_enum = ? ";
        SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
    }

    sql += "ORDER BY occurrence_time DESC LIMIT ?, ?";
    SQLPARAMS_PUSH(params, "ull", uint64_t, (pageIndex - 1) * pageSize);
    SQLPARAMS_PUSH(params, "ull", uint64_t, pageSize);

    return sqlSession->executeQuery<PtrClnProcedureDO>(sql, ProcedureMapper(), params);
}

// ============================================================================
// Count procedures by encounter ID with optional status filter
// ============================================================================
uint64_t ProcedureDAO::countByEncounter(
    const std::string& encounterId,
    const std::string& statusEnum)
{
    SqlParams params;
    std::string sql =
        "SELECT COUNT(1) FROM cln_procedure "
        "WHERE encounter_id = ? AND is_deleted = 0 ";
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);

    if (!statusEnum.empty()) {
        sql += "AND status_enum = ? ";
        SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
    }

    return sqlSession->executeQueryNumerical(sql, params);
}

// ============================================================================
// Query a single procedure by primary key
// ============================================================================
PtrClnProcedureDO ProcedureDAO::queryById(const std::string& procedureId)
{
    SqlParams params;
    std::string sql =
        "SELECT id, tenant_id, version, request_id, request_table, "
        "status_enum, status_reason_text, category_enum, procedure_code, "
        "encounter_id, patient_id, occurrence_time, recorded_time, recorder_id, "
        "org_id, location_id, group_id, refund_id, occurrence_slot_active "
        "FROM cln_procedure "
        "WHERE id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "s", std::string, procedureId);

    return sqlSession->executeQueryOne<PtrClnProcedureDO>(sql, ProcedureMapper(), params);
}

// ============================================================================
// Insert a new procedure record (status = COMPLETED)
// ============================================================================
int ProcedureDAO::insertCompleted(const ClnProcedureDO& procedure)
{
    return insert(procedure);
}

// ============================================================================
// Update procedure status and status reason
// ============================================================================
int ProcedureDAO::updateStatus(
    const std::string& procedureId,
    const std::string& statusEnum,
    const std::string& statusReasonText,
    const std::string& operatorId)
{
    std::string sql =
        "UPDATE cln_procedure SET "
        "status_enum = ?, "
        "status_reason_text = ?, "
        "updated_by = ?, "
        "updated_at = NOW() "
        "WHERE id = ? AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
    SQLPARAMS_PUSH(params, "s", std::string, statusReasonText);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, procedureId);

    return sqlSession->executeUpdate(sql, params);
}
