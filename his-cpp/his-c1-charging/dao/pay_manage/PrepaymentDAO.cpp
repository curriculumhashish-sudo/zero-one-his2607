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
    if (!sqlSession || !sqlSession->getConnection())
    {
        return 0;
    }
    try
    {
        SqlParams params;
        std::string condition = queryConditionBuilder(query, params);
        std::string sql = "SELECT COUNT(1) FROM fin_payment_rec_detail" + condition;
        return sqlSession->executeQueryNumerical(sql, params);
    }
    catch (...)
    {
        return 0;
    }
}

// Page query
std::list<PrepaymentDO> PrepaymentDAO::selectWithPage(const PrepaymentQuery::Wrapper& query)
{
    if (!sqlSession || !sqlSession->getConnection())
    {
        return std::list<PrepaymentDO>();
    }
    try
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
    catch (...)
    {
        return std::list<PrepaymentDO>();
    }
}

// Insert a new prepayment record
uint64_t PrepaymentDAO::addPrepayment(const PrepaymentDO& prepayment)
{
    if (!sqlSession || !sqlSession->getConnection())
    {
        return 0;
    }
    try
    {
        return insertAutoPk(prepayment);
    }
    catch (const std::exception& e)
    {
        cerr << "addPrepayment exception: " << e.what() << endl;
        return 0;
    }
    catch (...)
    {
        cerr << "addPrepayment unknown exception" << endl;
        return 0;
    }
}

// Insert a prepayment record with explicit PK (UUID)
uint64_t PrepaymentDAO::insertPrepayment(const PrepaymentDO& prepayment)
{
    if (!sqlSession || !sqlSession->getConnection())
    {
        return 0;
    }
    try
    {
        return insert(prepayment);
    }
    catch (const std::exception& e)
    {
        cerr << "insertPrepayment exception: " << e.what() << endl;
        return 0;
    }
    catch (...)
    {
        cerr << "insertPrepayment unknown exception" << endl;
        return 0;
    }
}

// Get account snapshot in a single query
AccountSnapshot PrepaymentDAO::getAccountSnapshot(const std::string& accountCode)
{
    AccountSnapshot snapshot;
    snapshot.accountId = accountCode;  // default: use accountCode as accountId
    snapshot.balance = 0.0;
    snapshot.predecessorId = "";

    // Guard against null session or connection
    if (!sqlSession || !sqlSession->getConnection())
    {
        return snapshot;
    }

    try
    {
        SqlParams params;
        params.push_back(SqlParam("s", std::make_shared<std::string>(accountCode)));
        std::string sql =
            "SELECT * FROM fin_payment_rec_detail "
            "WHERE account_code = ? AND target_enum = 'PREPAYMENT' "
            "ORDER BY pay_trans_date DESC, id DESC LIMIT 1";
        PrepaymentMapper mapper;
        auto result = sqlSession->executeQueryOne<PrepaymentDO, PrepaymentMapper>(sql, mapper, params);

        std::string accountId = result.getAccountId();
        if (!accountId.empty())
        {
            snapshot.accountId = accountId;
        }
        snapshot.balance = result.getAfterBalance();
        snapshot.predecessorId = result.getId();
    }
    catch (const std::exception& e)
    {
        cerr << "getAccountSnapshot exception: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "getAccountSnapshot unknown exception" << endl;
    }

    return snapshot;
}
