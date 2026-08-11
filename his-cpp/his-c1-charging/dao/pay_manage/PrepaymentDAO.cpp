#include "stdafx.h"
#include "PrepaymentDAO.h"
#include "PrepaymentMapper.h"

#include "NacosClient.h"

// Build WHERE clause with dynamic conditions
inline std::string PrepaymentDAO::queryConditionBuilder(
    const PrepaymentQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream condition;
    condition << " WHERE target_enum = 'PREPAYMENT'";

    if (query->inpatientNo && !query->inpatientNo->empty())
    {
        condition << " AND account_code = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->inpatientNo.getValue(""));
    }

    return condition.str();
}

// Count prepayment records
uint64_t PrepaymentDAO::count(const PrepaymentQuery::Wrapper& query)
{
    SqlParams params;
    std::string condition = queryConditionBuilder(query, params);
    std::string sql = "SELECT COUNT(1) FROM fin_payment_rec_detail" + condition;
    return sqlSession->executeQueryNumerical(sql, params);
}

// Paginated query
std::list<PrepaymentDO> PrepaymentDAO::selectWithPage(const PrepaymentQuery::Wrapper& query)
{
    SqlParams params;
    std::string condition = queryConditionBuilder(query, params);

    auto pageIndex = query->pageIndex.getValue(1);
    auto pageSize = query->pageSize.getValue(10);

    std::string sql = "SELECT * FROM fin_payment_rec_detail" + condition
        + " ORDER BY pay_trans_date DESC LIMIT "
        + std::to_string(((pageIndex - 1) * pageSize))
        + "," + std::to_string(pageSize);

    PrepaymentMapper mapper;
    return sqlSession->executeQuery<PrepaymentDO, PrepaymentMapper>(sql, mapper, params);
}

// Get latest account snapshot for balance chain
AccountSnapshot PrepaymentDAO::getAccountSnapshot(const std::string& accountCode)
{
    AccountSnapshot snap;
    snap.accountId = accountCode;
    snap.balance = 0.0;
    snap.predecessorId = "";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, accountCode);

    std::string sql =
        "SELECT * FROM fin_payment_rec_detail "
        "WHERE account_code = ? AND target_enum = 'PREPAYMENT' "
        "ORDER BY pay_trans_date DESC, id DESC LIMIT 1";

    PrepaymentMapper mapper;
    auto result = sqlSession->executeQueryOne<PrepaymentDO, PrepaymentMapper>(sql, mapper, params);

    // Check if record found (default-constructed DO has null idPtr)
    auto idPtr = result.getIdPtr();
    if (idPtr)
    {
        snap.accountId = result.getAccountId();
        snap.balance = result.getAfterBalance();
        snap.predecessorId = result.getId();
    }

    return snap;
}

// Local struct and mapper for resolveEncounter JOIN query
namespace {
    struct EncounterRow {
        std::string encounterId;
        std::string patientId;
        std::string accountId;
        std::string accountCode;
        double balance;
    };

    class EncounterMapper : public Mapper<EncounterRow> {
    public:
        EncounterRow mapper(ResultSet* rs) const override {
            EncounterRow row;
            row.encounterId = rs->getString("encounter_id");
            row.patientId = rs->getString("patient_id");
            row.accountId = rs->getString("account_id");
            row.accountCode = rs->getString("account_code");
            row.balance = rs->getDouble("balance");
            return row;
        }
    };
}

// Resolve encounter info by inpatient number (adm_encounter JOIN adm_account)
EncounterInfo PrepaymentDAO::resolveEncounter(const std::string& inpatientNo)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, inpatientNo);

    std::string sql =
        "SELECT e.id AS encounter_id, e.patient_id, a.id AS account_id, a.bus_no AS account_code, a.balance "
        "FROM adm_encounter e "
        "JOIN adm_account a ON a.encounter_id = e.id "
        "WHERE e.bus_no = ? AND e.is_deleted = 0 AND a.is_deleted = 0 "
        "LIMIT 1";

    EncounterMapper mapper;
    auto row = sqlSession->executeQueryOne<EncounterRow, EncounterMapper>(sql, mapper, params);

    // Check if record found (default-constructed row has empty encounterId)
    if (row.encounterId.empty())
    {
        throw std::runtime_error("inpatient number not found: " + inpatientNo);
    }

    EncounterInfo info;
    info.encounterId = row.encounterId;
    info.patientId = row.patientId;
    info.accountId = row.accountId;
    info.accountCode = row.accountCode;
    info.balance = row.balance;
    return info;
}

