#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _REFUND_GET_ITEM_DTO_
#define _REFUND_GET_ITEM_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// ==================== �շ���Ŀ�� DTO ====================
// ��Ӧ JSON: paymentGroups[].refundingLines[] �� preservedLines[] ��ÿ��Ԫ��
// ʾ��: {"chargeItemId":"...","paymentId":"...","chargeItemStatusEnum":"REFUNDING",
//        "chargeItemStatusEnumText":"�˿���","itemDisplayName":"����","lineTotalAmount":0.85}
class RefundChargeItemLineDTO : public oatpp::DTO
{
    DTO_INIT(RefundChargeItemLineDTO, DTO);

    DTO_FIELD(String, chargeItemId);
    DTO_FIELD_INFO(chargeItemId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.chargeItemId");
    };
    DTO_FIELD(String, paymentId);
    DTO_FIELD_INFO(paymentId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.paymentId");
    };
    DTO_FIELD(String, chargeItemStatusEnum);
    DTO_FIELD_INFO(chargeItemStatusEnum) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.chargeItemStatusEnum");
    };
    DTO_FIELD(String, chargeItemStatusEnumText);
    DTO_FIELD_INFO(chargeItemStatusEnumText) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.chargeItemStatusEnumText");
    };
    DTO_FIELD(String, itemName);
    DTO_FIELD_INFO(itemName) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.itemName");
    };
    DTO_FIELD(Float64, lineTotalAmount);
    DTO_FIELD_INFO(lineTotalAmount) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.lineTotalAmount");
    };
};

// ==================== ֧���� DTO ====================
// ��Ӧ JSON: paymentGroups[] ��ÿ��Ԫ��
// ʾ��: {"paymentId":"...","paymentBusNo":"PMR-2026-07-190015",
//        "paymentStatusEnum":"SUCCESS","paymentStatusEnumText":"�ɹ����տ�����ɣ�",
//        "entererUserId":"...","collectorName":"admin","halfRefundOnPayment":false,
//        "refundingLines":[...],"preservedLines":[],
//        "refundableAmount":24.91,"originalPayChannelCodes":["CASH"]}
class RefundPaymentGroupDTO : public oatpp::DTO
{
    DTO_INIT(RefundPaymentGroupDTO, DTO);

    DTO_FIELD(String, paymentRecordId);
    DTO_FIELD_INFO(paymentRecordId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.paymentRecordId");
    };
    DTO_FIELD(String, paymentNo);
    DTO_FIELD_INFO(paymentNo) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.paymentNo");
    };
    DTO_FIELD(String, paymentStatusEnum);
    DTO_FIELD_INFO(paymentStatusEnum) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.paymentStatusEnum");
    };
    DTO_FIELD(String, paymentStatusEnumText);
    DTO_FIELD_INFO(paymentStatusEnumText) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.paymentStatusEnumText");
    };
    DTO_FIELD(String, entererUserId);
    DTO_FIELD_INFO(entererUserId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.entererUserId");
    };
    DTO_FIELD(String, collectorName);
    DTO_FIELD_INFO(collectorName) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.collectorName");
    };
    DTO_FIELD(Boolean, halfRefundOnPayment);
    DTO_FIELD_INFO(halfRefundOnPayment) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.halfRefundOnPayment");
    };
    // �˷����б���Ƕ�����飩
    DTO_FIELD(oatpp::List<RefundChargeItemLineDTO::Wrapper>, refundingLines);
    DTO_FIELD_INFO(refundingLines) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.refundingLines");
    };
    // �������б���Ƕ�����飬�ṹͬ refundingLines��
    DTO_FIELD(oatpp::List<RefundChargeItemLineDTO::Wrapper>, preservedLines);
    DTO_FIELD_INFO(preservedLines) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.preservedLines");
    };
    DTO_FIELD(Float64, refundableAmount);
    DTO_FIELD_INFO(refundableAmount) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.refundableAmount");
    };
    // ԭʼ֧�����������б����ַ������飬�� ["CASH"]��
    DTO_FIELD(oatpp::List<String>, originalPayChannelCodes);
    DTO_FIELD_INFO(originalPayChannelCodes) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.originalPayChannelCodes");
    };
};

// ==================== �˷���Ŀ������Ӧ DTO ====================
// ��Ӧ JSON: data �ֶ�
// ʾ��: {"encounterId":"...","paymentGroups":[{...},...]}
class RefundGetItemDTO : public oatpp::DTO
{
    DTO_INIT(RefundGetItemDTO, DTO);

    DTO_FIELD(String, encounterId);
    DTO_FIELD_INFO(encounterId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.encounterId");
    };
    // ֧�����б���Ƕ�����飩
    DTO_FIELD(oatpp::List<RefundPaymentGroupDTO::Wrapper>, paymentGroups);
    DTO_FIELD_INFO(paymentGroups) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.paymentGroups");
    };
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_REFUND_GET_ITEM_DTO_
