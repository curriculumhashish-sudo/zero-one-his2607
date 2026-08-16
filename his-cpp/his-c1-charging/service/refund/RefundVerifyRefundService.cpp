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
#include "RefundVerifyRefundService.h"
#include "../../dao/refund/RefundVerifyRefundDAO.h"
#include "StringUtil.h"
#include <sstream>

// Prevent macro conflicts
#include "NacosClient.h"

// Verify whether selected charge items can be refunded
// Checks: items exist and are not already refunded, payment allows refund, no insurance conflict
RefundVerifyRefundDTO::Wrapper RefundVerifyRefundService::verify(const RefundVerifyRefundQuery::Wrapper& query)
{
    auto dto = RefundVerifyRefundDTO::createShared();

    // Parse refundingChargeItemIds from comma-separated string to vector
    std::string idsStr = query->refundingChargeItemIds.getValue("");
    if (idsStr.empty()) {
        dto->allowed = false;
        dto->rejectReasonCode = "NO_CHARGE_ITEMS_PROVIDED";
        return dto;
    }

    std::vector<std::string> chargeItemIds = StringUtil::split(idsStr, ",");
    if (chargeItemIds.empty()) {
        dto->allowed = false;
        dto->rejectReasonCode = "NO_CHARGE_ITEMS_PROVIDED";
        return dto;
    }

    // Build dynamic IN (?, ?, ...) clause with parameterized bindings
    RefundVerifyRefundDAO dao;
    SqlParams params;
    std::ostringstream inClause;
    for (size_t i = 0; i < chargeItemIds.size(); i++) {
        if (i > 0) inClause << ",";
        inClause << "?";
        SQLPARAMS_PUSH(params, "s", std::string, chargeItemIds[i]);
    }

    // Check: all charge items exist, are not deleted, not already refunded
    std::ostringstream sqlItems;
    sqlItems << "SELECT COUNT(*) FROM adm_charge_item"
        << " WHERE id IN (" << inClause.str() << ")"
        << " AND is_deleted = 0"
        << " AND status_enum != 'REFUNDED'"
        << " AND (refund_id IS NULL OR refund_id = '')";
    uint64_t validCount = dao.getSqlSession()->executeQueryNumerical(sqlItems.str(), params);

    if (validCount != chargeItemIds.size()) {
        dto->allowed = false;
        dto->rejectReasonCode = "CHARGE_ITEM_NOT_REFUNDABLE";
        return dto;
    }

    // Check: payment has no pending insurance settlement for this encounter
    std::string encId = query->encounterId.getValue("");
    if (!encId.empty()) {
        SqlParams payParams;
        std::ostringstream sqlPay;
        sqlPay << "SELECT COUNT(*) FROM fin_payment_reconciliation fpr"
            << " JOIN fin_payment_yb_settlement ybs ON ybs.reconciliation_id = fpr.id"
            << " WHERE fpr.encounter_id = ?"
            << " AND fpr.is_deleted = 0"
            << " AND ybs.status = 'CONFIRMED'";
        SQLPARAMS_PUSH(payParams, "s", std::string, encId);
        uint64_t insuranceCount = dao.getSqlSession()->executeQueryNumerical(sqlPay.str(), payParams);

        if (insuranceCount > 0) {
            dto->allowed = false;
            dto->rejectReasonCode = "INSURANCE_SETTLED";
            return dto;
        }
    }

    // All checks passed — rejectReasonCode set to empty (none)
    dto->allowed = true;
    dto->rejectReasonCode = "";
    return dto;
}
