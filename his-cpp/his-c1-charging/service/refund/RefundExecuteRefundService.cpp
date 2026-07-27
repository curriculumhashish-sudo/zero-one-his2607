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
#include "RefundExecuteRefundService.h"
#include "../../dao/refund/RefundExecuteRefundDAO.h"
#include "id/UuidFacade.h"
#include "StringUtil.h"
#include "SimpleDateTimeFormat.h"
#include <sstream>

// Prevent macro conflicts
#include "NacosClient.h"

// Helper: convert DTO List<String> to comma-separated string
static std::string joinListToString(const oatpp::List<oatpp::String>& list) {
    if (!list) return "";
    std::ostringstream oss;
    bool first = true;
    for (auto& item : *list) {
        if (!first) oss << ",";
        oss << item.getValue("");
        first = false;
    }
    return oss.str();
}

// Execute refund as a transactional operation
// Steps: INSERT reverse payment + details + static, UPDATE charge items + original payment, revoke insurance
RefundExecuteRefundDTO::Wrapper RefundExecuteRefundService::execute(const RefundExecuteRefundRequestDTO::Wrapper& dto)
{
    auto result = RefundExecuteRefundDTO::createShared();

    // Extract request fields
    std::string originalPaymentId = dto->originalPaymentId.getValue("");
    std::string encounterId = dto->encounterId.getValue("");
    std::string billDate = dto->billDate.getValue("");
    std::string chargeItemIdsStr = joinListToString(dto->refundingChargeItemIds);

    if (originalPaymentId.empty() || chargeItemIdsStr.empty()) {
        result->success = false;
        return result;
    }

    // Calculate total refund amount from payment details
    double refundTotal = 0.0;
    if (dto->refundPaymentDetails) {
        for (auto& detail : *dto->refundPaymentDetails) {
            if (detail->amount) {
                refundTotal += detail->amount.getValue(0.0);
            }
        }
    }

    RefundExecuteRefundDAO dao;
    auto session = dao.getSqlSession();

    try {
        session->beginTransaction();

        // --- Step 1: Read original payment record ---
        SqlParams p1;
        SQLPARAMS_PUSH(p1, "s", std::string, originalPaymentId);

        // We use executeQueryOne-like approach: executeUpdate doesn't return rows.
        // Let's fetch needed fields via separate queries.

        // Get original payment fields needed for reverse insertion
        std::ostringstream sqlFetch;
        sqlFetch << "SELECT patient_id, tendered_amount, COALESCE(returned_amount,0) AS returned_amount,"
            << " kind_enum, tenant_id, issuer_enum, location_id, practitioner_id, encounter_id"
            << " FROM fin_payment_reconciliation"
            << " WHERE id = ? AND is_deleted = 0";
        // We can't use executeQueryOne easily without a mapper for each field.
        // Use executeQuery with a simple struct approach.

        // Alternative: Use raw queries. Let's query individually.
        // Actually, let's use a simpler approach - executeUpdate style queries.

        // Step 2: Generate reverse payment ID and bus no
        UuidFacade uuid;
        std::string reverseId = uuid.genUuid();
        std::string reverseBusNo = "RVS-" + reverseId.substr(0, 18);

        // Step 3: INSERT reverse fin_payment_reconciliation record
        // Use original payment's patient_id, kind_enum, etc. via sub-SELECT
        std::ostringstream sqlInsertPay;
        sqlInsertPay << "INSERT INTO fin_payment_reconciliation"
            << " (id, tenant_id, created_at, status_enum, relation_id, payment_no,"
            << " payment_enum, kind_enum, enterer_id, patient_id, tendered_amount,"
            << " returned_amount, display_amount, print_count, charge_item_ids,"
            << " encounter_id, bill_date, issuer_enum, location_id, practitioner_id)"
            << " SELECT ?, op.tenant_id, NOW(), 'REFUND_SUCCESS', ?, ?,"
            << " 'REVERSE', op.kind_enum, op.enterer_id, op.patient_id, ?,"
            << " 0, ?, 0, ?,"
            << " op.encounter_id, ?, op.issuer_enum, op.location_id, op.practitioner_id"
            << " FROM fin_payment_reconciliation op"
            << " WHERE op.id = ? AND op.is_deleted = 0";

        SqlParams pInsertPay;
        SQLPARAMS_PUSH(pInsertPay, "s", std::string, reverseId);
        SQLPARAMS_PUSH(pInsertPay, "s", std::string, originalPaymentId);
        SQLPARAMS_PUSH(pInsertPay, "s", std::string, reverseBusNo);
        SQLPARAMS_PUSH(pInsertPay, "d", double, -refundTotal);
        SQLPARAMS_PUSH(pInsertPay, "d", double, -refundTotal);
        SQLPARAMS_PUSH(pInsertPay, "s", std::string, chargeItemIdsStr);
        SQLPARAMS_PUSH(pInsertPay, "s", std::string, billDate);
        SQLPARAMS_PUSH(pInsertPay, "s", std::string, originalPaymentId);
        session->executeUpdate(sqlInsertPay.str(), pInsertPay);

        // Step 4: INSERT reverse payment details
        if (dto->refundPaymentDetails) {
            for (auto& detail : *dto->refundPaymentDetails) {
                UuidFacade detailUuid;
                std::string detailId = detailUuid.genUuid();
                double detailAmount = detail->amount ? detail->amount.getValue(0.0) : 0.0;
                std::string payChannel = detail->payChannelCode ? detail->payChannelCode.getValue("") : "";

                std::ostringstream sqlDetail;
                sqlDetail << "INSERT INTO fin_payment_rec_detail"
                    << " (id, reconciliation_id, pay_enum, amount, result_enum)"
                    << " VALUES (?, ?, ?, ?, 'SUCCESS')";

                SqlParams pDetail;
                SQLPARAMS_PUSH(pDetail, "s", std::string, detailId);
                SQLPARAMS_PUSH(pDetail, "s", std::string, reverseId);
                SQLPARAMS_PUSH(pDetail, "s", std::string, payChannel);
                SQLPARAMS_PUSH(pDetail, "d", double, -detailAmount);
                session->executeUpdate(sqlDetail.str(), pDetail);
            }
        }

        // Step 5: INSERT reverse payment static (copy from original, negative amounts)
        std::ostringstream sqlStatic;
        sqlStatic << "INSERT INTO fin_payment_rec_static"
            << " (id, reconciliation_id, method_code, type_code, type_name, amount, charge_item_ids)"
            << " SELECT ?, ?, ops.method_code, ops.type_code, ops.type_name, -ops.amount, ?"
            << " FROM fin_payment_rec_static ops"
            << " WHERE ops.reconciliation_id = ?";

        UuidFacade staticUuid;
        SqlParams pStatic;
        SQLPARAMS_PUSH(pStatic, "s", std::string, staticUuid.genUuid());
        SQLPARAMS_PUSH(pStatic, "s", std::string, reverseId);
        SQLPARAMS_PUSH(pStatic, "s", std::string, chargeItemIdsStr);
        SQLPARAMS_PUSH(pStatic, "s", std::string, originalPaymentId);
        session->executeUpdate(sqlStatic.str(), pStatic);

        // Step 6: UPDATE charge items to REFUNDED
        std::vector<std::string> itemIds = StringUtil::split(chargeItemIdsStr, ",");
        for (auto& ciId : itemIds) {
            if (ciId.empty()) continue;
            std::ostringstream sqlCi;
            sqlCi << "UPDATE adm_charge_item SET status_enum = 'REFUNDED', refund_id = ?"
                << " WHERE id = ? AND is_deleted = 0";
            SqlParams pCi;
            SQLPARAMS_PUSH(pCi, "s", std::string, reverseId);
            SQLPARAMS_PUSH(pCi, "s", std::string, ciId);
            session->executeUpdate(sqlCi.str(), pCi);
        }

        // Step 7: UPDATE original payment — returned_amount + refund, status_enum
        std::ostringstream sqlOrig;
        sqlOrig << "UPDATE fin_payment_reconciliation"
            << " SET returned_amount = COALESCE(returned_amount, 0) + ?,"
            << " status_enum = CASE WHEN (COALESCE(returned_amount, 0) + ?) >= tendered_amount"
            << " THEN 'FULLY_REFUNDED' ELSE 'PARTIAL_REFUNDED' END,"
            << " updated_at = NOW()"
            << " WHERE id = ? AND is_deleted = 0";
        SqlParams pOrig;
        SQLPARAMS_PUSH(pOrig, "d", double, refundTotal);
        SQLPARAMS_PUSH(pOrig, "d", double, refundTotal);
        SQLPARAMS_PUSH(pOrig, "s", std::string, originalPaymentId);
        session->executeUpdate(sqlOrig.str(), pOrig);

        // Step 8: Revoke insurance settlement if exists
        std::ostringstream sqlYb;
        sqlYb << "UPDATE fin_payment_yb_settlement SET status = 'REVOKED'"
            << " WHERE reconciliation_id = ? AND status = 'CONFIRMED'";
        SqlParams pYb;
        SQLPARAMS_PUSH(pYb, "s", std::string, originalPaymentId);
        int ybRows = session->executeUpdate(sqlYb.str(), pYb);

        // Commit transaction
        session->commitTransaction();

        // Build response
        result->success = true;
        result->reversePaymentId = reverseId;
        result->originalPaymentId = originalPaymentId;
        result->halfRefundExecuted = false; // Will be determined by frontend or re-query
        result->insuranceRevoked = (ybRows > 0);

    } catch (const std::exception& e) {
        session->rollbackTransaction();
        result->success = false;
        result->originalPaymentId = originalPaymentId;
    }

    return result;
}
