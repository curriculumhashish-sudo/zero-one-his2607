#include "stdafx.h"
#include "WkfServiceRequestDAO.h"
#include "WkfServiceRequestMapper.h"

//批量停嘱诊疗 (IF-IP-DOR-20)
int WkfServiceRequestDAO::stopOrderBatch(
    const std::list<std::string>& orderIds,
    int stopReason,
    const std::string& operatorId)
{
    //空列表直接返回
    if (orderIds.empty()) return 0;

    SqlParams params;
    std::ostringstream sql;

    sql << "UPDATE wkf_service_request "
        << "SET status_enum = 'STOPPED', "
        << "    status_reason = ?, "
        << "    status_changed_time = NOW(), "
        << "    updated_at = NOW() "
        << "WHERE id IN (";

    //绑定停嘱原因参数
    SQLPARAMS_PUSH(params, "i", int, stopReason);

    //循环绑定批量ID
    bool first = true;
    for (auto& id : orderIds) {
        if (!first) sql << ",";
        else first = false;
        sql << "?";
        SQLPARAMS_PUSH(params, "s", std::string, id);
    }

    sql << ") AND status_enum != 'STOPPED' AND is_deleted = 0";

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