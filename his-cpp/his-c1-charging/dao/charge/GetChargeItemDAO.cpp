#include "stdafx.h"
#include "GetChargeItemDAO.h"
#include "GetChargeItemMapper.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

std::list<GetChargeItemDO> GetChargeItemDAO::selectByEncounterId(const std::string& encounterId)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);

    std::ostringstream sql;
    sql << "SELECT"
        << " ci.id AS charge_item_id,"
        << " ci.service_table AS service_table,"
        << " ci.service_id AS service_id,"
        << " cid.charge_name AS item_name,"
        << " ci.bus_no AS bus_no,"
        << " ci.unit_price AS unit_price,"
        << " ci.quantity_value AS quantity_value,"
        << " ci.quantity_unit AS quantity_unit,"
        << " cid.type_code AS charge_category_code,"
        << " ci.status_enum AS status_code,"
        << " ci.total_price AS total_price,"
        << " ci.performer_id AS cashier_user_id,"
        << " doc.name AS cashier_name,"
        << " ci.account_id AS account_id,"
        << " fc.name AS contract_name,"
        << " fc.id AS contract_id,"
        << " ed.med_type AS med_type_code,"
        << " '' AS yb_no,"
        << " ci.encounter_id AS encounter_id"
        << " FROM adm_charge_item ci"
        << " LEFT JOIN adm_charge_item_definition cid ON ci.definition_id = cid.id"
        << " LEFT JOIN adm_practitioner doc ON ci.performer_id = doc.id"
        << " LEFT JOIN adm_account acc ON ci.account_id = acc.id"
        << " LEFT JOIN fin_contract fc ON acc.contract_no = fc.bus_no"
        << " LEFT JOIN ("
        << "   SELECT encounter_id,"
        << "     SUBSTRING_INDEX(GROUP_CONCAT(med_type ORDER BY primary_flag DESC, COALESCE(rank, 999) ASC), ',', 1) AS med_type"
        << "   FROM adm_encounter_diagnosis"
        << "   GROUP BY encounter_id"
        << " ) ed ON ci.encounter_id = ed.encounter_id"
        << " WHERE ci.encounter_id = ?"
        << " ORDER BY ci.id";

    return sqlSession->executeQuery<GetChargeItemDO>(sql.str(), GetChargeItemMapper(), params);
}
