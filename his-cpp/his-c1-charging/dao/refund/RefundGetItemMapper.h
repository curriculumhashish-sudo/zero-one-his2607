// ============================================================================
// RefundGetItemMapper.h — 退费项目 — 结果集映射器
// ============================================================================
// Mapper 的工作流程：
//   1. DAO 执行 SQL 产生 ResultSet（N 行）
//   2. DAO 调用 sqlSession->executeQuery<RefundGetItemDO>(sql, RefundGetItemMapper(), params)
//   3. 框架对 ResultSet 每行调用 mapper(resultSet) → 产生一个 RefundGetItemDO
//   4. 所有 DO 收集到 std::list<RefundGetItemDO> 返回给 DAO
// ============================================================================

#pragma once
#include "Mapper.h"
#include "../../domain/do/refund/RefundGetItemDO.h"

// 值类型 Mapper — 用于列表查询，返回 std::list<RefundGetItemDO>
class RefundGetItemMapper : public Mapper<RefundGetItemDO>
{
public:
    RefundGetItemDO mapper(ResultSet* resultSet) const override
    {
        RefundGetItemDO data;

        // resultSet->getString("列名") 从当前行读取指定列的值
        // 列名必须与 SQL 中 SELECT ... AS xxx 完全一致（区分大小写）
        // 就诊 + 支付组层级
        data.setEncounterId(resultSet->getString("encounter_id"));
        data.setPaymentId(resultSet->getString("payment_id"));
        data.setPaymentBusNo(resultSet->getString("payment_bus_no"));
        data.setPaymentStatusEnum(resultSet->getString("payment_status_enum"));
        data.setPaymentStatusEnumText(resultSet->getString("payment_status_enum_text"));
        data.setEntererUserId(resultSet->getString("enterer_user_id"));
        data.setCollectorName(resultSet->getString("collector_name"));
        data.setHalfRefundOnPayment(resultSet->getString("half_refund_on_payment"));
        data.setRefundableAmount(resultSet->getString("refundable_amount"));
        data.setOriginalPayChannelCodes(resultSet->getString("original_pay_channel_codes"));
        // 收费项目行层级
        data.setChargeItemId(resultSet->getString("charge_item_id"));
        data.setChargeItemStatusEnum(resultSet->getString("charge_item_status_enum"));
        data.setChargeItemStatusEnumText(resultSet->getString("charge_item_status_enum_text"));
        data.setItemDisplayName(resultSet->getString("item_display_name"));
        data.setLineTotalAmount(resultSet->getString("line_total_amount"));
        return data;
    }
};

// 指针类型 Mapper — 用于需要 shared_ptr 的场景
class PtrRefundGetItemMapper : public Mapper<PtrRefundGetItemDO>
{
public:
    PtrRefundGetItemDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<RefundGetItemDO>();
        // 就诊 + 支付组层级
        data->setEncounterId(resultSet->getString("encounter_id"));
        data->setPaymentId(resultSet->getString("payment_id"));
        data->setPaymentBusNo(resultSet->getString("payment_bus_no"));
        data->setPaymentStatusEnum(resultSet->getString("payment_status_enum"));
        data->setPaymentStatusEnumText(resultSet->getString("payment_status_enum_text"));
        data->setEntererUserId(resultSet->getString("enterer_user_id"));
        data->setCollectorName(resultSet->getString("collector_name"));
        data->setHalfRefundOnPayment(resultSet->getString("half_refund_on_payment"));
        data->setRefundableAmount(resultSet->getString("refundable_amount"));
        data->setOriginalPayChannelCodes(resultSet->getString("original_pay_channel_codes"));
        // 收费项目行层级
        data->setChargeItemId(resultSet->getString("charge_item_id"));
        data->setChargeItemStatusEnum(resultSet->getString("charge_item_status_enum"));
        data->setChargeItemStatusEnumText(resultSet->getString("charge_item_status_enum_text"));
        data->setItemDisplayName(resultSet->getString("item_display_name"));
        data->setLineTotalAmount(resultSet->getString("line_total_amount"));
        return data;
    }
};
