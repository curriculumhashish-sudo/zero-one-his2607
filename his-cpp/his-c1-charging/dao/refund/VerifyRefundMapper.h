// ============================================================================
// VerifyRefundMapper.h — 退费校验 — 结果集映射器
// ============================================================================
// 注: 退费校验为业务逻辑校验，不走 DB 映射，本 Mapper 保留以备后续扩展

#pragma once
#include "Mapper.h"
#include "../../domain/do/refund/VerifyRefundDO.h"

class VerifyRefundMapper : public Mapper<VerifyRefundDO>
{
public:
    VerifyRefundDO mapper(ResultSet* resultSet) const override
    {
        VerifyRefundDO data;
        data.setAllowed(resultSet->getString("allowed"));
        data.setRejectReasonCode(resultSet->getString("reject_reason_code"));
        return data;
    }
};
