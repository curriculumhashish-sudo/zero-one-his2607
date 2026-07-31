#include "stdafx.h"
/*
 Copyright Zero One Star. All rights reserved.

 @Author: fanpai
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
#include "ProcedurePerformerDAO.h"

// ============================================================================
// Mapper: converts a ResultSet row to ProcedurePerformerDO
// ============================================================================
class ProcedurePerformerMapper : public Mapper<PtrProcedurePerformerDO>
{
public:
    PtrProcedurePerformerDO mapper(ResultSet* rs) const override
    {
        auto performer = std::make_shared<ProcedurePerformerDO>();
        performer->setId(rs->getString("id"));
        performer->setTenantId(rs->getString("tenant_id"));
        performer->setCreatedAt(rs->getString("created_at"));
        performer->setUpdatedAt(rs->getString("updated_at"));
        performer->setCreatedBy(rs->getString("created_by"));
        performer->setUpdatedBy(rs->getString("updated_by"));
        performer->setIsDeleted(rs->getInt("is_deleted"));
        performer->setDeletedAt(rs->getString("deleted_at"));
        performer->setDeletedBy(rs->getString("deleted_by"));
        performer->setVersion(rs->getInt("version"));
        performer->setProcedureId(rs->getString("procedure_id"));
        performer->setFunctionCode(rs->getString("function_code"));
        performer->setStartTime(rs->getString("start_time"));
        performer->setEndTime(rs->getString("end_time"));
        performer->setPractitionerId(rs->getString("practitioner_id"));
        performer->setOrgFromId(rs->getString("org_from_id"));
        return performer;
    }
};

// ============================================================================
// Query performers by procedure ID
// ============================================================================
std::list<PtrProcedurePerformerDO> ProcedurePerformerDAO::queryByProcedureId(
    const std::string& procedureId)
{
    SqlParams params;
    std::string sql =
        "SELECT id, tenant_id, created_at, updated_at, created_by, updated_by, "
        "is_deleted, deleted_at, deleted_by, version, "
        "procedure_id, function_code, start_time, end_time, "
        "practitioner_id, org_from_id "
        "FROM cln_procedure_performer "
        "WHERE procedure_id = ? AND is_deleted = 0 "
        "ORDER BY start_time ASC";
    SQLPARAMS_PUSH(params, "s", std::string, procedureId);

    return sqlSession->executeQuery<PtrProcedurePerformerDO>(
        sql, ProcedurePerformerMapper(), params);
}

// ============================================================================
// Insert a single procedure performer
// ============================================================================
int ProcedurePerformerDAO::insert(const ProcedurePerformerDO& performer)
{
    return BaseDAO::insert(performer);
}

// ============================================================================
// Insert multiple performers in batch
// ============================================================================
int ProcedurePerformerDAO::insertBatch(const std::list<ProcedurePerformerDO>& performers)
{
    if (performers.empty()) return 0;
    return BaseDAO::insertBatch(performers);
}

// ============================================================================
// Logical delete all performers for a procedure
// ============================================================================
int ProcedurePerformerDAO::removeByProcedureId(
    const std::string& procedureId,
    const std::string& operatorId)
{
    std::string sql =
        "UPDATE cln_procedure_performer SET "
        "is_deleted = 1, "
        "deleted_by = ?, "
        "deleted_at = NOW() "
        "WHERE procedure_id = ? AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, procedureId);

    return sqlSession->executeUpdate(sql, params);
}
