#pragma once

#ifndef _GET_PATIENT_QUEUE_SERVICE_H_
#define _GET_PATIENT_QUEUE_SERVICE_H_

#include "domain/vo/charge/GetPatientQueueJsonVO.h"
#include "domain/query/charge/GetPatientQueueQuery.h"
#include "domain/dto/charge/GetPatientQueueDTO.h"

/**
 * 患者队列查询服务（接口1）
 * 编排：DAO 查询 → DO→DTO 转换 → 内存计算 → 分页组装
 */
class GetPatientQueueService
{
private:
    // 性别编码转展示文案
    static std::string translateGender(const std::string& code);

    // 出生日期转年龄文案（如 "1990-05-15" → "35岁"）
    static std::string computeAgeText(const std::string& birthDate);

    // 收费状态编码转展示文案
    static std::string translateChargeStatus(const std::string& code);

    // 根据收费项状态列表计算汇总状态码
    static std::string computeRollupStatusCode(
        const std::list<std::string>& statusCodes);

    // 汇总状态码转展示文案
    static std::string translateRollupStatus(const std::string& code);

public:
    /**
     * 分页查询患者队列
     * @param query 前端查询参数（keyword, startDate, endDate, queueTabCode, pageIndex, pageSize）
     * @return 分页 DTO，含 content[], total, pages, pageIndex, pageSize
     */
    GetPatientQueuePageDTO::Wrapper listAll(const GetPatientQueueQuery::Wrapper& query);
};

#endif // !_GET_PATIENT_QUEUE_SERVICE_H_
