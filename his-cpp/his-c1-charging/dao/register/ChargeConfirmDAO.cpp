#pragma once
#include "stdafx.h"
#include "ChargeConfirmDAO.h"

// --------------------- 更新支付草稿状态：DRAFT → COMPLETED ---------------------
int ChargeConfirmDAO::updatePaymentReconciliation(const std::string& paymentDraftId, const std::string& paymentEnum, const std::string& paymentNo,
    const std::string& billDate, int version) {
    std::string sql = "UPDATE fin_payment_reconciliation SET "
        "status_enum = 'COMPLETED', "
        "payment_enum = ?, "
        "outcome_enum = 'COMPLETE', "
        "bill_date = ?, "
        "version = version + 1 "
        "WHERE id = ? AND status_enum = 'DRAFT' AND version = ?";
    return sqlSession->executeUpdate(sql, "%s%s%s%s%i",
        std::string(paymentEnum), std::string(billDate),
        std::string(paymentDraftId), version);          
}

// --------------------- 插入支付明细记录 ---------------------
int ChargeConfirmDAO::insertPaymentDetail(const std::string& detailId, const std::string& reconciliationId, std::string& accountId,
    const std::string& payEnum, double amount, double chargeAmount, const std::string& payTransDate,
    const std::string& resultEnum, const std::string& targetEnum) {
    std::string sql = "INSERT INTO fin_payment_rec_detail ("
        "id, "
        "reconciliation_id, "
        "account_id, "
        "pay_enum, "
        "amount, "
        "charge_amount, "
        "pay_trans_date, "
        "result_enum, "
        "target_enum, "
        "version) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, 0)";
    return sqlSession->executeUpdate(sql, "%s%s%s%s%d%d%s%s%s",
        std::string(detailId), std::string(reconciliationId), std::string(accountId),
        std::string(payEnum), amount, chargeAmount, std::string(payTransDate),
        std::string(resultEnum), std::string(targetEnum));
}

// --------------------- 插入支付分类统计记录 ---------------------
int ChargeConfirmDAO::insertPaymentStatic(const std::string& reconciliationId, const std::string& methodCode, const std::string& typeCode,
    const std::string& typeName, double amount, const std::string& chargeItemIds) {
    std::string sql = "INSERT INTO fin_payment_rec_static ("
        "reconciliation_id, "
        "method_code, "
        "type_code, "
        "type_name, "
        "amount, "
        "charge_item_ids, "
        "version) "
        "VALUES (?, ?, ?, ?, ?, ?, 0)";
    return sqlSession->executeUpdate(sql, "%s%s%s%s%d%s",
        std::string(reconciliationId), std::string(methodCode), std::string(typeCode),
        std::string(typeName), amount, std::string(chargeItemIds));
}

// --------------------- 更新收费项状态：UNPAID → PAID ---------------------
int ChargeConfirmDAO::updateChargeItemToPaid(const std::string& paymentDraftId) {
    std::string sql = "UPDATE adm_charge_item SET "
        "status_enum = 'PAID', "
        "version = version + 1 "
        "WHERE id = ? AND status_enum = 'UNPAID'";
    return sqlSession->executeUpdate(sql, "%s%i", std::string(paymentDraftId));
}

// --------------------- 插入发票记录 ---------------------
int ChargeConfirmDAO::insertInvoice(const std::string& invoiceId, const std::string& patientId, const std::string& reconciliationId,
    const std::string& paymentId, const std::string& busNo, const std::string& billNo, const std::string& typeCode,
    double amount, const std::string& chargeItemIds, const std::string& issuedAt, const std::string& paymentTerms) {
    std::string sql = "INSERT INTO adm_invoice ("
        "id, "
        "patient_id, "
        "reconciliation_id, "
        "payment_id, "
        "bus_no, "
        "bill_no, "
        "type_code, "
        "amount, "
        "charge_item_ids, "
        "issued_at, "
        "payment_terms, "
        "version) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 0)";
    return sqlSession->executeUpdate(sql, "%s%s%s%s%s%s%s%d%s%s%s",
        std::string(invoiceId), std::string(patientId), std::string(reconciliationId),
        std::string(paymentId), std::string(busNo), std::string(billNo),
        std::string(typeCode), amount, std::string(chargeItemIds),
        std::string(issuedAt), std::string(paymentTerms));
}

// --------------------- 更新就诊状态：PENDING → REGISTERED ---------------------
int ChargeConfirmDAO::updateEncounterToRegistered(const std::string& encounterId, int version) {
    std::string sql = "UPDATE adm_encounter SET "
        "status_enum = 'REGISTERED', "
        "version = version + 1 "
        "WHERE id = ? AND status_enum = 'PENDING' AND version = ?";
    return sqlSession->executeUpdate(sql, "%s%i", std::string(encounterId), version);
}

// --------------------- 更新账户余额（扣减） ---------------------
int ChargeConfirmDAO::updateAccountBalance(const std::string& accountId, double amount, int version) {
    std::string sql = "UPDATE adm_account SET "
        "balance = balance - ?, "
        "version = version + 1 "
        "WHERE id = ? AND version = ?";
    return sqlSession->executeUpdate(sql, "%d%s%i", amount, std::string(accountId), version);
}

