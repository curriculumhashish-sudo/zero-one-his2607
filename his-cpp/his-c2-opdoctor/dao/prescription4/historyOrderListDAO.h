#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#ifndef _HISTORY_ORDER_LIST_DAO_
#define _HISTORY_ORDER_LIST_DAO_

#include "BaseDAO.h"
#include "../../domain/do/prescription4/HistoryOrderListDO.h"
#include "../../domain/query/prescription4/HistoryOrderListQuery.h"

/**
 * 历史医嘱就诊记录数据访问层
 * 查询某患者的所有就诊记录，并统计各类医嘱数量
 */
class HistoryOrderListDAO : public BaseDAO
{
private:
    /**
     * 构建分页查询的 WHERE 条件
     * @param query 查询条件对象
     * @param params 输出参数，用于参数化查询
     * @return WHERE 子句字符串
     */
    inline std::string queryConditionBuilder(const HistoryOrderListQuery::Wrapper& query, SqlParams& params);

public:
    // ========== 统计 ==========
    /**
     * 统计符合条件的就诊记录总数（用于分页）
     */
    uint64_t count(const HistoryOrderListQuery::Wrapper& query);

    // ========== 查询 ==========
    /**
     * 分页查询就诊记录列表（含主诊断、医生、医嘱计数）
     * @return 就诊记录 DO 列表（智能指针）
     */
    std::list<PtrHistoryOrderListDO> selectWithPage(const HistoryOrderListQuery::Wrapper& query);
};

#endif // !_HISTORY_ORDER_LIST_DAO_