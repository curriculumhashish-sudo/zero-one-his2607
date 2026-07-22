#include "stdafx.h"
#include "WardDAO.h"
#include "WardMapper.h"


std::list<WardDO> WardDAO::selectNameList(
    const std::string& organizationId)
{
    // 1. Define SQL
    std::string sql = R"(
        SELECT id, name
        FROM adm_location
        WHERE organization_id = ?
          AND form_enum = 'WARD'
          AND is_deleted = 0
          AND status_enum = 'ACTIVE'
        ORDER BY display_order, name
    )";

    // 2. Execute query
    return sqlSession->executeQuery<WardDO>(
        sql,
          WardMapper(),
        "%S",
        organizationId
    );
}