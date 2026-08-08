// ============================================================================
// RefundGetItemMapper.h �� �˷���Ŀ �� �����ӳ����
// ============================================================================
// Mapper �Ĺ������̣�
//   1. DAO ִ�� SQL ���� ResultSet��N �У�
//   2. DAO ���� sqlSession->executeQuery<RefundGetItemDO>(sql, RefundGetItemMapper(), params)
//   3. ��ܶ� ResultSet ÿ�е��� mapper(resultSet) �� ����һ�� RefundGetItemDO
//   4. ���� DO �ռ��� std::list<RefundGetItemDO> ���ظ� DAO
// ============================================================================

#pragma once
#include "Mapper.h"
#include "../../domain/do/refund/RefundGetItemDO.h"

// ֵ���� Mapper �� �����б���ѯ������ std::list<RefundGetItemDO>
class RefundGetItemMapper : public Mapper<RefundGetItemDO>
{
public:
    RefundGetItemDO mapper(ResultSet* resultSet) const override
    {
        RefundGetItemDO data;

        // resultSet->getString("����") �ӵ�ǰ�ж�ȡָ���е�ֵ
        // ���������� SQL �� SELECT ... AS xxx ��ȫһ�£����ִ�Сд��
        // ���� + ֧����㼶
        data.setEncounterId(resultSet->getString("encounter_id"));
        data.setPaymentRecordId(resultSet->getString("payment_record_id"));
        data.setPaymentNo(resultSet->getString("payment_no"));
        data.setPaymentStatusEnum(resultSet->getString("payment_status_enum"));
        data.setPaymentStatusEnumText(resultSet->getString("payment_status_enum_text"));
        data.setEntererUserId(resultSet->getString("enterer_user_id"));
        data.setCollectorName(resultSet->getString("collector_name"));
        data.setHalfRefundOnPayment(resultSet->getString("half_refund_on_payment"));
        data.setRefundableAmount(resultSet->getString("refundable_amount"));
        data.setOriginalPayChannelCodes(resultSet->getString("original_pay_channel_codes"));
        // �շ���Ŀ�в㼶
        data.setChargeItemId(resultSet->getString("charge_item_id"));
        data.setChargeItemStatusEnum(resultSet->getString("charge_item_status_enum"));
        data.setChargeItemStatusEnumText(resultSet->getString("charge_item_status_enum_text"));
        data.setItemName(resultSet->getString("item_name"));
        data.setLineTotalAmount(resultSet->getString("line_total_amount"));
        return data;
    }
};

// ָ������ Mapper �� ������Ҫ shared_ptr �ĳ���
class PtrRefundGetItemMapper : public Mapper<PtrRefundGetItemDO>
{
public:
    PtrRefundGetItemDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<RefundGetItemDO>();
        // ���� + ֧����㼶
        data->setEncounterId(resultSet->getString("encounter_id"));
        data->setPaymentRecordId(resultSet->getString("payment_record_id"));
        data->setPaymentNo(resultSet->getString("payment_no"));
        data->setPaymentStatusEnum(resultSet->getString("payment_status_enum"));
        data->setPaymentStatusEnumText(resultSet->getString("payment_status_enum_text"));
        data->setEntererUserId(resultSet->getString("enterer_user_id"));
        data->setCollectorName(resultSet->getString("collector_name"));
        data->setHalfRefundOnPayment(resultSet->getString("half_refund_on_payment"));
        data->setRefundableAmount(resultSet->getString("refundable_amount"));
        data->setOriginalPayChannelCodes(resultSet->getString("original_pay_channel_codes"));
        // �շ���Ŀ�в㼶
        data->setChargeItemId(resultSet->getString("charge_item_id"));
        data->setChargeItemStatusEnum(resultSet->getString("charge_item_status_enum"));
        data->setChargeItemStatusEnumText(resultSet->getString("charge_item_status_enum_text"));
        data->setItemName(resultSet->getString("item_name"));
        data->setLineTotalAmount(resultSet->getString("line_total_amount"));
        return data;
    }
};
