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
#ifndef _EXECUTE_REFUND_DTO_
#define _EXECUTE_REFUND_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Refund payment detail DTO
// JSON: refundPaymentDetails[n].{amount, payChannelCode}
class RefundPaymentDetailDTO : public oatpp::DTO
{
    DTO_INIT(RefundPaymentDetailDTO, DTO);

    DTO_FIELD(Float64, amount);
    DTO_FIELD_INFO(amount) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.amount");
    };
    DTO_FIELD(String, payChannelCode);
    DTO_FIELD_INFO(payChannelCode) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.payChannelCode");
    };
};

// Execute refund request DTO
// sample-json: {billDate, encounterId, originalPaymentId, refundingChargeItemIds, refundPaymentDetails}
class ExecuteRefundRequestDTO : public oatpp::DTO
{
    DTO_INIT(ExecuteRefundRequestDTO, DTO);

    DTO_FIELD(String, billDate);
    DTO_FIELD_INFO(billDate) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.billDate");
    };
    DTO_FIELD(String, encounterId);
    DTO_FIELD_INFO(encounterId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.encounterId");
    };
    DTO_FIELD(String, originalPaymentId);
    DTO_FIELD_INFO(originalPaymentId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.originalPaymentId");
    };
    DTO_FIELD(oatpp::List<String>, refundingChargeItemIds, "refundingChargeItemIds");
    DTO_FIELD_INFO(refundingChargeItemIds) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.refundingChargeItemIds");
    };
    DTO_FIELD(oatpp::List<RefundPaymentDetailDTO::Wrapper>, refundPaymentDetails);
    DTO_FIELD_INFO(refundPaymentDetails) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.refundPaymentDetails");
    };
};

// Execute refund response DTO
// sample-json: {success, reversePaymentId, originalPaymentId, halfRefundExecuted, insuranceRevoked}
class ExecuteRefundDTO : public oatpp::DTO
{
    DTO_INIT(ExecuteRefundDTO, DTO);

    DTO_FIELD(Boolean, success);
    DTO_FIELD_INFO(success) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.success");
    };
    DTO_FIELD(String, reversePaymentId);
    DTO_FIELD_INFO(reversePaymentId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.reversePaymentId");
    };
    DTO_FIELD(String, originalPaymentId);
    DTO_FIELD_INFO(originalPaymentId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.originalPaymentId");
    };
    DTO_FIELD(Boolean, halfRefundExecuted);
    DTO_FIELD_INFO(halfRefundExecuted) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.halfRefundExecuted");
    };
    DTO_FIELD(Boolean, insuranceRevoked);
    DTO_FIELD_INFO(insuranceRevoked) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.insuranceRevoked");
    };
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_EXECUTE_REFUND_DTO_
