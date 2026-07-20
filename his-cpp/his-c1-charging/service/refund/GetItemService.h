// ============================================================================
// GetItemService.h — 退费项目 — 业务逻辑层
// ============================================================================
// Service 层职责：
//   1. 调用 DAO 获取扁平行数据
//   2. 将 DO 列表按 paymentId 分组
//   3. 每个支付组内按 chargeItemStatusEnum 区分为 refundingLines / preservedLines
//   4. 组装嵌套 DTO 结构返回给 Controller
// ============================================================================

#pragma once
#include "domain/vo/refund/GetItemVO.h"
#include "domain/query/refund/GetItemQuery.h"
#include "domain/dto/refund/GetItemDTO.h"

class GetItemService
{
public:
    // listAll — 根据就诊ID查询退费项目明细
    // 参数: 前端传入的查询条件（encounterId + detailViewCode）
    // 返回: GetItemDTO 包含 encounterId 和嵌套的 paymentGroups 结构
    //   paymentGroups[].refundingLines — chargeItemStatusEnum = "REFUNDING" 的行
    //   paymentGroups[].preservedLines  — chargeItemStatusEnum = "PRESERVED" 的行
    GetItemDTO::Wrapper listAll(const GetItemQuery::Wrapper& query);
};
