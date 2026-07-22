// ============================================================================
// RefundExecuteRefundMapper.h — 执行退费 — 结果集映射器
// ============================================================================
// 注: 执行退费为事务性写操作，不走 DB 映射查询，本 Mapper 保留以备后续扩展

#pragma once
#include "Mapper.h"
#include "../../domain/do/refund/RefundExecuteRefundDO.h"

class RefundExecuteRefundMapper : public Mapper<RefundExecuteRefundDO>
{
public:
    RefundExecuteRefundDO mapper(ResultSet* resultSet) const override
    {
        RefundExecuteRefundDO data;
        data.setSuccess(resultSet->getString("success"));
        data.setReversePaymentId(resultSet->getString("reverse_payment_id"));
        data.setOriginalPaymentId(resultSet->getString("original_payment_id"));
        data.setHalfRefundExecuted(resultSet->getString("half_refund_executed"));
        data.setInsuranceRevoked(resultSet->getString("insurance_revoked"));
        return data;
    }
};
