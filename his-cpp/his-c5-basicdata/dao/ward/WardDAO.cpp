#include "stdafx.h"
#include "WardDAO.h"
#include "WardMapper.h"


std::list<WardDO> WardDAO::selectNameList(
    const std::string& organizationId)
{
    
    std::string sql = R"(
        SELECT id, name
        FROM adm_location
        WHERE organization_id = '%s'
          AND form_enum = 'WARD'
          AND is_deleted = 0
          AND status_enum = 'ACTIVE'
        ORDER BY display_order, name
    )";

 
    return sqlSession->executeQuery<WardDO>(
        sql,
          WardMapper(),
        "%S",
        organizationId
    );
}