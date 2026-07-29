#include "stdafx.h"
#include "QuitNumberDAO.h"

int QuitNumberDAO::updateEncounterToCancelled(const std::string& id, int version) {
    std::string sql = "UPDATE adm_encounter SET "
        "status_enum = 'CANCELLED', end_time = NOW(), version = version + 1 "
        "WHERE id = ? AND status_enum = 'REGISTERED' AND version = ?";
    return sqlSession->executeUpdate(sql, "%s%i", id, version);
}

int QuitNumberDAO::updateQueueToCancelled(const std::string& encounterId, const std::string& cancelReason) {
    std::string sql = "UPDATE wkf_appointment_queue SET "
        "cancel_reason = ?, cancellation_reason_code = 'PATIENT_CANCEL', cancellation_at = NOW(), version = version + 1 "
        "WHERE encounter_id = ?";
    return sqlSession->executeUpdate(sql, "%s%s", cancelReason, encounterId);
}

int QuitNumberDAO::releaseSlot(const std::string& slotId) {
    std::string sql = "UPDATE wkf_slot SET "
        "booked_count = booked_count - 1, "
        "status_code = CASE WHEN booked_count - 1 < capacity THEN 'free' ELSE status_code END, "
        "version = version + 1 "
        "WHERE id = ?";
    return sqlSession->executeUpdate(sql, "%s", slotId);
}

int QuitNumberDAO::updateChargeItemToRefunded(const std::string& chargeItemId, const std::string& refundId, int version) {
    std::string sql = "UPDATE adm_charge_item SET "
        "status_enum = 'REFUNDED', refund_id = ?, version = version + 1 "
        "WHERE id = ? AND status_enum = 'PAID' AND version = ?";
    return sqlSession->executeUpdate(sql, "%s%s%i", refundId, chargeItemId, version);
}

int QuitNumberDAO::insertRefundPayment(const std::string& id, const std::string& patientId, const std::string& encounterId,
    const std::string& paymentNo, const std::string& paymentEnum, double amount,
    const std::string& chargeItemIds, const std::string& entererId) {
    std::string sql = "INSERT INTO fin_payment_reconciliation ("
        "id, status_enum, patient_id, encounter_id, payment_no, payment_enum, kind_enum, "
        "tendered_amount, display_amount, charge_item_ids, enterer_id, bill_date, version) "
        "VALUES (?, 'COMPLETED', ?, ?, ?, ?, 'REFUND', ?, ?, ?, ?, NOW(), 0)";
    return sqlSession->executeUpdate(sql, "%s%s%s%s%s%d%d%s%s",
        id, patientId, encounterId,
        paymentNo, paymentEnum,
        amount, amount, chargeItemIds, entererId);
}

int QuitNumberDAO::insertRefundDetail(const std::string& id, const std::string& reconciliationId,
    const std::string& payEnum, double returnAmount) {
    std::string sql = "INSERT INTO fin_payment_rec_detail ("
        "id, reconciliation_id, pay_enum, amount, charge_amount, return_amount, pay_trans_date, result_enum, version) "
        "VALUES (?, ?, ?, 0, 0, ?, NOW(), 'SUCCESS', 0)";
    return sqlSession->executeUpdate(sql, "%s%s%s%d",
        id, reconciliationId, payEnum, returnAmount);
}


int QuitNumberDAO::updateInvoiceToCancelled(const std::string& invoiceId, const std::string& cancelledReason) {
    std::string sql = "UPDATE adm_invoice SET "
        "status_enum = 'CANCELLED', cancelled_reason = ?, version = version + 1 "
        "WHERE id = ? AND status_enum = 'ISSUED'";
    return sqlSession->executeUpdate(sql, "%s%s", cancelledReason, invoiceId);
}


int QuitNumberDAO::updateAccountBalanceRefund(const std::string& accountId, double amount) {
    std::string sql = "UPDATE adm_account SET "
        "balance = balance + ?, version = version + 1 "
        "WHERE id = ?";
    return sqlSession->executeUpdate(sql, "%d%s", amount, accountId);
}
