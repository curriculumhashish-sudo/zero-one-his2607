#include "stdafx.h"
#include "PrepaymentDAO.h"
#include "PrepaymentMapper.h"

// Build query conditions, filter by inpatient number
inline std::string PrepaymentDAO::queryConditionBuilder(
    const PrepaymentQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream condition;
    condition << " WHERE 1=1";

    // Only prepayment type records
    condition << " AND target_enum = 'PREPAYMENT'";

    // Filter by inpatient number (account_code), optional
    if (query->inpatientNo)
    {
        condition << " AND account_code = ?";
        params.push_back(SqlParam("s", std::make_shared<std::string>(
            query->inpatientNo->c_str())));
    }

    return condition.str();
}

// Count records
uint64_t PrepaymentDAO::count(const PrepaymentQuery::Wrapper& query)
{
    SqlParams params;
    std::string condition = queryConditionBuilder(query, params);
    std::string sql = "SELECT COUNT(1) FROM fin_payment_rec_detail" + condition;
    return sqlSession->executeQueryNumerical(sql, params);
}

// Page query
std::list<PrepaymentDO> PrepaymentDAO::selectWithPage(const PrepaymentQuery::Wrapper& query)
{
    SqlParams params;
    std::string condition = queryConditionBuilder(query, params);

    // Get pagination params
    auto pageIndex = query->pageIndex.getValue(1);
    auto pageSize = query->pageSize.getValue(10);

    // Order by transaction time descending (newest first)
    std::string sql = "SELECT * FROM fin_payment_rec_detail" + condition
        + " ORDER BY pay_trans_date DESC LIMIT "
        + std::to_string(((pageIndex - 1) * pageSize))
        + "," + std::to_string(pageSize);

    PrepaymentMapper mapper;
    return sqlSession->executeQuery<PrepaymentDO, PrepaymentMapper>(sql, mapper, params);
}

// Insert a new prepayment record
uint64_t PrepaymentDAO::addPrepayment(const PrepaymentDO& prepayment)
{
    return insertAutoPk(prepayment);
}

// Get the latest after_balance as before_balance for a new transaction
double PrepaymentDAO::getLatestBalance(const std::string& accountCode)
{
    SqlParams params;
    params.push_back(SqlParam("s", std::make_shared<std::string>(accountCode)));
    std::string sql =
        "SELECT * FROM fin_payment_rec_detail "
        "WHERE account_code = ? AND target_enum = 'PREPAYMENT' "
        "ORDER BY pay_trans_date DESC, id DESC LIMIT 1";
    PrepaymentMapper mapper;
    auto result = sqlSession->executeQueryOne<PrepaymentDO, PrepaymentMapper>(sql, mapper, params);
    return result.getAfterBalance();
}

// Get account_id by account_code (inpatient number)
std::string PrepaymentDAO::getAccountIdByCode(const std::string& accountCode)
{
    SqlParams params;
    params.push_back(SqlParam("s", std::make_shared<std::string>(accountCode)));
    std::string sql =
        "SELECT * FROM fin_payment_rec_detail "
        "WHERE account_code = ? AND target_enum = 'PREPAYMENT' "
        "ORDER BY pay_trans_date DESC LIMIT 1";
    PrepaymentMapper mapper;
    auto result = sqlSession->executeQueryOne<PrepaymentDO, PrepaymentMapper>(sql, mapper, params);
    std::string accountId = result.getAccountId();
    if (accountId.empty())
    {
        // Fallback: use account_code as account_id if no record exists
        return accountCode;
    }
    return accountId;
}
