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
#ifndef _GET_ITEM_DTO_
#define _GET_ITEM_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// ==================== 收费项目行 DTO ====================
// 对应 JSON: paymentGroups[].refundingLines[] 和 preservedLines[] 的每个元素
// 示例: {"chargeItemId":"...","paymentId":"...","chargeItemStatusEnum":"REFUNDING",
//        "chargeItemStatusEnumText":"退款中","itemDisplayName":"黄芪","lineTotalAmount":0.85}
class ChargeItemLineDTO : public oatpp::DTO
{
    DTO_INIT(ChargeItemLineDTO, DTO);

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
    DTO_FIELD(String, itemDisplayName);
    DTO_FIELD_INFO(itemDisplayName) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.itemDisplayName");
    };
    DTO_FIELD(Float64, lineTotalAmount);
    DTO_FIELD_INFO(lineTotalAmount) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.lineTotalAmount");
    };
};

// ==================== 支付组 DTO ====================
// 对应 JSON: paymentGroups[] 的每个元素
// 示例: {"paymentId":"...","paymentBusNo":"PMR-2026-07-190015",
//        "paymentStatusEnum":"SUCCESS","paymentStatusEnumText":"成功（收款已完成）",
//        "entererUserId":"...","collectorName":"admin","halfRefundOnPayment":false,
//        "refundingLines":[...],"preservedLines":[],
//        "refundableAmount":24.91,"originalPayChannelCodes":["CASH"]}
class PaymentGroupDTO : public oatpp::DTO
{
    DTO_INIT(PaymentGroupDTO, DTO);

    DTO_FIELD(String, paymentId);
    DTO_FIELD_INFO(paymentId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.paymentId");
    };
    DTO_FIELD(String, paymentBusNo);
    DTO_FIELD_INFO(paymentBusNo) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.paymentBusNo");
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
    // 退费行列表（嵌套数组）
    DTO_FIELD(oatpp::List<ChargeItemLineDTO::Wrapper>, refundingLines);
    DTO_FIELD_INFO(refundingLines) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.refundingLines");
    };
    // 保留行列表（嵌套数组，结构同 refundingLines）
    DTO_FIELD(oatpp::List<ChargeItemLineDTO::Wrapper>, preservedLines);
    DTO_FIELD_INFO(preservedLines) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.preservedLines");
    };
    DTO_FIELD(Float64, refundableAmount);
    DTO_FIELD_INFO(refundableAmount) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.refundableAmount");
    };
    // 原始支付渠道编码列表（字符串数组，如 ["CASH"]）
    DTO_FIELD(oatpp::List<String>, originalPayChannelCodes);
    DTO_FIELD_INFO(originalPayChannelCodes) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.originalPayChannelCodes");
    };
};

// ==================== 退费项目顶层响应 DTO ====================
// 对应 JSON: data 字段
// 示例: {"encounterId":"...","paymentGroups":[{...},...]}
class GetItemDTO : public oatpp::DTO
{
    DTO_INIT(GetItemDTO, DTO);

    DTO_FIELD(String, encounterId);
    DTO_FIELD_INFO(encounterId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.encounterId");
    };
    // 支付组列表（嵌套数组）
    DTO_FIELD(oatpp::List<PaymentGroupDTO::Wrapper>, paymentGroups);
    DTO_FIELD_INFO(paymentGroups) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.paymentGroups");
    };
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_GET_ITEM_DTO_
