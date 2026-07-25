#pragma once

#ifndef _GET_PATIENT_QUEUE_DAO_H_
#define _GET_PATIENT_QUEUE_DAO_H_

#include "BaseDAO.h"
#include "domain/do/charge/GetPatientQueueDO.h"
#include "domain/query/charge/GetPatientQueueQuery.h"

/**
 * 患者队列查询 DAO
 * 提供计数和分页查询，对应主 JOIN + 聚合查询
 */
class GetPatientQueueDAO : public BaseDAO
{
private:
    /**
     * 根据查询参数动态构建 WHERE 子句
     * @param query 前端查询参数（keyword, startDate, endDate, queueTabCode）
     * @param params 输出参数，用于 prepared statement 绑定
     * @return WHERE 子句字符串，以 " WHERE 1=1 ..." 开头
     */
    inline std::string queryConditionBuilder(
        const GetPatientQueueQuery::Wrapper& query, SqlParams& params);

public:
    /**
     * 统计符合查询条件的总记录数
     * @param query 前端查询参数
     * @return 匹配的就诊总数
     */
    uint64_t count(const GetPatientQueueQuery::Wrapper& query);

    /**
     * 分页查询患者队列
     * @param query 前端查询参数（含 pageIndex 和 pageSize）
     * @return 当前页的 DO 列表
     */
    std::list<GetPatientQueueDO> selectWithPage(const GetPatientQueueQuery::Wrapper& query);
};

#endif // !_GET_PATIENT_QUEUE_DAO_H_
