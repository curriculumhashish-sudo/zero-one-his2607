#include "stdafx.h"
#include "SettleChargeDAO.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

void SettleChargeDAO::insertPayment(const std::string& paymentId, const std::string& nowStr,
    const std::string& paymentNo, const std::string& encounterId,
    const std::string& totalAmountStr, const std::string& chargeItemIdsStr,
    const std::string& billDate, const std::string& invoiceId)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, paymentId);
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);
    SQLPARAMS_PUSH(params, "s", std::string, paymentNo);
    SQLPARAMS_PUSH(params, "s", std::string, "CASHIER");
    SQLPARAMS_PUSH(params, "s", std::string, "OUTPATIENT_CLINIC");
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, totalAmountStr);
    SQLPARAMS_PUSH(params, "s", std::string, totalAmountStr);
    SQLPARAMS_PUSH(params, "s", std::string, chargeItemIdsStr);
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, billDate);
    SQLPARAMS_PUSH(params, "s", std::string, invoiceId);

    std::ostringstream sql;
    sql << "INSERT INTO fin_payment_reconciliation"
        << " (id, tenant_id, created_at, is_deleted, version,"
        << " status_enum, payment_no, payment_enum, kind_enum,"
        << " enterer_id, patient_id, tendered_amount, display_amount,"
        << " print_count, charge_item_ids, encounter_id, bill_date, invoice_id)"
        << " VALUES (?,NULL,?,0,0, 'SUCCESS',?,?,?,"
        << " ?,?,?,?, 0,?,?,?,?)";

    sqlSession->executeUpdate(sql.str(), params);
}

void SettleChargeDAO::insertInvoice(const std::string& invoiceId, const std::string& nowStr,
    const std::string& encounterId, const std::string& paymentId,
    const std::string& chargeItemIdsStr, const std::string& totalAmountStr,
    const std::string& busNo)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, invoiceId);
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);
    SQLPARAMS_PUSH(params, "s", std::string, busNo);
    SQLPARAMS_PUSH(params, "s", std::string, "OUTPATIENT");
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, paymentId);
    SQLPARAMS_PUSH(params, "s", std::string, paymentId);
    SQLPARAMS_PUSH(params, "s", std::string, chargeItemIdsStr);
    SQLPARAMS_PUSH(params, "s", std::string, totalAmountStr);

    std::ostringstream sql;
    sql << "INSERT INTO adm_invoice"
        << " (id, tenant_id, created_at, is_deleted, version,"
        << " bus_no, type_code,"
        << " patient_id, reconciliation_id, payment_id,"
        << " charge_item_ids, amount, status_enum)"
        << " VALUES (?,NULL,?,0,0, ?,?, ?,?,?, ?,?,'DRAFT')";

    sqlSession->executeUpdate(sql.str(), params);
}

void SettleChargeDAO::updateChargeItemsToSettled(const std::string& chargeItemIdsStr,
    const std::string& nowStr)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);

    std::ostringstream sql;
    sql << "UPDATE adm_charge_item SET status_enum='SETTLED', updated_at=?"
        << " WHERE FIND_IN_SET(id, ?)";

    SQLPARAMS_PUSH(params, "s", std::string, chargeItemIdsStr);

    sqlSession->executeUpdate(sql.str(), params);
}
