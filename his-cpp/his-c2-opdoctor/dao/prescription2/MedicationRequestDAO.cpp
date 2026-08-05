#include "stdafx.h"
#include "MedicationRequestDAO.h"
#include "MedicationRequestMapper.h"

namespace
{
// Fields required for medical advice state transitions.
const std::string MEDICATION_REQUEST_COLUMNS =
    "id, encounter_id, version, group_id, status_enum, execute_num, "
    "status_changed_time, performer_check_id, check_time, updated_by, updated_at ";
}

PtrMedicationRequestDO MedicationRequestDAO::selectById(
    const std::string& id, const std::string& encounterId)
{
    // Restrict the read operation to the specified encounter.
    const std::string sql =
        "SELECT " + MEDICATION_REQUEST_COLUMNS +
        "FROM med_medication_request "
        "WHERE id=? AND encounter_id=? AND is_deleted=0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    return sqlSession->executeQueryOne<PtrMedicationRequestDO>(
        sql, PtrMedicationRequestMapper(), params);
}

PtrMedicationRequestDO MedicationRequestDAO::selectByIdForUpdate(
    const std::string& id, const std::string& encounterId)
{
    // Lock the matching record for the surrounding transaction.
    const std::string sql =
        "SELECT " + MEDICATION_REQUEST_COLUMNS +
        "FROM med_medication_request "
        "WHERE id=? AND encounter_id=? AND is_deleted=0 FOR UPDATE";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    return sqlSession->executeQueryOne<PtrMedicationRequestDO>(
        sql, PtrMedicationRequestMapper(), params);
}

std::list<std::string> MedicationRequestDAO::selectIdsByGroupForUpdate(
    const std::string& encounterId, const std::string& groupId)
{
    // Lock all records in the group before validating its completeness.
    const std::string sql =
        "SELECT id FROM med_medication_request "
        "WHERE encounter_id=? AND group_id=? AND is_deleted=0 FOR UPDATE";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, groupId);
    return sqlSession->executeQuery<std::string>(
        sql, MedicationRequestIdMapper(), params);
}

int MedicationRequestDAO::updateStatus(const MedicationRequestDO& data,
    const std::string& expectedStatus, int expectedVersion,
    bool updateCheckFields)
{
    // Build a conditional update to protect against concurrent state changes.
    std::string sql =
        "UPDATE med_medication_request "
        "SET status_enum=?, status_changed_time=?, updated_by=?, updated_at=?";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, data.getStatusEnum());
    SQLPARAMS_PUSH(params, "s", std::string, data.getStatusChangedTime());
    SQLPARAMS_PUSH(params, "s", std::string, data.getUpdatedBy());
    SQLPARAMS_PUSH(params, "s", std::string, data.getUpdatedAt());

    if (updateCheckFields)
    {
        // Signing also records the signing user and time.
        sql += ", performer_check_id=?, check_time=?";
        SQLPARAMS_PUSH(params, "s", std::string, data.getPerformerCheckId());
        SQLPARAMS_PUSH(params, "s", std::string, data.getCheckTime());
    }

    sql += " , version=? "
        "WHERE id=? AND encounter_id=? AND version=? "
        "AND status_enum=? AND is_deleted=0";
    SQLPARAMS_PUSH(params, "i", int, data.getVersion());
    SQLPARAMS_PUSH(params, "s", std::string, data.getId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getEncounterId());
    SQLPARAMS_PUSH(params, "i", int, expectedVersion);
    SQLPARAMS_PUSH(params, "s", std::string, expectedStatus);
    return sqlSession->executeUpdate(sql, params);
}
