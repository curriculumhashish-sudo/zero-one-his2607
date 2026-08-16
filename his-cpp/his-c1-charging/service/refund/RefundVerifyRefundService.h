// ============================================================================
// RefundVerifyRefundService.h — 退费校验 — 业务逻辑层
// ============================================================================
// 校验流程:
//   1. 查询 adm_charge_item — 检查收费项目是否存在且状态可退
//   2. 查询 fin_payment_reconciliation — 检查支付流水状态
//   3. 返回 allowed + rejectReasonCode
// ============================================================================

#pragma once
#include "domain/vo/refund/RefundVerifyRefundVO.h"
#include "domain/query/refund/RefundVerifyRefundQuery.h"
#include "domain/dto/refund/RefundVerifyRefundDTO.h"

class RefundVerifyRefundService
{
public:
    // 校验退费是否可执行
    RefundVerifyRefundDTO::Wrapper verify(const RefundVerifyRefundQuery::Wrapper& query);
};
