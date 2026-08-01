#include "stdafx.h"
#include "PrepaymentDAO.h"
#include "PrepaymentMapper.h"

#include "NacosClient.h"

// 构建带动态条件的WHERE子句
inline std::string PrepaymentDAO::queryConditionBuilder(
    const PrepaymentQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream condition;
    condition << " WHERE target_enum = 'PREPAYMENT'";

    if (query->inpatientNo)
    {
        condition << " AND account_code = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->inpatientNo->c_str());
    }

    return condition.str();
}

// 统计预交金记录数
uint64_t PrepaymentDAO::count(const PrepaymentQuery::Wrapper& query)
{
    SqlParams params;
    std::string condition = queryConditionBuilder(query, params);
    std::string sql = "SELECT COUNT(1) FROM fin_payment_rec_detail" + condition;
    return sqlSession->executeQueryNumerical(sql, params);
}

// 分页查询
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

// 获取最新账户快照用于余额链接
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

    // 检查是否查到记录（默认构造DO的idPtr为空）
    auto idPtr = result.getIdPtr();
    if (idPtr)
    {
        snap.accountId = result.getAccountId();
        snap.balance = result.getAfterBalance();
        snap.predecessorId = result.getId();
    }

    return snap;
}

// 使用原生SQL插入预交金记录（收或退）
void PrepaymentDAO::insertPrepayment(
    const std::string& id, const std::string& nowStr,
    const std::string& tenantId, const std::string& operatorName,
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
    SQLPARAMS_PUSH(params, "s", std::string, id);    // 对账ID = 支付记录ID
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

    // 流水链接：首条为NULL，否则指向上一条ID
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
        << " VALUES (?,?,?,?,?,?, 0,0, ?,?, ?,?, ?,?,?,?, ?,?,?, ?,?,?)"
        << (predecessorId.empty() ? "" : ",?")
        << ")";

    sqlSession->executeUpdate(sql.str(), params);
}
