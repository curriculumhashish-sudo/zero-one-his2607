#include "stdafx.h"
#include "PrintChargeReceiptDAO.h"
#include "PrintChargeReceiptMapper.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

std::list<PrintChargeReceiptSlipDO> PrintChargeReceiptDAO::selectSlipByPaymentId(const std::string& paymentId)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, paymentId);

    std::ostringstream sql;
    sql << "SELECT"
        << " e.id AS encounter_id,"
        << " p.name AS patient_name,"
        << " org.name AS hospital_name,"
        << " p.gender_enum AS gender_enum_text,"
        << " '' AS age_text,"
        << " '' AS fee_nature_text,"
        << " CAST(fpr.display_amount AS CHAR) AS total_amount_text,"
        << " doc.name AS cashier_name,"
        << " CAST(fpr.bill_date AS CHAR) AS charge_time_text,"
        << " fpr.id AS payment_id"
        << " FROM fin_payment_reconciliation fpr"
        << " JOIN adm_encounter e ON fpr.encounter_id = e.id"
        << " JOIN adm_patient p ON e.patient_id = p.id"
        << " LEFT JOIN adm_organization org ON e.organization_id = org.id"
        << " LEFT JOIN adm_practitioner doc ON fpr.enterer_id = doc.user_id"
        << " WHERE fpr.id = ?";

    return sqlSession->executeQuery<PrintChargeReceiptSlipDO>(sql.str(), PrintChargeReceiptSlipMapper(), params);
}

std::list<PrintChargeReceiptSlipDO> PrintChargeReceiptDAO::selectSlipByEncounterId(const std::string& encounterId)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);

    std::ostringstream sql;
    sql << "SELECT"
        << " e.id AS encounter_id,"
        << " p.name AS patient_name,"
        << " org.name AS hospital_name,"
        << " p.gender_enum AS gender_enum_text,"
        << " '' AS age_text,"
        << " '' AS fee_nature_text,"
        << " CAST(fpr.display_amount AS CHAR) AS total_amount_text,"
        << " doc.name AS cashier_name,"
        << " CAST(fpr.bill_date AS CHAR) AS charge_time_text,"
        << " fpr.id AS payment_id"
        << " FROM fin_payment_reconciliation fpr"
        << " JOIN adm_encounter e ON fpr.encounter_id = e.id"
        << " JOIN adm_patient p ON e.patient_id = p.id"
        << " LEFT JOIN adm_organization org ON e.organization_id = org.id"
        << " LEFT JOIN adm_practitioner doc ON fpr.enterer_id = doc.user_id"
        << " WHERE fpr.encounter_id = ? AND fpr.status_enum = 'SUCCESS'"
        << " ORDER BY fpr.created_at DESC LIMIT 1";

    return sqlSession->executeQuery<PrintChargeReceiptSlipDO>(sql.str(), PrintChargeReceiptSlipMapper(), params);
}

std::list<PrintChargeReceiptTemplateDO> PrintChargeReceiptDAO::selectTemplateByScenario(const std::string& scenarioCode)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, scenarioCode);

    std::ostringstream sql;
    sql << "SELECT bus_no, scenario_code, version,"
        << " CAST(print_json AS CHAR) AS print_json,"
        << " CAST(sample_print_data AS CHAR) AS sample_print_data"
        << " FROM prt_template"
        << " WHERE scenario_code = ? AND status = 'ACTIVE'"
        << " ORDER BY version DESC LIMIT 1";

    return sqlSession->executeQuery<PrintChargeReceiptTemplateDO>(sql.str(), PrintChargeReceiptTemplateMapper(), params);
}

int PrintChargeReceiptDAO::insertPrintLog(const std::string& traceId, const std::string& busNo,
    int version, const std::string& status,
    const std::string& printer, int copies, const std::string& clientType,
    const std::string& errorCode, const std::string& errorMessage,
    const std::string& bizRef)
{
    SqlParams params;
    std::ostringstream sql;
    sql << "INSERT INTO prt_print_event_log"
        << " (id, tenant_id, trace_id, bus_no, template_version, status,"
        << " printer_name, copies, error_code, error_message,"
        << " biz_ref_json, client_type, created_at, is_deleted)"
        << " VALUES (?, 'tenant_default', ?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), 0)";

    std::string logId = traceId + "_log";
    SQLPARAMS_PUSH(params, "s", std::string, logId);
    SQLPARAMS_PUSH(params, "s", std::string, traceId);
    SQLPARAMS_PUSH(params, "s", std::string, busNo);
    SQLPARAMS_PUSH(params, "i", int32_t, version);
    SQLPARAMS_PUSH(params, "s", std::string, status);
    SQLPARAMS_PUSH(params, "s", std::string, printer);
    SQLPARAMS_PUSH(params, "i", int32_t, copies);
    SQLPARAMS_PUSH(params, "s", std::string, errorCode);
    SQLPARAMS_PUSH(params, "s", std::string, errorMessage);
    SQLPARAMS_PUSH(params, "s", std::string, bizRef);
    SQLPARAMS_PUSH(params, "s", std::string, clientType);

    return sqlSession->executeUpdate(sql.str(), params);
}
