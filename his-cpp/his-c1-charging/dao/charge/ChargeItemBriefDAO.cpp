#include "stdafx.h"
#include "ChargeItemBriefDAO.h"
#include "ChargeItemBriefMapper.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

std::list<ChargeItemBriefDO> ChargeItemBriefDAO::selectByEncounterIds(
    const std::list<std::string>& encounterIds)
{
    if (encounterIds.empty())
        return {};

    SqlParams params;
    std::ostringstream inClause;

    // 构建 IN (?, ?, ...) 子句并绑定参数
    bool first = true;
    for (auto& id : encounterIds)
    {
        if (!first)
            inClause << ", ";
        else
            first = false;
        inClause << "?";
        SQLPARAMS_PUSH(params, "s", std::string, id);
    }

    std::ostringstream sql;
    sql << "SELECT"
        << " ci.id AS charge_item_id,"
        << " ci.status_enum AS status_code,"
        << " ci.total_price AS total_price,"
        << " cid.charge_name AS item_label,"
        << " ci.encounter_id AS encounter_id"
        << " FROM adm_charge_item ci"
        << " LEFT JOIN adm_charge_item_definition cid"
        << "   ON ci.definition_id = cid.id"
        << " WHERE ci.encounter_id IN (" << inClause.str() << ")"
        << " ORDER BY ci.encounter_id, ci.id";

    return sqlSession->executeQuery<ChargeItemBriefDO>(
        sql.str(), ChargeItemBriefMapper(), params);
}
