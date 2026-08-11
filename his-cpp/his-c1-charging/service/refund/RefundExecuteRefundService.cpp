/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/08/11 00:00:00

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
// Steps:
//   Pre-A: verify original payment exists and is refundable
//   Pre-B: verify refund amount <= remaining refundable amount
//   Pre-C: verify all charge items belong to this payment
//   1. INSERT reverse fin_payment_reconciliation
//   2. INSERT reverse fin_payment_rec_detail rows
//   3. INSERT reverse fin_payment_rec_static
//   4. UPDATE adm_charge_item to REFUNDED
//   5. UPDATE original fin_payment_reconciliation (returned_amount + status)
//   6. UPDATE fin_payment_yb_settlement (revoke if confirmed)
// Every INSERT/UPDATE step checks affected row count; any failure rolls back.
RefundExecuteRefundDTO::Wrapper RefundExecuteRefundService::execute(const RefundExecuteRefundRequestDTO::Wrapper& dto)
{
    auto result = RefundExecuteRefundDTO::createShared();

    // ---- Extract request fields ----
    std::string originalPaymentId = dto->originalPaymentId.getValue("");
    std::string encounterId = dto->encounterId.getValue("");
    std::string billDate = dto->billDate.getValue("");
    std::string chargeItemIdsStr = joinListToString(dto->refundingChargeItemIds);

    // Basic non-empty validation
    if (originalPaymentId.empty() || chargeItemIdsStr.empty()) {
        result->success = false;
        result->errorCode = "INVALID_PARAMS";
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
    if (refundTotal <= 0) {
        result->success = false;
        result->errorCode = "INVALID_REFUND_AMOUNT";
        return result;
    }

    std::vector<std::string> itemIds = StringUtil::split(chargeItemIdsStr, ",");

    RefundExecuteRefundDAO dao;
    auto session = dao.getSqlSession();

    // Will be set after pre-validation passes
    double tenderedAmount = 0;
    double returnedAmount = 0;

    try {
        session->beginTransaction();

        // ================================================================
        // Pre-validation A: verify original payment exists and is refundable
        // ================================================================
        {
            std::ostringstream sql;
            sql << "SELECT tendered_amount, COALESCE(returned_amount, 0) AS returned_amount,"
                << " status_enum, charge_item_ids"
                << " FROM fin_payment_reconciliation"
                << " WHERE id = ? AND is_deleted = 0";
            // Use raw connection for multi-column fetch (same pattern as RefundPreparePrintService)
            auto conn = session->getConnection();
            auto pstmt = conn->prepareStatement(sql.str());
            pstmt->setString(1, originalPaymentId);
            auto res = pstmt->executeQuery();

            if (!res->next()) {
                delete res;
                delete pstmt;
                session->rollbackTransaction();
                result->success = false;
                result->errorCode = "PAYMENT_NOT_FOUND";
                result->originalPaymentId = originalPaymentId;
                return result;
            }

            tenderedAmount = res->getDouble("tendered_amount");
            returnedAmount = res->getDouble("returned_amount");
            std::string statusEnum = res->getString("status_enum");
            std::string origChargeItemIds = res->getString("charge_item_ids");
            delete res;
            delete pstmt;

            // Check not already fully refunded
            if (statusEnum == "FULLY_REFUNDED") {
                session->rollbackTransaction();
                result->success = false;
                result->errorCode = "PAYMENT_ALREADY_REFUNDED";
                result->originalPaymentId = originalPaymentId;
                return result;
            }

            // ============================================================
            // Pre-validation B: refund amount <= remaining refundable amount
            // ============================================================
            double refundableAmount = tenderedAmount - returnedAmount;
            if (refundTotal > refundableAmount) {
                session->rollbackTransaction();
                result->success = false;
                result->errorCode = "REFUND_AMOUNT_EXCEEDS";
                result->originalPaymentId = originalPaymentId;
                return result;
            }

            // ============================================================
            // Pre-validation C: all charge items belong to this payment
            // ============================================================
            if (!origChargeItemIds.empty()) {
                for (auto& ciId : itemIds) {
                    if (ciId.empty()) continue;
                    std::ostringstream sqlItem;
                    sqlItem << "SELECT COUNT(*) FROM adm_charge_item"
                        << " WHERE id = ?"
                        << " AND is_deleted = 0"
                        << " AND FIND_IN_SET(id, ?) > 0";
                    SqlParams pItem;
                    SQLPARAMS_PUSH(pItem, "s", std::string, ciId);
                    SQLPARAMS_PUSH(pItem, "s", std::string, origChargeItemIds);
                    uint64_t found = session->executeQueryNumerical(sqlItem.str(), pItem);
                    if (found == 0) {
                        session->rollbackTransaction();
                        result->success = false;
                        result->errorCode = "CHARGE_ITEM_NOT_IN_PAYMENT";
                        result->originalPaymentId = originalPaymentId;
                        return result;
                    }
                }
            }
        }

        // ================================================================
        // All pre-validations passed — execute refund
        // ================================================================

        // Step 1: Generate reverse payment ID and bus no
        UuidFacade uuid;
        std::string reverseId = uuid.genUuid();
        std::string reverseBusNo = "RVS-" + reverseId.substr(0, 18);

        // Step 2: INSERT reverse fin_payment_reconciliation record
        // Uses INSERT ... SELECT to copy fields from original payment
        int affectedRows = 0;
        {
            std::ostringstream sql;
            sql << "INSERT INTO fin_payment_reconciliation"
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

            SqlParams params;
            SQLPARAMS_PUSH(params, "s", std::string, reverseId);
            SQLPARAMS_PUSH(params, "s", std::string, originalPaymentId);
            SQLPARAMS_PUSH(params, "s", std::string, reverseBusNo);
            SQLPARAMS_PUSH(params, "d", double, -refundTotal);
            SQLPARAMS_PUSH(params, "d", double, -refundTotal);
            SQLPARAMS_PUSH(params, "s", std::string, chargeItemIdsStr);
            SQLPARAMS_PUSH(params, "s", std::string, billDate);
            SQLPARAMS_PUSH(params, "s", std::string, originalPaymentId);
            affectedRows = session->executeUpdate(sql.str(), params);
        }
        if (affectedRows != 1) {
            session->rollbackTransaction();
            result->success = false;
            result->errorCode = "INSERT_REVERSE_FAILED";
            result->originalPaymentId = originalPaymentId;
            return result;
        }

        // Step 3: INSERT reverse payment details (one per refund channel)
        if (dto->refundPaymentDetails) {
            for (auto& detail : *dto->refundPaymentDetails) {
                std::string detailId = UuidFacade().genUuid();
                double detailAmount = detail->amount ? detail->amount.getValue(0.0) : 0.0;
                std::string payChannel = detail->payChannelCode ? detail->payChannelCode.getValue("") : "";

                std::ostringstream sql;
                sql << "INSERT INTO fin_payment_rec_detail"
                    << " (id, tenant_id, created_at, reconciliation_id, account_id, pay_enum, amount, result_enum)"
                    << " VALUES (?, '1', NOW(), ?, '1', ?, ?, 'SUCCESS')";

                SqlParams params;
                SQLPARAMS_PUSH(params, "s", std::string, detailId);
                SQLPARAMS_PUSH(params, "s", std::string, reverseId);
                SQLPARAMS_PUSH(params, "s", std::string, payChannel);
                SQLPARAMS_PUSH(params, "d", double, -detailAmount);
                int detailRows = session->executeUpdate(sql.str(), params);
                if (detailRows != 1) {
                    session->rollbackTransaction();
                    result->success = false;
                    result->errorCode = "INSERT_DETAIL_FAILED";
                    result->originalPaymentId = originalPaymentId;
                    return result;
                }
            }
        }

        // Step 4: INSERT reverse payment static (copy from original, negative amounts)
        {
            std::ostringstream sql;
            sql << "INSERT INTO fin_payment_rec_static"
                << " (id, tenant_id, created_at, reconciliation_id, method_code, type_code, type_name, amount, charge_item_ids)"
                << " SELECT ?, '1', NOW(), ?, ops.method_code, ops.type_code, ops.type_name, -ops.amount, ?"
                << " FROM fin_payment_rec_static ops"
                << " WHERE ops.reconciliation_id = ?";

            SqlParams params;
            SQLPARAMS_PUSH(params, "s", std::string, UuidFacade().genUuid());
            SQLPARAMS_PUSH(params, "s", std::string, reverseId);
            SQLPARAMS_PUSH(params, "s", std::string, chargeItemIdsStr);
            SQLPARAMS_PUSH(params, "s", std::string, originalPaymentId);
            int staticRows = session->executeUpdate(sql.str(), params);
            // Accept >= 0: original payment may have no static snapshot rows
            if (staticRows < 0) {
                session->rollbackTransaction();
                result->success = false;
                result->errorCode = "INSERT_STATIC_FAILED";
                result->originalPaymentId = originalPaymentId;
                return result;
            }
        }

        // Step 5: UPDATE charge items to REFUNDED
        for (auto& ciId : itemIds) {
            if (ciId.empty()) continue;
            std::ostringstream sql;
            sql << "UPDATE adm_charge_item SET status_enum = 'REFUNDED', refund_id = ?"
                << " WHERE id = ? AND is_deleted = 0";
            SqlParams params;
            SQLPARAMS_PUSH(params, "s", std::string, reverseId);
            SQLPARAMS_PUSH(params, "s", std::string, ciId);
            int ciRows = session->executeUpdate(sql.str(), params);
            if (ciRows != 1) {
                session->rollbackTransaction();
                result->success = false;
                result->errorCode = "UPDATE_CHARGE_ITEM_FAILED";
                result->originalPaymentId = originalPaymentId;
                return result;
            }
        }

        // Step 6: UPDATE original payment — accumulate returned_amount, set status
        {
            std::ostringstream sql;
            sql << "UPDATE fin_payment_reconciliation"
                << " SET returned_amount = COALESCE(returned_amount, 0) + ?,"
                << " status_enum = CASE WHEN (COALESCE(returned_amount, 0) + ?) >= tendered_amount"
                << " THEN 'FULLY_REFUNDED' ELSE 'PARTIAL_REFUNDED' END,"
                << " updated_at = NOW()"
                << " WHERE id = ? AND is_deleted = 0";
            SqlParams params;
            SQLPARAMS_PUSH(params, "d", double, refundTotal);
            SQLPARAMS_PUSH(params, "d", double, refundTotal);
            SQLPARAMS_PUSH(params, "s", std::string, originalPaymentId);
            int origRows = session->executeUpdate(sql.str(), params);
            if (origRows != 1) {
                session->rollbackTransaction();
                result->success = false;
                result->errorCode = "UPDATE_ORIGINAL_FAILED";
                result->originalPaymentId = originalPaymentId;
                return result;
            }
        }

        // Step 7: Revoke insurance settlement if exists (optional — 0 rows is OK)
        int ybRows = 0;
        {
            std::ostringstream sql;
            sql << "UPDATE fin_payment_yb_settlement SET status = 'REVOKED'"
                << " WHERE reconciliation_id = ? AND status = 'CONFIRMED'";
            SqlParams params;
            SQLPARAMS_PUSH(params, "s", std::string, originalPaymentId);
            ybRows = session->executeUpdate(sql.str(), params);
        }

        // ================================================================
        // All steps succeeded — commit transaction
        // ================================================================
        session->commitTransaction();

        result->success = true;
        result->reversePaymentId = reverseId;
        result->originalPaymentId = originalPaymentId;
        // halfRefundExecuted: true if not all remaining amount is being refunded
        result->halfRefundExecuted = (refundTotal < (tenderedAmount - returnedAmount));
        result->insuranceRevoked = (ybRows > 0);

    } catch (const std::exception& e) {
        session->rollbackTransaction();
        result->success = false;
        result->errorCode = "INTERNAL_ERROR";
        result->originalPaymentId = originalPaymentId;
    }

    return result;
}
