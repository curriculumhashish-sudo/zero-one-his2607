#include "stdafx.h"
#include "ServiceRequestDAO.h"
#include "ServiceRequestMapper.h"

namespace
{
// Fields required for service request printing.
const std::string SERVICE_REQUEST_COLUMNS =
    "id, encounter_id, version, status_enum, category_enum, print_count, updated_by, updated_at ";
}

PtrServiceRequestDO ServiceRequestDAO::selectById(
    const std::string& id)
{
    // Query one active service request.
    const std::string sql =
        "SELECT " + SERVICE_REQUEST_COLUMNS +
        "FROM wkf_service_request "
        "WHERE id=? AND is_deleted=0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    return sqlSession->executeQueryOne<PtrServiceRequestDO>(
        sql, PtrServiceRequestMapper(), params);
}

PtrServiceRequestDO ServiceRequestDAO::selectByIdForUpdate(
    const std::string& id)
{
    // Lock the matching request for the surrounding transaction.
    const std::string sql =
        "SELECT " + SERVICE_REQUEST_COLUMNS +
        "FROM wkf_service_request "
        "WHERE id=? AND is_deleted=0 FOR UPDATE";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    return sqlSession->executeQueryOne<PtrServiceRequestDO>(
        sql, PtrServiceRequestMapper(), params);
}

int ServiceRequestDAO::incrementPrintCount(
    const ServiceRequestDO& data, int expectedVersion)
{
    // Update only the print count and audit fields.
    const std::string sql =
        "UPDATE wkf_service_request "
        "SET print_count=?, updated_by=?, updated_at=?, version=? "
        "WHERE id=? AND version=? AND is_deleted=0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "i", int, data.getPrintCount());
    SQLPARAMS_PUSH(params, "s", std::string, data.getUpdatedBy());
    SQLPARAMS_PUSH(params, "s", std::string, data.getUpdatedAt());
    SQLPARAMS_PUSH(params, "i", int, data.getVersion());
    SQLPARAMS_PUSH(params, "s", std::string, data.getId());
    SQLPARAMS_PUSH(params, "i", int, expectedVersion);
    return sqlSession->executeUpdate(sql, params);
}
