// ============================================================================
// VerifyRefundDAO.h — 退费校验 — 数据访问对象
// ============================================================================
// 校验逻辑通过 Service 层直接查询 adm_charge_item / fin_payment_reconciliation
// 本 DAO 保留以备后续需要 DB 级校验时扩展

#pragma once
#include "BaseDAO.h"
#include "../../domain/do/refund/VerifyRefundDO.h"
#include "../../domain/query/refund/VerifyRefundQuery.h"

class VerifyRefundDAO : public BaseDAO
{
public:
    // 查询可退费状态（预留，实际校验逻辑在 Service 层）
    std::list<VerifyRefundDO> selectByEncounterId(const VerifyRefundQuery::Wrapper& query);
};
