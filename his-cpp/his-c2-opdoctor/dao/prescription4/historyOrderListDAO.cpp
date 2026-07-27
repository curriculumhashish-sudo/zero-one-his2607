/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#include "stdafx.h"
#include "historyOrderListDAO.h"
#include "historyOrderListMapper.h"

// ========== 查询条件构建器 ==========
std::string HistoryOrderListDAO::queryConditionBuilder(const HistoryOrderListQuery::Wrapper& query, SqlParams& params)
{
    std::stringstream sqlCondition;
    sqlCondition << " WHERE e.is_deleted = 0 AND e.patient_id = ?";
    SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));

    // 就诊类型过滤
    if (query->visitType && !query->visitType->empty()) {
        sqlCondition << " AND e.class_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->visitType.getValue(""));
    }

    // 关键词模糊匹配（主诊断 或 医生姓名）
    if (query->keyword && !query->keyword->empty()) {
        sqlCondition << " AND ("
            << "EXISTS (SELECT 1 FROM adm_encounter_diagnosis ed "
            << "LEFT JOIN cln_condition c ON ed.condition_id = c.id "
            << "WHERE ed.encounter_id = e.id AND ed.primary_flag = 1 "
            << "AND c.description LIKE CONCAT('%', ?, '%')) "
            << "OR EXISTS (SELECT 1 FROM adm_encounter_participant ep "
            << "LEFT JOIN adm_practitioner p ON ep.practitioner_id = p.id "
            << "WHERE ep.encounter_id = e.id AND ep.role_code = 'primary' "
            << "AND p.name LIKE CONCAT('%', ?, '%'))"
            << ")";
        SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
        SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
    }

    return sqlCondition.str();
}

// ========== 统计总数 ==========
uint64_t HistoryOrderListDAO::count(const HistoryOrderListQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM adm_encounter e";
    sql += queryConditionBuilder(query, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

// ========== 分页查询 ==========
std::list<PtrHistoryOrderListDO> HistoryOrderListDAO::selectWithPage(const HistoryOrderListQuery::Wrapper& query)
{
    SqlParams params;

    std::string sql =
        "SELECT "
        "e.id AS visit_id, "
        "DATE_FORMAT(COALESCE(e.start_time, e.created_at), '%Y-%m-%d %H:%i') AS visit_time, "
        "e.class_enum AS visit_type, "
        "c.description AS main_diagnosis, "
        "p.name AS doctor_name, "
        "COALESCE(drug.cnt, 0) AS drug_count, "
        "COALESCE(exam.cnt, 0) AS exam_count, "
        "COALESCE(material.cnt, 0) AS material_count "
        "FROM adm_encounter e "
        "LEFT JOIN adm_encounter_diagnosis ed ON e.id = ed.encounter_id AND ed.primary_flag = 1 "
        "LEFT JOIN cln_condition c ON ed.condition_id = c.id "
        "LEFT JOIN adm_encounter_participant ep ON e.id = ep.encounter_id AND ep.role_code = 'primary' "
        "LEFT JOIN adm_practitioner p ON ep.practitioner_id = p.id "
        "LEFT JOIN (SELECT encounter_id, COUNT(*) AS cnt FROM med_medication_request WHERE is_deleted = 0 GROUP BY encounter_id) drug ON e.id = drug.encounter_id "
        "LEFT JOIN (SELECT encounter_id, COUNT(*) AS cnt FROM wkf_service_request WHERE is_deleted = 0 GROUP BY encounter_id) exam ON e.id = exam.encounter_id "
        "LEFT JOIN (SELECT encounter_id, COUNT(*) AS cnt FROM wkf_device_request WHERE is_deleted = 0 GROUP BY encounter_id) material ON e.id = material.encounter_id";

    sql += queryConditionBuilder(query, params);
    sql += " ORDER BY COALESCE(e.start_time, e.created_at) DESC, e.id DESC ";

    int pageIndex = query->pageIndex ? query->pageIndex.getValue(1) : 1;
    int pageSize = query->pageSize ? query->pageSize.getValue(10) : 10;
    sql += " LIMIT " + std::to_string((pageIndex - 1) * pageSize) + "," + std::to_string(pageSize);

    return sqlSession->executeQuery<PtrHistoryOrderListDO>(sql, PtrHistoryOrderListMapper(), params);
}