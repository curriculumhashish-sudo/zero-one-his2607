/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/25
*/
#include "stdafx.h"
#include "Chinapres2DAO.h"
#include "Chinapres2Mapper.h"
#include "Chinapres2DetailMapper.h"

// ========== 查询条件构建器 ==========
std::string Chinapres2DAO::queryConditionBuilder(const Chinapres2Query::Wrapper& query, SqlParams& params)
{
    std::stringstream sqlCondition;
    sqlCondition << " WHERE m.is_deleted = 0";

    if (query->preNum) {
        sqlCondition << " AND m.execute_num = ?";
        SQLPARAMS_PUSH(params, "i", int, query->preNum.getValue(0));
    }

    if (query->medicine && !query->medicine->empty()) {
        sqlCondition << " AND EXISTS (SELECT 1 FROM med_medication_request_detail d WHERE d.request_id = m.id AND d.medication_id LIKE CONCAT('%', ?, '%'))";
        SQLPARAMS_PUSH(params, "s", std::string, query->medicine.getValue(""));
    }

    return sqlCondition.str();
}

// ========== 统计总数 ==========
uint64_t Chinapres2DAO::count(const Chinapres2Query::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM med_medication_request m";
    sql += queryConditionBuilder(query, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

// ========== 分页查询（联查） ==========
std::list<PtrChinapres2ViewDO> Chinapres2DAO::selectWithPage(const Chinapres2Query::Wrapper& query)
{
    SqlParams params;

    std::string sql =
        "SELECT "
        "m.id, "
        "m.bus_no, "
        "m.prescription_no, "
        "m.group_id, "
        "m.status_enum, "
        "m.is_deleted, "
        "m.patient_id, "
        "m.encounter_id, "
        "m.requester_id, "
        "m.org_id, "
        "m.method_code, "
        "m.rate_code, "
        "m.execute_num, "
        "m.chinese_herbs_dose_quantity, "
        "m.suffering_flag, "
        "m.encounter_diagnosis_id, "
        "m.condition_definition_id, "
        "m.dosage_instruction, "
        "m.remark, "
        "m.created_at, "
        "m.status_reason, "
        "c.description AS diagnosis_name, "
        "p.name AS requester_name, "
        "(SELECT GROUP_CONCAT(CONCAT(d.quantity, d.unit_code) SEPARATOR ', ') "
        "FROM med_medication_request_detail d WHERE d.request_id = m.id) AS medication_names, "
        "0 AS total_price "
        "FROM med_medication_request m "
        "LEFT JOIN adm_encounter_diagnosis ed ON m.encounter_diagnosis_id = ed.id "
        "LEFT JOIN cln_condition c ON ed.condition_id = c.id "
        "LEFT JOIN adm_practitioner p ON m.requester_id = p.id";

    sql += queryConditionBuilder(query, params);
    sql += " ORDER BY m.created_at DESC, m.id DESC ";

    int pageIndex = query->pageIndex ? query->pageIndex.getValue(1) : 1;
    int pageSize = query->pageSize ? query->pageSize.getValue(10) : 10;
    sql += " LIMIT " + std::to_string((pageIndex - 1) * pageSize) + "," + std::to_string(pageSize);

    return sqlSession->executeQuery<PtrChinapres2ViewDO>(sql, Chinapres2ViewMapper(), params);
}

// ========== 根据 ID 查询主表 ==========
PtrChinapres2MainDO Chinapres2DAO::selectById(const std::string& id)
{
    std::string sql =
        "SELECT "
        "id, bus_no, prescription_no, group_id, status_enum, is_deleted, "
        "patient_id, encounter_id, requester_id, org_id, "
        "method_code, rate_code, execute_num, chinese_herbs_dose_quantity, "
        "suffering_flag, encounter_diagnosis_id, condition_definition_id, "
        "dosage_instruction, remark, created_at, status_reason "
        "FROM med_medication_request "
        "WHERE id = ? AND is_deleted = 0";
    return sqlSession->executeQueryOne<PtrChinapres2MainDO>(sql, PtrChinapres2Mapper(), "%s", id);
}

// ========== 根据主表 ID 查询明细列表 ==========
std::list<PtrChinapres2DetailDO> Chinapres2DAO::selectDetailsByRequestIdPtr(const std::string& requestId)
{
    std::string sql =
        "SELECT "
        "id, request_id, medication_id, quantity, unit_code, "
        "dose, dose_unit_code, sort_no, "
        "create_by, create_time, update_by, update_time "
        "FROM med_medication_request_detail "
        "WHERE request_id = ? "
        "ORDER BY sort_no ASC, id ASC";
    return sqlSession->executeQuery<PtrChinapres2DetailDO>(sql, PtrChinapres2DetailMapper(), "%s", requestId);
}

// ========== 插入主表 ==========
bool Chinapres2DAO::insertMain(const Chinapres2MainDO& data)
{
    std::string sql =
        "INSERT INTO med_medication_request ("
        "id, bus_no, prescription_no, group_id, status_enum, is_deleted, "
        "patient_id, encounter_id, requester_id, org_id, "
        "method_code, rate_code, execute_num, chinese_herbs_dose_quantity, "
        "suffering_flag, encounter_diagnosis_id, condition_definition_id, "
        "dosage_instruction, remark, created_at, status_reason"
        ") VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, data.getId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getBusNo());
    SQLPARAMS_PUSH(params, "s", std::string, data.getPrescriptionNo());
    SQLPARAMS_PUSH(params, "s", std::string, data.getGroupId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getStatusEnum());
    SQLPARAMS_PUSH(params, "i", int, data.getIsDeleted());
    SQLPARAMS_PUSH(params, "s", std::string, data.getPatientId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getEncounterId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getRequesterId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getOrgId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getMethodCode());
    SQLPARAMS_PUSH(params, "s", std::string, data.getRateCode());
    SQLPARAMS_PUSH(params, "i", int, data.getExecuteNum());
    SQLPARAMS_PUSH(params, "d", double, data.getChineseHerbsDoseQuantity());
    SQLPARAMS_PUSH(params, "i", int, data.getSufferingFlag());
    SQLPARAMS_PUSH(params, "s", std::string, data.getEncounterDiagnosisId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getConditionDefinitionId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getDosageInstruction());
    SQLPARAMS_PUSH(params, "s", std::string, data.getRemark());
    SQLPARAMS_PUSH(params, "s", std::string, data.getCreatedAt());
    SQLPARAMS_PUSH(params, "s", std::string, data.getStatusReason());

    return sqlSession->executeUpdate(sql, params) > 0;
}

// ========== 批量插入明细表 ==========
bool Chinapres2DAO::insertDetailsPtr(const std::list<PtrChinapres2DetailDO>& details)
{
    if (details.empty()) return true;

    for (auto& data : details) {
        std::string sql =
            "INSERT INTO med_medication_request_detail ("
            "id, request_id, medication_id, quantity, unit_code, dose, dose_unit_code, sort_no, created_at"
            ") VALUES ('"
            + data->getId() + "', '"
            + data->getRequestId() + "', '"
            + data->getMedicationId() + "', "
            + std::to_string(data->getQuantity()) + ", '"
            + data->getUnitCode() + "', "
            + std::to_string(data->getDose()) + ", '"
            + data->getDoseUnitCode() + "', "
            + std::to_string(data->getSortNo()) + ", '"
            + data->getCreateTime() + "')";

        if (sqlSession->executeUpdate(sql) <= 0) {
            return false;
        }
    }
    return true;
}

// ========== 更新主表 ==========
bool Chinapres2DAO::updateMain(const Chinapres2MainDO& data)
{
    std::string sql =
        "UPDATE med_medication_request SET "
        "bus_no = ?, "
        "prescription_no = ?, "
        "group_id = ?, "
        "status_enum = ?, "
        "is_deleted = ?, "
        "patient_id = ?, "
        "encounter_id = ?, "
        "requester_id = ?, "
        "org_id = ?, "
        "method_code = ?, "
        "rate_code = ?, "
        "execute_num = ?, "
        "chinese_herbs_dose_quantity = ?, "
        "suffering_flag = ?, "
        "encounter_diagnosis_id = ?, "
        "condition_definition_id = ?, "
        "dosage_instruction = ?, "
        "remark = ?, "
        "status_reason = ? "
        "WHERE id = ?";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, data.getBusNo());
    SQLPARAMS_PUSH(params, "s", std::string, data.getPrescriptionNo());
    SQLPARAMS_PUSH(params, "s", std::string, data.getGroupId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getStatusEnum());
    SQLPARAMS_PUSH(params, "i", int, data.getIsDeleted());
    SQLPARAMS_PUSH(params, "s", std::string, data.getPatientId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getEncounterId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getRequesterId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getOrgId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getMethodCode());
    SQLPARAMS_PUSH(params, "s", std::string, data.getRateCode());
    SQLPARAMS_PUSH(params, "i", int, data.getExecuteNum());
    SQLPARAMS_PUSH(params, "d", double, data.getChineseHerbsDoseQuantity());
    SQLPARAMS_PUSH(params, "i", int, data.getSufferingFlag());
    SQLPARAMS_PUSH(params, "s", std::string, data.getEncounterDiagnosisId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getConditionDefinitionId());
    SQLPARAMS_PUSH(params, "s", std::string, data.getDosageInstruction());
    SQLPARAMS_PUSH(params, "s", std::string, data.getRemark());
    SQLPARAMS_PUSH(params, "s", std::string, data.getStatusReason());
    SQLPARAMS_PUSH(params, "s", std::string, data.getId());

    return sqlSession->executeUpdate(sql, params) > 0;
}

// ========== 根据 ID 软删除（主表 + 明细表） ==========
bool Chinapres2DAO::softDeleteById(const std::string& id)
{
    std::string detailSql = "UPDATE med_medication_request_detail SET is_deleted = 1 WHERE request_id = ?";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    sqlSession->executeUpdate(detailSql, params);

    std::string mainSql = "UPDATE med_medication_request SET is_deleted = 1 WHERE id = ?";
    SqlParams mainParams;
    SQLPARAMS_PUSH(mainParams, "s", std::string, id);
    return sqlSession->executeUpdate(mainSql, mainParams) > 0;
}

// ========== 批量软删除 ==========
bool Chinapres2DAO::softDeleteBatch(const std::list<std::string>& ids)
{
    if (ids.empty()) return true;

    std::string placeholders;
    SqlParams params;
    for (auto& id : ids) {
        if (!placeholders.empty()) placeholders += ", ";
        placeholders += "?";
        SQLPARAMS_PUSH(params, "s", std::string, id);
    }

    std::string detailSql = "UPDATE med_medication_request_detail SET is_deleted = 1 WHERE request_id IN (" + placeholders + ")";
    sqlSession->executeUpdate(detailSql, params);

    std::string mainSql = "UPDATE med_medication_request SET is_deleted = 1 WHERE id IN (" + placeholders + ")";
    return sqlSession->executeUpdate(mainSql, params) > 0;
}