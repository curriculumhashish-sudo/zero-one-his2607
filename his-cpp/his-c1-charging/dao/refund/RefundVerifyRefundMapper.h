// ============================================================================
// RefundVerifyRefundMapper.h — 退费校验 — 结果集映射器
// ============================================================================
// 注: 退费校验为业务逻辑校验，不走 DB 映射，本 Mapper 保留以备后续扩展

#pragma once
#include "Mapper.h"
#include "../../domain/do/refund/RefundVerifyRefundDO.h"

class RefundVerifyRefundMapper : public Mapper<RefundVerifyRefundDO>
{
public:
    RefundVerifyRefundDO mapper(ResultSet* resultSet) const override
    {
        RefundVerifyRefundDO data;
        data.setAllowed(resultSet->getString("allowed"));
        data.setRejectReasonCode(resultSet->getString("reject_reason_code"));
        return data;
    }
};
