#pragma once

#ifndef _REFUND_GET_ITEM_DO_H_
#define _REFUND_GET_ITEM_DO_H_

#include "../DoInclude.h"

/**
 * 退费患者队列数据对象
 * 对应多表 JOIN 查询结果，不是单表映射
 */
class RefundGetItemDO : public BaseDO
{
    // ==================== 就诊 + 支付组层级 ====================
    // 就诊ID
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    // 支付记录ID
    MYSQL_SYNTHESIZE(string, paymentId, PaymentId);
    // 支付流水号（如 PMR-2026-07-190015）
    MYSQL_SYNTHESIZE(string, paymentBusNo, PaymentBusNo);
    // 支付状态枚举码（SUCCESS / REFUNDING / ...）
    MYSQL_SYNTHESIZE(string, paymentStatusEnum, PaymentStatusEnum);
    // 支付状态枚举显示文本（如"成功（收款已完成）"）
    MYSQL_SYNTHESIZE(string, paymentStatusEnumText, PaymentStatusEnumText);
    // 录入人用户ID
    MYSQL_SYNTHESIZE(string, entererUserId, EntererUserId);
    // 收款人姓名
    MYSQL_SYNTHESIZE(string, collectorName, CollectorName);
    // 是否为支付级别半退
    MYSQL_SYNTHESIZE(string, halfRefundOnPayment, HalfRefundOnPayment);
    // 可退总金额
    MYSQL_SYNTHESIZE(string, refundableAmount, RefundableAmount);
    // 原始支付渠道编码（逗号分隔，如 "CASH"）
    MYSQL_SYNTHESIZE(string, originalPayChannelCodes, OriginalPayChannelCodes);

    // ==================== 收费项目行层级 ====================
    // 收费项目ID
    MYSQL_SYNTHESIZE(string, chargeItemId, ChargeItemId);
    // 收费项目状态枚举码（REFUNDING-退款中 / PRESERVED-保留）
    MYSQL_SYNTHESIZE(string, chargeItemStatusEnum, ChargeItemStatusEnum);
    // 收费项目状态枚举显示文本（如"退款中"）
    MYSQL_SYNTHESIZE(string, chargeItemStatusEnumText, ChargeItemStatusEnumText);
    // 项目显示名称（如"黄芪"、"静脉注射"）
    MYSQL_SYNTHESIZE(string, itemDisplayName, ItemDisplayName);
    // 行合计金额
    MYSQL_SYNTHESIZE(string, lineTotalAmount, LineTotalAmount);

public:
    // 多表 JOIN 视图，非单表映射，表名为空字符串
    RefundGetItemDO() : BaseDO("")
    {
        // 就诊 + 支付组层级
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD_PK("payment_id", "s", paymentId);
        MYSQL_ADD_FIELD("payment_bus_no", "s", paymentBusNo);
        MYSQL_ADD_FIELD("payment_status_enum", "s", paymentStatusEnum);
        MYSQL_ADD_FIELD("payment_status_enum_text", "s", paymentStatusEnumText);
        MYSQL_ADD_FIELD("enterer_user_id", "s", entererUserId);
        MYSQL_ADD_FIELD("collector_name", "s", collectorName);
        MYSQL_ADD_FIELD("half_refund_on_payment", "s", halfRefundOnPayment);
        MYSQL_ADD_FIELD("refundable_amount", "s", refundableAmount);
        MYSQL_ADD_FIELD("original_pay_channel_codes", "s", originalPayChannelCodes);
        // 收费项目行层级
        MYSQL_ADD_FIELD("charge_item_id", "s", chargeItemId);
        MYSQL_ADD_FIELD("charge_item_status_enum", "s", chargeItemStatusEnum);
        MYSQL_ADD_FIELD("charge_item_status_enum_text", "s", chargeItemStatusEnumText);
        MYSQL_ADD_FIELD("item_display_name", "s", itemDisplayName);
        MYSQL_ADD_FIELD("line_total_amount", "s", lineTotalAmount);
    }
};

// 定义智能指针别名
typedef std::shared_ptr<RefundGetItemDO> PtrRefundGetItemDO;



#endif // !_REFUND_GET_PATIENT_DO_H_
