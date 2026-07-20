// ============================================================================
// GetPatientDAO.h — 退费患者队列 · 数据访问对象
// ============================================================================
// 继承 BaseDAO，获得 sqlSession（数据库会话）、executeQuery 等方法
// 本 DAO 按规范只提供查询方法（count + selectWithPage），
// 不使用父类的 insert/update/delete（因为 DO 对应多表 JOIN，非单表）
// ============================================================================

#pragma once
#include "BaseDAO.h"
#include "../../domain/do/refund/GetPatientDO.h"
#include "../../domain/query/refund/GetPatientQuery.h"

class GetPatientDAO : public BaseDAO
{
private:
    // ------------------------------------------------------------------
    // queryConditionBuilder — 动态构建 WHERE 子句（私有辅助方法）
    // ------------------------------------------------------------------
    // 根据前端传来的 keyword 和 queueTabCode 动态拼接 SQL 条件
    // 使用 SqlParams 参数化查询，防止 SQL 注入
    // 返回值：形如 " WHERE 1=1 AND (...) AND (...)" 的 SQL 片段
    // ------------------------------------------------------------------
    inline std::string queryConditionBuilder(
        const GetPatientQuery::Wrapper& query, SqlParams& params);

public:
    // 查询符合条件的记录总数（用于前端分页组件显示总条数）
    uint64_t count(const GetPatientQuery::Wrapper& query);

    // 分页查询记录列表（含排序 + LIMIT 分页）
    std::list<GetPatientDO> selectWithPage(const GetPatientQuery::Wrapper& query);
};
