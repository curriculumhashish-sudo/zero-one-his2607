#pragma once

#ifndef _REFUND_GET_ITEM_DO_H_
#define _REFUND_GET_ITEM_DO_H_

#include "../DoInclude.h"

/**
 * �˷ѻ��߶������ݶ���
 * ��Ӧ��� JOIN ��ѯ��������ǵ���ӳ��
 */
class RefundGetItemDO : public BaseDO
{
    // ==================== ���� + ֧����㼶 ====================
    // ����ID
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    // ֧����¼ID
    MYSQL_SYNTHESIZE(string, paymentRecordId, PaymentRecordId);
    // ֧����ˮ�ţ��� PMR-2026-07-190015��
    MYSQL_SYNTHESIZE(string, paymentNo, PaymentNo);
    // ֧��״̬ö���루SUCCESS / REFUNDING / ...��
    MYSQL_SYNTHESIZE(string, paymentStatusEnum, PaymentStatusEnum);
    // ֧��״̬ö����ʾ�ı�����"�ɹ����տ�����ɣ�"��
    MYSQL_SYNTHESIZE(string, paymentStatusEnumText, PaymentStatusEnumText);
    // ¼�����û�ID
    MYSQL_SYNTHESIZE(string, entererUserId, EntererUserId);
    // �տ�������
    MYSQL_SYNTHESIZE(string, collectorName, CollectorName);
    // �Ƿ�Ϊ֧���������
    MYSQL_SYNTHESIZE(string, halfRefundOnPayment, HalfRefundOnPayment);
    // �����ܽ��
    MYSQL_SYNTHESIZE(string, refundableAmount, RefundableAmount);
    // ԭʼ֧���������루���ŷָ����� "CASH"��
    MYSQL_SYNTHESIZE(string, originalPayChannelCodes, OriginalPayChannelCodes);

    // ==================== �շ���Ŀ�в㼶 ====================
    // �շ���ĿID
    MYSQL_SYNTHESIZE(string, chargeItemId, ChargeItemId);
    // �շ���Ŀ״̬ö���루REFUNDING-�˿��� / PRESERVED-������
    MYSQL_SYNTHESIZE(string, chargeItemStatusEnum, ChargeItemStatusEnum);
    // �շ���Ŀ״̬ö����ʾ�ı�����"�˿���"��
    MYSQL_SYNTHESIZE(string, chargeItemStatusEnumText, ChargeItemStatusEnumText);
    // ��Ŀ��ʾ���ƣ���"����"��"����ע��"��
    MYSQL_SYNTHESIZE(string, itemName, ItemName);
    // �кϼƽ��
    MYSQL_SYNTHESIZE(string, lineTotalAmount, LineTotalAmount);

public:
    // ��� JOIN ��ͼ���ǵ���ӳ�䣬����Ϊ���ַ���
    RefundGetItemDO() : BaseDO("")
    {
        // ���� + ֧����㼶
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD_PK("payment_record_id", "s", paymentRecordId);
        MYSQL_ADD_FIELD("payment_no", "s", paymentNo);
        MYSQL_ADD_FIELD("payment_status_enum", "s", paymentStatusEnum);
        MYSQL_ADD_FIELD("payment_status_enum_text", "s", paymentStatusEnumText);
        MYSQL_ADD_FIELD("enterer_user_id", "s", entererUserId);
        MYSQL_ADD_FIELD("collector_name", "s", collectorName);
        MYSQL_ADD_FIELD("half_refund_on_payment", "s", halfRefundOnPayment);
        MYSQL_ADD_FIELD("refundable_amount", "s", refundableAmount);
        MYSQL_ADD_FIELD("original_pay_channel_codes", "s", originalPayChannelCodes);
        // �շ���Ŀ�в㼶
        MYSQL_ADD_FIELD("charge_item_id", "s", chargeItemId);
        MYSQL_ADD_FIELD("charge_item_status_enum", "s", chargeItemStatusEnum);
        MYSQL_ADD_FIELD("charge_item_status_enum_text", "s", chargeItemStatusEnumText);
        MYSQL_ADD_FIELD("item_name", "s", itemName);
        MYSQL_ADD_FIELD("line_total_amount", "s", lineTotalAmount);
    }
};

// ��������ָ�����
typedef std::shared_ptr<RefundGetItemDO> PtrRefundGetItemDO;



#endif // !_REFUND_GET_PATIENT_DO_H_
