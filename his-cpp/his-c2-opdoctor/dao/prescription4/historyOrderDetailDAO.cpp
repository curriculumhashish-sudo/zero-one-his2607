/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#include "stdafx.h"
#include "HistoryOrderDetailDAO.h"
#include "HistoryOrderDetailMapper.h"

std::string HistoryOrderDetailDAO::buildFullQuerySql(const HistoryOrderDetailQuery::Wrapper& query, SqlParams& params)
{
    std::string visitId = query->visitId.getValue("");
    std::string orderType = query->orderType ? query->orderType.getValue("") : "";
    std::string keyword = query->keyword ? query->keyword.getValue("") : "";

    // 药品子查询
    std::string sqlDrug =
        "SELECT "
        "  mr.id AS order_id, "
        "  '药品' AS order_type, "
        "  mmd.name AS order_name, "
        "  mr.execute_num AS quantity, "
        "  (SELECT COUNT(*) FROM med_medication_dispense WHERE med_req_id = mr.id AND is_deleted = 0) AS executed_count, "
        "  NULL AS sample_type, "
        "  org.name AS execute_hospital, "
        "  DATE_FORMAT(mr.req_authored_time, '%Y-%m-%d %H:%i') AS create_time, "
        "  cond.description AS diagnosis, "
        "  p.name AS doctor_name, "
        "  mmd.skin_test_flag AS is_skin_test "
        "FROM med_medication_request mr "
        "LEFT JOIN med_medication_definition mmd ON mr.medication_id = mmd.id "
        "LEFT JOIN adm_organization org ON mr.perform_org_id = org.id "
        "LEFT JOIN adm_practitioner p ON mr.requester_id = p.id "
        "LEFT JOIN cln_condition cond ON mr.condition_definition_id = cond.id "
        "WHERE mr.encounter_id = ? AND mr.is_deleted = 0";

    // 服务子查询（检验检查）
    std::string sqlService =
        "SELECT "
        "  sr.id AS order_id, "
        "  '检验检查' AS order_type, "
        "  ad.name AS order_name, "
        "  sr.execution_count AS quantity, "
        "  0 AS executed_count, "
        "  spec.name AS sample_type, "
        "  org.name AS execute_hospital, "
        "  DATE_FORMAT(sr.authored_time, '%Y-%m-%d %H:%i') AS create_time, "
        "  cond.description AS diagnosis, "
        "  p.name AS doctor_name, "
        "  0 AS is_skin_test "
        "FROM wkf_service_request sr "
        "LEFT JOIN wkf_activity_definition ad ON sr.activity_id = ad.id "
        "LEFT JOIN adm_organization org ON sr.org_id = org.id "
        "LEFT JOIN adm_practitioner p ON sr.requester_id = p.id "
        "LEFT JOIN cln_condition cond ON sr.condition_definition_id = cond.id "
        "LEFT JOIN adm_specimen_definition spec ON ad.specimen_code = spec.bus_no "
        "WHERE sr.encounter_id = ? AND sr.is_deleted = 0";

    // 耗材子查询
    std::string sqlDevice =
        "SELECT "
        "  dr.id AS order_id, "
        "  '耗材' AS order_type, "
        "  dd.name AS order_name, "
        "  dr.quantity AS quantity, "
        "  (SELECT COUNT(*) FROM wkf_device_dispense WHERE device_req_id = dr.id AND is_deleted = 0) AS executed_count, "
        "  NULL AS sample_type, "
        "  org.name AS execute_hospital, "
        "  DATE_FORMAT(dr.req_authored_time, '%Y-%m-%d %H:%i') AS create_time, "
        "  cond.description AS diagnosis, "
        "  p.name AS doctor_name, "
        "  0 AS is_skin_test "
        "FROM wkf_device_request dr "
        "LEFT JOIN adm_device_definition dd ON dr.device_def_id = dd.id "
        "LEFT JOIN adm_organization org ON dr.perform_org_id = org.id "
        "LEFT JOIN adm_practitioner p ON dr.requester_id = p.id "
        "LEFT JOIN cln_condition cond ON dr.condition_definition_id = cond.id "
        "WHERE dr.encounter_id = ? AND dr.is_deleted = 0";

    std::string unionSql = sqlDrug + " UNION ALL " + sqlService + " UNION ALL " + sqlDevice;

    std::string finalSql = "SELECT * FROM (" + unionSql + ") t WHERE 1=1";
    if (!orderType.empty()) {
        finalSql += " AND t.order_type = ?";
    }
    if (!keyword.empty()) {
        finalSql += " AND (t.order_name LIKE CONCAT('%', ?, '%') OR t.diagnosis LIKE CONCAT('%', ?, '%'))";
    }

    // ========== 关键修正：参数顺序必须与 SQL 中 ? 的顺序一致 ==========
    // 先推入三个 encounter_id（对应三个子查询中的 ?）
    SQLPARAMS_PUSH(params, "s", std::string, visitId);
    SQLPARAMS_PUSH(params, "s", std::string, visitId);
    SQLPARAMS_PUSH(params, "s", std::string, visitId);
    // 再推入 orderType（若存在）
    if (!orderType.empty()) {
        SQLPARAMS_PUSH(params, "s", std::string, orderType);
    }
    // 最后推入 keyword（若存在，两个）
    if (!keyword.empty()) {
        SQLPARAMS_PUSH(params, "s", std::string, keyword);
        SQLPARAMS_PUSH(params, "s", std::string, keyword);
    }

    return finalSql;
}

uint64_t HistoryOrderDetailDAO::count(const HistoryOrderDetailQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM (" + buildFullQuerySql(query, params) + ") t";
    return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PtrHistoryOrderDetailDO> HistoryOrderDetailDAO::selectWithPage(const HistoryOrderDetailQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = buildFullQuerySql(query, params);
    sql += " ORDER BY create_time DESC, order_id DESC ";

    int pageIndex = query->pageIndex ? query->pageIndex.getValue(1) : 1;
    int pageSize = query->pageSize ? query->pageSize.getValue(10) : 10;
    sql += " LIMIT " + std::to_string((pageIndex - 1) * pageSize) + "," + std::to_string(pageSize);

    return sqlSession->executeQuery<PtrHistoryOrderDetailDO>(sql, PtrHistoryOrderDetailMapper(), params);
}