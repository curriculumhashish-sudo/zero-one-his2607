#include "stdafx.h"
#include "WkfServiceRequestDAO.h"
#include "WkfServiceRequestMapper.h"

//批量停止服务 (IF-IP-DOR-20)
int WkfServiceRequestDAO::stopOrderBatch(
    const std::list<std::string>& orderIds,
    int stopReason,
    const std::string& operatorId, const std::string& encounterId, const std::string& patientId)
{
    if (orderIds.empty()) return 0;

    SqlParams params;
    std::ostringstream sql;

    sql << "UPDATE wkf_service_request "
        << "SET status_enum = 'STOPPED', "
        << "    status_reason = ?, "
        << "    status_changed_time = NOW(), "
        << "    updated_at = NOW() "
        << "WHERE id IN (";

    SQLPARAMS_PUSH(params, "i", int, stopReason);

    bool first = true;
    for (auto& id : orderIds) {
        if (!first) sql << ",";
        else first = false;
        sql << "?";
        SQLPARAMS_PUSH(params, "s", std::string, id);
    }

    sql << ") AND status_enum != 'STOPPED' AND is_deleted = 0";
    sql << " AND encounter_id = ? ";
    sql << " AND patient_id = ? ";
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, patientId);

    return sqlSession->executeUpdate(sql.str(), params);
}

PtrWkfServiceRequestDO WkfServiceRequestDAO::selectById(const std::string& id)
{
    if (id.empty()) return nullptr;

    std::string sql =
        "SELECT id, group_id, parent_id, status_enum, therapy_enum, "
        "based_on_table, based_on_id, patient_id, encounter_id, "
        "status_changed_time "
        "FROM wkf_service_request "
        "WHERE id = ? AND is_deleted = 0 LIMIT 1";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);

    WkfServiceRequestMapper mapper;
    return sqlSession->executeQueryOne<PtrWkfServiceRequestDO>(sql, mapper, params);
}
