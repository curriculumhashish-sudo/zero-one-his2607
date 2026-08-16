/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/26 00:00:00

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
#include "stdafx.h"
#include "RefundGetItemService.h"
#include "../../dao/refund/RefundGetItemDAO.h"
#include <sstream>
#include "StringUtil.h"

// Prevent macro conflicts
#include "NacosClient.h"

// Fallback dictionary mapping for payment status enums
static std::string fallbackPaymentStatusText(const std::string& code) {
    if (code == "SUCCESS") return "Success";
    if (code == "REFUNDING") return "Refunding";
    if (code == "REFUNDED") return "Refunded";
    return code;
}

// Fallback dictionary mapping for charge item status enums
static std::string fallbackChargeItemStatusText(const std::string& code) {
    if (code == "REFUNDING") return "Refunding";
    if (code == "REFUNDED") return "Refunded";
    if (code == "PRESERVED") return "Preserved";
    if (code == "SETTLED") return "Settled";
    return code;
}

// Query refund item details for a given encounter
// Groups flat DO rows by payment_id, then categorizes charge items into
// refundingLines (REFUNDING) and preservedLines (PRESERVED/SETTLED/REFUNDED)
RefundGetItemDTO::Wrapper RefundGetItemService::listAll(const RefundGetItemQuery::Wrapper& query)
{
    auto dto = RefundGetItemDTO::createShared();

    // Set encounter ID from query
    if (query->encounterId) {
        dto->encounterId = query->encounterId;
    }

    // Query flat data (already ordered by fpr.id = payment_id)
    RefundGetItemDAO dao;
    std::list<RefundGetItemDO> flatList = dao.selectWithPage(query);
    if (flatList.empty()) {
        dto->paymentGroups = oatpp::List<RefundPaymentGroupDTO::Wrapper>::createShared();
        return dto;
    }

    // Sequential grouping — avoids copying RefundGetItemDO (BaseDO deletes copy ctor)
    // SQL is ORDER BY fpr.id, so rows for the same payment are contiguous
    auto groupList = oatpp::List<RefundPaymentGroupDTO::Wrapper>::createShared();
    RefundPaymentGroupDTO::Wrapper currentGroup;
    std::string currentPaymentId;

    for (RefundGetItemDO& row : flatList) {
        std::string pid = row.getPaymentRecordId();

        // New payment group
        if (pid != currentPaymentId) {
            if (currentGroup) {
                groupList->push_back(currentGroup);
            }
            currentPaymentId = pid;

            currentGroup = RefundPaymentGroupDTO::createShared();
            currentGroup->refundingLines = oatpp::List<RefundChargeItemLineDTO::Wrapper>::createShared();
            currentGroup->preservedLines = oatpp::List<RefundChargeItemLineDTO::Wrapper>::createShared();

            // Payment-level fields from first row
            currentGroup->paymentRecordId = row.getPaymentRecordId();
            currentGroup->paymentNo = row.getPaymentNo();
            currentGroup->paymentStatusEnum = row.getPaymentStatusEnum();
            currentGroup->paymentStatusEnumText = fallbackPaymentStatusText(row.getPaymentStatusEnum());
            currentGroup->entererUserId = row.getEntererUserId();
            currentGroup->collectorName = row.getCollectorName();
            currentGroup->halfRefundOnPayment = (row.getHalfRefundOnPayment() == "true");
            currentGroup->refundableAmount = std::stod(row.getRefundableAmount());

            // originalPayChannelCodes: comma-separated -> List<String>
            auto channelList = oatpp::List<oatpp::String>::createShared();
            std::string channels = row.getOriginalPayChannelCodes();
            if (!channels.empty()) {
                std::istringstream iss(channels);
                std::string token;
                while (std::getline(iss, token, ',')) {
                    if (!token.empty()) {
                        channelList->push_back(token);
                    }
                }
            }
            currentGroup->originalPayChannelCodes = channelList;
        }

        // Build charge item line for this row
        auto line = RefundChargeItemLineDTO::createShared();
        line->chargeItemId = row.getChargeItemId();
        line->paymentId = row.getPaymentRecordId();
        line->itemName = row.getItemName();
        line->lineTotalAmount = std::stod(row.getLineTotalAmount());

        std::string statusEnum = row.getChargeItemStatusEnum();
        line->chargeItemStatusEnum = statusEnum;
        line->chargeItemStatusEnumText = fallbackChargeItemStatusText(statusEnum);

        // Categorize: REFUNDING -> refundingLines, others -> preservedLines
        if (statusEnum == "REFUNDING") {
            currentGroup->refundingLines->push_back(line);
        }
        else {
            currentGroup->preservedLines->push_back(line);
        }
    }
    // Don't forget the last group
    if (currentGroup) {
        groupList->push_back(currentGroup);
    }

    dto->paymentGroups = groupList;
    return dto;
}
