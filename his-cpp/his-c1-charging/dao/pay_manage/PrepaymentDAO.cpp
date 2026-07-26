#include "stdafx.h"
#include "PrepaymentDAO.h"
#include "PrepaymentMapper.h"

// 构建查询条件：根据住院号筛选
inline std::string PrepaymentDAO::queryConditionBuilder(
    const PrepaymentQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream condition;
    condition << " WHERE 1=1";

    // 只查预交金类型的记录
    condition << " AND target_enum = 'PREPAYMENT'";

    // 按住院号筛选（必填）
    if (query->inpatientNo)
    {
        condition << " AND account_code = ?";
        params.push_back(SqlParam("s", std::make_shared<std::string>(
            query->inpatientNo->c_str())));
    }

    return condition.str();
}

// 统计记录数
uint64_t PrepaymentDAO::count(const PrepaymentQuery::Wrapper& query)
{
    return 0;
}

// 分页查询
std::list<PrepaymentDO> PrepaymentDAO::selectWithPage(const PrepaymentQuery::Wrapper& query)
{
    SqlParams params;
    std::string condition = queryConditionBuilder(query, params);

    // 获取分页参数
    auto pageIndex = query->pageIndex.getValue(1);
    auto pageSize = query->pageSize.getValue(10);

    // 按交易时间倒序，最新的在前
    std::string sql = "SELECT * FROM fin_payment_rec_detail" + condition
        + " ORDER BY pay_trans_date DESC LIMIT "
        + std::to_string(((pageIndex - 1) * pageSize))
        + "," + std::to_string(pageSize);

    PrepaymentMapper mapper;
    return sqlSession->executeQuery<PrepaymentDO, PrepaymentMapper>(sql, mapper, params);
}
