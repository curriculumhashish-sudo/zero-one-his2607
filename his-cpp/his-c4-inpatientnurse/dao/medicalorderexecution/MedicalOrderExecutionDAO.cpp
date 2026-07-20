/*
 Copyright Zero One Star. All rights reserved.

 @Author: fanpai
 @Date: 2026/07/20 16:00:00

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
#include "MedicalOrderExecutionDAO.h"

// ============================================================================
// Mapper: converts a ResultSet row to WkfServiceRequestDO.
// Required by SqlSession::executeQuery -- it calls mapper.mapper(ResultSet*),
// so we must subclass Mapper<T> instead of using a plain lambda.
// ============================================================================
class WkfServiceRequestMapper : public Mapper<PtrWkfServiceRequestDO>
{
public:
    PtrWkfServiceRequestDO mapper(ResultSet* rs) const override
    {
        auto sr = std::make_shared<WkfServiceRequestDO>();
        sr->setId(rs->getString("id"));
        sr->setBusNo(rs->getString("bus_no"));
        sr->setPrescriptionNo(rs->getString("prescription_no"));
        sr->setStatusEnum(rs->getString("status_enum"));
        sr->setIntentEnum(rs->getString("intent_enum"));
        sr->setCategoryEnum(rs->getString("category_enum"));
        sr->setPriorityEnum(rs->getString("priority_enum"));
        sr->setPatientId(rs->getString("patient_id"));
        sr->setEncounterId(rs->getString("encounter_id"));
        sr->setAuthoredTime(rs->getString("authored_time"));
        sr->setRequesterId(rs->getString("requester_id"));
        sr->setPerformerId(rs->getString("performer_id"));
        sr->setPerformerCheckId(rs->getString("performer_check_id"));
        sr->setCheckTime(rs->getString("check_time"));
        sr->setLocationId(rs->getString("location_id"));
        sr->setQuantity(rs->getDouble("quantity"));
        sr->setUnitCode(rs->getString("unit_code"));
        sr->setExecutionCount(rs->getInt("execution_count"));
        sr->setRemark(rs->getString("remark"));
        sr->setEntrustContent(rs->getString("entrust_content"));
        sr->setReasonText(rs->getString("reason_text"));
        return sr;
    }
};

// ============================================================================
// Helper: dynamically build WHERE clause from query filters.
// Only adds conditions for fields that are actually populated.
// ============================================================================
static void buildWhereClause(const MedicalOrderQuery::Wrapper& query,
                              std::string& sql, SqlParams& params)
{
    if (query->statusEnum && !query->statusEnum.getValue("").empty()) {
        sql += " AND sr.status_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->statusEnum.getValue(""));
    }
    if (query->patientId && !query->patientId.getValue("").empty()) {
        sql += " AND sr.patient_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
    }
    if (query->encounterId && !query->encounterId.getValue("").empty()) {
        sql += " AND sr.encounter_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
    }
    if (query->startDate && !query->startDate.getValue("").empty()) {
        sql += " AND sr.authored_time >= ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->startDate.getValue(""));
    }
    if (query->endDate && !query->endDate.getValue("").empty()) {
        sql += " AND sr.authored_time <= ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->endDate.getValue(""));
    }
    if (query->locationId && !query->locationId.getValue("").empty()) {
        sql += " AND sr.location_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->locationId.getValue(""));
    }
    if (query->categoryEnum && !query->categoryEnum.getValue("").empty()) {
        sql += " AND sr.category_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->categoryEnum.getValue(""));
    }
    if (query->priorityEnum && !query->priorityEnum.getValue("").empty()) {
        sql += " AND sr.priority_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->priorityEnum.getValue(""));
    }
    if (query->intentEnum && !query->intentEnum.getValue("").empty()) {
        sql += " AND sr.intent_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->intentEnum.getValue(""));
    }
}

// ============================================================================
// Requirement 1: query verification items
// ============================================================================
std::list<PtrWkfServiceRequestDO> MedicalOrderExecutionDAO::queryVerificationItems(
    const MedicalOrderQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql =
        "SELECT sr.id, sr.bus_no, sr.prescription_no, sr.status_enum, "
        "sr.intent_enum, sr.category_enum, sr.priority_enum, "
        "sr.patient_id, sr.encounter_id, sr.authored_time, "
        "sr.requester_id, sr.performer_id, sr.performer_check_id, sr.check_time, "
        "sr.location_id, sr.quantity, sr.unit_code, "
        "sr.execution_count, sr.remark, sr.entrust_content, sr.reason_text "
        "FROM wkf_service_request sr "
        "WHERE sr.is_deleted = 0 ";

    buildWhereClause(query, sql, params);
    sql += " ORDER BY sr.authored_time DESC LIMIT ?, ?";

    SQLPARAMS_PUSH(params, "ull", uint64_t,
        (query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10));
    SQLPARAMS_PUSH(params, "ull", uint64_t, query->pageSize.getValue(10));

    return sqlSession->executeQuery<PtrWkfServiceRequestDO>(
        sql, WkfServiceRequestMapper(), params);
}

// ============================================================================
// Count verification items
// ============================================================================
uint64_t MedicalOrderExecutionDAO::countVerificationItems(
    const MedicalOrderQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql =
        "SELECT COUNT(1) FROM wkf_service_request sr "
        "WHERE sr.is_deleted = 0 ";

    buildWhereClause(query, sql, params);

    return sqlSession->executeQueryNumerical(sql, params);
}

// ============================================================================
// Requirement 4: query execution items
// ============================================================================
std::list<PtrWkfServiceRequestDO> MedicalOrderExecutionDAO::queryExecutionItems(
    const MedicalOrderQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql =
        "SELECT sr.id, sr.bus_no, sr.prescription_no, sr.status_enum, "
        "sr.intent_enum, sr.category_enum, sr.priority_enum, "
        "sr.patient_id, sr.encounter_id, sr.authored_time, "
        "sr.requester_id, sr.performer_id, sr.performer_check_id, sr.check_time, "
        "sr.location_id, sr.quantity, sr.unit_code, "
        "sr.execution_count, sr.remark, sr.entrust_content, sr.reason_text "
        "FROM wkf_service_request sr "
        "WHERE sr.is_deleted = 0 ";

    buildWhereClause(query, sql, params);
    sql += " ORDER BY sr.authored_time DESC LIMIT ?, ?";

    SQLPARAMS_PUSH(params, "ull", uint64_t,
        (query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10));
    SQLPARAMS_PUSH(params, "ull", uint64_t, query->pageSize.getValue(10));

    return sqlSession->executeQuery<PtrWkfServiceRequestDO>(
        sql, WkfServiceRequestMapper(), params);
}

// ============================================================================
// Count execution items
// ============================================================================
uint64_t MedicalOrderExecutionDAO::countExecutionItems(
    const MedicalOrderQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql =
        "SELECT COUNT(1) FROM wkf_service_request sr "
        "WHERE sr.is_deleted = 0 ";

    buildWhereClause(query, sql, params);

    return sqlSession->executeQueryNumerical(sql, params);
}

// ============================================================================
// Requirement 2: verify pass
// ============================================================================
int MedicalOrderExecutionDAO::verifyPass(
    const std::string& serviceRequestId, const std::string& verifierId)
{
    std::string sql =
        "UPDATE wkf_service_request SET "
        "status_enum = 'verified', "
        "performer_check_id = ?, "
        "check_time = NOW() "
        "WHERE id = ? AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, verifierId);
    SQLPARAMS_PUSH(params, "s", std::string, serviceRequestId);

    return sqlSession->executeUpdate(sql, params);
}

// ============================================================================
// Requirement 3: return order
// ============================================================================
int MedicalOrderExecutionDAO::returnOrder(
    const std::string& serviceRequestId, const std::string& returnReason)
{
    std::string sql =
        "UPDATE wkf_service_request SET "
        "status_enum = 'returned', "
        "reason_text = ? "
        "WHERE id = ? AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, returnReason);
    SQLPARAMS_PUSH(params, "s", std::string, serviceRequestId);

    return sqlSession->executeUpdate(sql, params);
}
