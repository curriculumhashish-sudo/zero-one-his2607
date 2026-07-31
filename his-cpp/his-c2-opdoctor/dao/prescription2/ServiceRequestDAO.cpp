#include "stdafx.h"
#include "ServiceRequestDAO.h"
#include "ServiceRequestMapper.h"

PtrServiceRequestDO ServiceRequestDAO::selectById(const std::string& id)
{
    // 加载展示或更新打印信息所需字段。
    std::string sql =
        "SELECT id, print_count, updated_by, updated_at "
        "FROM wkf_service_request "
        "WHERE id=? AND is_deleted=0";

    return sqlSession->executeQueryOne<PtrServiceRequestDO>(
        sql, PtrServiceRequestMapper(), "%s", id);
}

PtrServiceRequestDO ServiceRequestDAO::selectByIdForUpdate(const std::string& id)
{
    // 锁定申请单，确保打印次数递增操作的原子性。
    std::string sql =
        "SELECT id, print_count, updated_by, updated_at "
        "FROM wkf_service_request "
        "WHERE id=? AND is_deleted=0 FOR UPDATE";

    return sqlSession->executeQueryOne<PtrServiceRequestDO>(
        sql, PtrServiceRequestMapper(), "%s", id);
}
