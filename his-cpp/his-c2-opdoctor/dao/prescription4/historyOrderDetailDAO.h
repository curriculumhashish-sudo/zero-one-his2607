#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#ifndef _HISTORY_ORDER_DETAIL_DAO_
#define _HISTORY_ORDER_DETAIL_DAO_

#include "BaseDAO.h"
#include "../../domain/do/prescription4/historyOrderDetailDO.h"
#include "../../domain/query/prescription4/historyOrderDetailQuery.h"

/**
 * 历史医嘱明细数据访问层
 * 查询某次就诊的所有医嘱（药品、服务、耗材）合并列表
 */
class HistoryOrderDetailDAO : public BaseDAO
{
private:
    /**
     * 构建完整查询 SQL（包含 UNION ALL 及外部过滤条件）
     * @param query 查询条件
     * @param params 输出参数
     * @return 完整 SQL 字符串（不含 ORDER BY 和 LIMIT）
     */
    std::string buildFullQuerySql(const HistoryOrderDetailQuery::Wrapper& query, SqlParams& params);

public:
    // ========== 统计 ==========
    /**
     * 统计符合条件的医嘱明细总数
     */
    uint64_t count(const HistoryOrderDetailQuery::Wrapper& query);

    // ========== 查询 ==========
    /**
     * 分页查询医嘱明细列表（合并三种医嘱类型）
     */
    std::list<PtrHistoryOrderDetailDO> selectWithPage(const HistoryOrderDetailQuery::Wrapper& query);
};

#endif // !_HISTORY_ORDER_DETAIL_DAO_