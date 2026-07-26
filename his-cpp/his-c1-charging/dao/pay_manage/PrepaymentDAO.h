// ============================================================================
// PrepaymentDAO.h — 预交金收退 — 数据访问对象
// ============================================================================
// 继承 BaseDAO，提供预交金流水的查询操作
// 增删改操作：直接使用 BaseDAO 的 insert / update / deleteById 方法
// ============================================================================

#pragma once
#include "BaseDAO.h"
#include "../../domain/do/pay_manage/PrepaymentDO.h"
#include "../../domain/query/pay_manage/PrepaymentQuery.h"

class PrepaymentDAO : public BaseDAO
{
private:
    // 根据查询条件动态构建 WHERE 子句
    inline std::string queryConditionBuilder(
        const PrepaymentQuery::Wrapper& query, SqlParams& params);

public:
    // 统计预交金流水记录数（分页用）
    uint64_t count(const PrepaymentQuery::Wrapper& query);

    // 分页查询预交金流水列表
    std::list<PrepaymentDO> selectWithPage(const PrepaymentQuery::Wrapper& query);
};
