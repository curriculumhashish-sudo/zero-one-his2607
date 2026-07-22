// ============================================================================
// RefundGetItemDAO.h — 退费项目 — 数据访问对象
// ============================================================================
// 继承 BaseDAO，获得 sqlSession（数据库会话）、executeQuery 等方法
// 按 DAO 规范只提供查询方法
// 不使用父类 insert/update/delete，因为 DO 对应 JOIN 视图而非单表
// ============================================================================

#pragma once
#include "BaseDAO.h"
#include "../../domain/do/refund/RefundGetItemDO.h"
#include "../../domain/query/refund/RefundGetItemQuery.h"

class RefundGetItemDAO : public BaseDAO
{
private:
    // ------------------------------------------------------------------
    // queryConditionBuilder — 动态构建 WHERE 子句（私有辅助方法）
    // ------------------------------------------------------------------
    // 根据前端传入的 encounterId 和 detailViewCode 动态拼接 SQL 条件
    // 使用 SqlParams 进行参数化查询，防止 SQL 注入
    // 返回值: " WHERE 1=1 AND (...) AND (...)" 格式的 SQL 片段
    // ------------------------------------------------------------------
    inline std::string queryConditionBuilder(
        const RefundGetItemQuery::Wrapper& query, SqlParams& params);

public:
    // 分页查询退费项目明细列表（扁平行）
    // 参数: 前端传入的查询条件（encounterId + detailViewCode）
    // 返回: 扁平 DO 列表，由 Service 层分组为嵌套结构
    std::list<RefundGetItemDO> selectWithPage(const RefundGetItemQuery::Wrapper& query);

    // 统计满足条件的记录总数
    uint64_t count(const RefundGetItemQuery::Wrapper& query);
};
