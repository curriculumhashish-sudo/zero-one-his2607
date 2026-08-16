#pragma once

#ifndef _GET_PATIENT_QUEUE_SERVICE_H_
#define _GET_PATIENT_QUEUE_SERVICE_H_

#include "domain/vo/charge/GetPatientQueueJsonVO.h"
#include "domain/query/charge/GetPatientQueueQuery.h"
#include "domain/dto/charge/GetPatientQueueDTO.h"

/**
 * 患者队列查询服务（接口1）
 *
 * 查询流程：DAO查数据 -> DO转DTO -> 内存计算补充字段 -> 分页组装返回
 */
class GetPatientQueueService
{
private:
    // 性别编码转展示文本（MALE -> 男，FEMALE -> 女）
    static std::string translateGender(const std::string& code);

    // 由出生日期计算年龄展示文本，如 "1990-05-15" 计算为 "35岁"
    static std::string computeAgeText(const std::string& birthDate);

    // 收费状态编码转展示文本（DRAFT -> 待收费 等）
    static std::string translateChargeStatus(const std::string& code);

    // 根据所有收费项的状态码列表，聚合计算出汇总状态码（planned/settled/mixed）
    static std::string computeRollupStatusCode(
        const std::list<std::string>& statusCodes);

    // 汇总状态码转展示文本（planned -> 计划中 等）
    static std::string translateRollupStatus(const std::string& code);

public:
    /**
     * 分页查询患者队列
     * @param query 前端传入的查询条件，包含 keyword 关键字、startDate/endDate 时间范围、
     *              queueTabCode 页签过滤、pageIndex 页码、pageSize 每页条数
     * @return 分页结果，内含 content 数据列表、total 总记录数、pages 总页数等
     */
    GetPatientQueuePageDTO::Wrapper listAll(const GetPatientQueueQuery::Wrapper& query);
};

#endif // !_GET_PATIENT_QUEUE_SERVICE_H_
