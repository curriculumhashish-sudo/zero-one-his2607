// ============================================================================
// PrepaymentMapper.h — 预交金收退 — 结果集映射器
// ============================================================================
// 将 SQL 查询结果集 ResultSet 映射为 PrepaymentDO 对象
// ============================================================================

#pragma once
#include "Mapper.h"
#include "../../domain/do/pay_manage/PrepaymentDO.h"

class PrepaymentMapper : public Mapper<PrepaymentDO>
{
public:
    PrepaymentDO mapper(ResultSet* resultSet) const override
    {
        PrepaymentDO data;
        // 系统字段
        data.setId(resultSet->getString("id"));
        data.setTenantId(resultSet->getString("tenant_id"));
        data.setCreatedAt(resultSet->getString("created_at"));
        data.setUpdatedAt(resultSet->getString("updated_at"));
        data.setCreatedBy(resultSet->getString("created_by"));
        data.setUpdatedBy(resultSet->getString("updated_by"));
        data.setIsDeleted(resultSet->getInt("is_deleted"));
        data.setDeletedAt(resultSet->getString("deleted_at"));
        data.setDeletedBy(resultSet->getString("deleted_by"));
        data.setVersion(resultSet->getInt("version"));

        // 业务字段
        data.setReconciliationId(resultSet->getString("reconciliation_id"));
        data.setPredecessorId(resultSet->getString("predecessor_id"));
        data.setTargetEnum(resultSet->getString("target_enum"));
        data.setAccountId(resultSet->getString("account_id"));
        data.setAccountCode(resultSet->getString("account_code"));
        data.setPayEnum(resultSet->getString("pay_enum"));
        data.setPayLevelEnum(resultSet->getString("pay_level_enum"));
        data.setAmount(resultSet->getDouble("amount"));
        data.setReturnAmount(resultSet->getDouble("return_amount"));
        data.setChargeAmount(resultSet->getDouble("charge_amount"));
        data.setPayTransNo(resultSet->getString("pay_trans_no"));
        data.setPayTransText(resultSet->getString("pay_trans_text"));
        data.setPayTransDate(resultSet->getString("pay_trans_date"));
        data.setBeforeBalance(resultSet->getDouble("before_balance"));
        data.setAfterBalance(resultSet->getDouble("after_balance"));
        data.setResultEnum(resultSet->getString("result_enum"));
        return data;
    }
};
