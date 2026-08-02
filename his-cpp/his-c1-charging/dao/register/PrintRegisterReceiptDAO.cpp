#include "stdafx.h"
#include "PrintRegisterReceiptDAO.h"
#include "PrintRegisterReceiptMapper.h" // 引入自定义 Mapper

// ==================== 查询操作 ====================

std::string PrintRegisterReceiptDAO::queryPrintBinding(const std::string& busNo) {
    std::string sql = "SELECT binding_code FROM prt_print_binding WHERE bus_no = ? AND enabled = 1";
    return sqlSession->executeQueryOne<std::string>(sql, PrintRegisterReceiptMapper(), "%s", busNo);
}

std::string PrintRegisterReceiptDAO::queryPrintTemplate(const std::string& scenarioCode, const std::string& status) {
    std::string sql = "SELECT print_json FROM prt_template WHERE scenario_code = ? AND status = ?";
    return sqlSession->executeQueryOne<std::string>(sql, PrintRegisterReceiptMapper(), "%s%s", scenarioCode, status);
}

// ==================== 更新操作 ====================

int PrintRegisterReceiptDAO::insertPrintLog(const std::string& traceId, const std::string& bindingCode, int templateVersion,
    const std::string& printerName, int copies, const std::string& operatorId,
    const std::string& operatorName, const std::string& clientType, const std::string& bizRefJson) {
    std::string sql = "INSERT INTO prt_print_event_log "
        "(trace_id, bus_no, binding_code, template_version, status, printer_name, copies, operator_id, operator_name, client_type, biz_ref_json, created_at) "
        "VALUES (?, 'op-reg-print', ?, ?, 'SUCCESS', ?, ?, ?, ?, ?, NOW())";

    return sqlSession->executeUpdate(sql, "%s%s%i%s%i%s%s%s%s",
        traceId, bindingCode, templateVersion,
        printerName, copies, operatorId,
        operatorName, clientType, bizRefJson);
}

