// ============================================================================
// RefundGetPatientService.h — 退费患者队列 · 业务服务
// ============================================================================
// Service 层的职责：
//   1. 调用 DAO 获取原始数据
//   2. 将 DO 批量转换为 DTO（使用 ZO_STAR_DOMAIN_DO_TO_DTO 宏）
//   3. 组装分页对象（PageDTO），设置 pageIndex/pageSize/total/pages
//   4. 返回给 Controller
// ============================================================================

#pragma once
#include "domain/vo/refund/RefundGetPatientVO.h"
#include "domain/query/refund/RefundGetPatientQuery.h"
#include "domain/dto/refund/RefundGetPatientDTO.h"                                                 

class RefundGetPatientService
{
public:
    // listAll — 分页查询退费患者队列
    // 参数：前端传来的查询条件（含 keyword、queueTabCode、pageNo、pageSize）
    // 返回：RefundGetPatientPageDTO，包含 content[]、totalCount、pageNo、pageSize
    RefundGetPatientPageDTO::Wrapper listAll(const RefundGetPatientQuery::Wrapper& query);
};