// Insert payment reconciliation main record
std::string PrepaymentDAO::insertReconciliation(
    const std::string& id, const std::string& nowStr,
    const std::string& tenantId, const std::string& operatorId,
    const std::string& paymentNo, const std::string& paymentEnum,
    const std::string& patientId, const std::string& encounterId,
    const std::string& accountId,
    double tenderedAmount, double returnedAmount,
    const std::string& billDate)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, "PREPAYMENT");
    SQLPARAMS_PUSH(params, "s", std::string, paymentNo);
    SQLPARAMS_PUSH(params, "s", std::string, paymentEnum);
    SQLPARAMS_PUSH(params, "s", std::string, "PREPAYMENT");
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, patientId);
    SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    SQLPARAMS_PUSH(params, "s", std::string, accountId);
    SQLPARAMS_PUSH(params, "d", double, tenderedAmount);
    SQLPARAMS_PUSH(params, "d", double, returnedAmount);
    SQLPARAMS_PUSH(params, "d", double, tenderedAmount);
    SQLPARAMS_PUSH(params, "s", std::string, billDate);
    SQLPARAMS_PUSH(params, "s", std::string, std::string(""));
    SQLPARAMS_PUSH(params, "i", int32_t, 0);

    std::ostringstream sql;
    sql << "INSERT INTO fin_payment_reconciliation"
        << " (id, tenant_id, created_at, updated_at, created_by, updated_by,"
        << " is_deleted, version,"
        << " status_enum, payment_no, payment_enum, kind_enum,"
        << " enterer_id, patient_id, encounter_id, account_id,"
        << " tendered_amount, returned_amount, display_amount,"
        << " bill_date, charge_item_ids, print_count)"
        << " VALUES (?,?,?,?,?,?, 0,0, ?,?,?,?, ?,?,?,?, ?,?,?, ?, ?,?)";

    int row = sqlSession->executeUpdate(sql.str(), params);
    if (row != 1)
    {
        throw std::runtime_error("insertReconciliation failed: rows affected=" + std::to_string(row));
    }

    return id;
}

// Update account balance in adm_account
void PrepaymentDAO::updateAccountBalance(const std::string& accountId, double newBalance,
    const std::string& nowStr)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "d", double, newBalance);
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);
    SQLPARAMS_PUSH(params, "s", std::string, accountId);

    std::string sql =
        "UPDATE adm_account SET balance = ?, updated_at = ? WHERE id = ? AND is_deleted = 0";

    int row = sqlSession->executeUpdate(sql, params);
    if (row != 1)
    {
        throw std::runtime_error("updateAccountBalance failed: rows affected=" + std::to_string(row));
    }
}

// Insert prepayment detail record using native SQL (charge or refund)
void PrepaymentDAO::insertPrepayment(
    const std::string& id, const std::string& nowStr,
    const std::string& tenantId, const std::string& operatorName,
    const std::string& reconciliationId,
    const std::string& accountId, const std::string& accountCode,
    const std::string& payEnum, double amount,
    double chargeAmount, double returnAmount,
    const std::string& payTransNo, const std::string& payTransText,
    const std::string& payTransDate,
    double beforeBalance, double afterBalance,
    const std::string& predecessorId)
{
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);
    SQLPARAMS_PUSH(params, "s", std::string, nowStr);
    SQLPARAMS_PUSH(params, "s", std::string, operatorName);
    SQLPARAMS_PUSH(params, "s", std::string, operatorName);
    SQLPARAMS_PUSH(params, "s", std::string, reconciliationId);
    SQLPARAMS_PUSH(params, "s", std::string, "PREPAYMENT");
    SQLPARAMS_PUSH(params, "s", std::string, accountId);
    SQLPARAMS_PUSH(params, "s", std::string, accountCode);
    SQLPARAMS_PUSH(params, "s", std::string, payEnum);
    SQLPARAMS_PUSH(params, "d", double, amount);
    SQLPARAMS_PUSH(params, "d", double, chargeAmount);
    SQLPARAMS_PUSH(params, "d", double, returnAmount);
    SQLPARAMS_PUSH(params, "s", std::string, payTransNo);
    SQLPARAMS_PUSH(params, "s", std::string, payTransText);
    SQLPARAMS_PUSH(params, "s", std::string, payTransDate);
    SQLPARAMS_PUSH(params, "d", double, beforeBalance);
    SQLPARAMS_PUSH(params, "d", double, afterBalance);
    SQLPARAMS_PUSH(params, "s", std::string, "SUCCESS");

    // predecessor_id: NULL for first record, otherwise links to previous record ID
    if (!predecessorId.empty())
    {
        SQLPARAMS_PUSH(params, "s", std::string, predecessorId);
    }

    std::ostringstream sql;
    sql << "INSERT INTO fin_payment_rec_detail"
        << " (id, tenant_id, created_at, updated_at, created_by, updated_by,"
        << " is_deleted, version,"
        << " reconciliation_id, target_enum,"
        << " account_id, account_code,"
        << " pay_enum, amount, charge_amount, return_amount,"
        << " pay_trans_no, pay_trans_text, pay_trans_date,"
        << " before_balance, after_balance, result_enum"
        << (predecessorId.empty() ? "" : ", predecessor_id")
        << ")"
        << " VALUES (?,?,?,?,?,?, 0,0, ?,?, ?,?, ?,?,?,?, ?,?,?, ?,?,?"
        << (predecessorId.empty() ? "" : ",?")
        << ")";

    int row = sqlSession->executeUpdate(sql.str(), params);
    if (row != 1)
    {
        throw std::runtime_error("insertPrepayment failed: rows affected=" + std::to_string(row));
    }
}
