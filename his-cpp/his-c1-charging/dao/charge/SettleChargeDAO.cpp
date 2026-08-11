#include "stdafx.h"
#include "SettleChargeDAO.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

// ---- Transaction control ----

void SettleChargeDAO::beginTransaction()
{
    sqlSession->beginTransaction();
}

void SettleChargeDAO::commitTransaction()
{
    sqlSession->commitTransaction();
}

void SettleChargeDAO::rollbackTransaction()
{
    sqlSession->rollbackTransaction();
}

// ---- Read ----

namespace
{
    struct EncounterPatient
    {
        std::string patientId;
    };

    class EncounterPatientMapper : public Mapper<EncounterPatient>
    {
    public:
        EncounterPatient mapper(ResultSet* rs) const override
        {
            EncounterPatient result;
            result.patientId = rs->getString("patient_id");
            return result;
        }
    };

    class AccountInfoMapper : public Mapper<SettleChargeDAO::AccountInfo>
    {
    public:
        SettleChargeDAO::AccountInfo mapper(ResultSet* rs) const override
        {
            SettleChargeDAO::AccountInfo result;
            result.accountId = rs->getString("id");
            result.accountCode = rs->getString("bus_no");
            return result;
        }
    };
}

std::string SettleChargeDAO::selectPatientIdByEncounter(const std::string& encounterId)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);

    std::ostringstream sql;
    sql << "SELECT patient_id FROM adm_encounter"
        << " WHERE id = ? AND is_deleted = 0";

    auto result = sqlSession->executeQueryOne<EncounterPatient>(
        sql.str(), EncounterPatientMapper(), params);
    return result.patientId;
}

SettleChargeDAO::AccountInfo SettleChargeDAO::selectAccountByEncounter(const std::string& encounterId)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);

    std::ostringstream sql;
    sql << "SELECT id, bus_no FROM adm_account"
        << " WHERE encounter_id = ? AND is_deleted = 0";

    auto result = sqlSession->executeQueryOne<AccountInfo>(
        sql.str(), AccountInfoMapper(), params);
    return result;
}

int SettleChargeDAO::insertPayment(const std::string& paymentId, const std::string& nowStr,
    const std::string& paymentNo, const std::string& entererId,
    const std::string& patientId, const std::string& totalAmountStr,
    const std::string& chargeItemIdsStr,
    const std::string& billDate, const std::string& invoiceId,
    const std::string& encounterId)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, paymentId);
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);
    SQLPARAMS_PUSH(params, "s", std::string, paymentNo);
    SQLPARAMS_PUSH(params, "s", std::string, "CASHIER");
    SQLPARAMS_PUSH(params, "s", std::string, "OUTPATIENT_CLINIC");
    SQLPARAMS_PUSH(params, "s", std::string, entererId);
    SQLPARAMS_PUSH(params, "s", std::string, patientId);
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

    return sqlSession->executeUpdate(sql.str(), params);
}

int SettleChargeDAO::insertPaymentDetail(const std::string& detailId, const std::string& nowStr,
    const std::string& reconciliationId, const std::string& payChannelCode,
    const std::string& amountStr, const std::string& entererId,
    const std::string& accountId, const std::string& accountCode)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, detailId);
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);
    SQLPARAMS_PUSH(params, "s", std::string, reconciliationId);
    SQLPARAMS_PUSH(params, "s", std::string, payChannelCode);
    SQLPARAMS_PUSH(params, "s", std::string, amountStr);
    SQLPARAMS_PUSH(params, "s", std::string, amountStr);
    SQLPARAMS_PUSH(params, "s", std::string, accountId);
    SQLPARAMS_PUSH(params, "s", std::string, accountCode);
    SQLPARAMS_PUSH(params, "s", std::string, entererId);

    std::ostringstream sql;
    sql << "INSERT INTO fin_payment_rec_detail"
        << " (id, tenant_id, created_at, is_deleted, version,"
        << " reconciliation_id, pay_enum, amount, charge_amount,"
        << " account_id, account_code, result_enum, created_by)"
        << " VALUES (?,NULL,?,0,0,"
        << " ?,?,?,?,"
        << " ?,?,'SUCCESS',?)";

    return sqlSession->executeUpdate(sql.str(), params);
}

int SettleChargeDAO::insertInvoice(const std::string& invoiceId, const std::string& nowStr,
    const std::string& patientId, const std::string& paymentId,
    const std::string& chargeItemIdsStr, const std::string& totalAmountStr,
    const std::string& busNo)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, invoiceId);
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);
    SQLPARAMS_PUSH(params, "s", std::string, busNo);
    SQLPARAMS_PUSH(params, "s", std::string, "OUTPATIENT");
    SQLPARAMS_PUSH(params, "s", std::string, patientId);
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

    return sqlSession->executeUpdate(sql.str(), params);
}

int SettleChargeDAO::updateChargeItemsToSettled(const std::string& chargeItemIdsStr,
    const std::string& nowStr)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);

    std::ostringstream sql;
    sql << "UPDATE adm_charge_item SET status_enum='SETTLED', updated_at=?"
        << " WHERE FIND_IN_SET(id, ?)";

    SQLPARAMS_PUSH(params, "s", std::string, chargeItemIdsStr);

    return sqlSession->executeUpdate(sql.str(), params);
}
