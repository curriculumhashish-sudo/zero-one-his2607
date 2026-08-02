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
#include "RefundGetItemDAO.h"
#include "RefundGetItemMapper.h"

// Build dynamic WHERE clause from query filters
inline std::string RefundGetItemDAO::queryConditionBuilder(const RefundGetItemQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream sqlCondition;
    sqlCondition << " WHERE fpr.is_deleted = 0";

    // Filter by encounter ID (required)
    if (query->encounterId) {
        std::string encId = query->encounterId.getValue("");
        if (!encId.empty()) {
            sqlCondition << " AND fpr.encounter_id = ?";
            SQLPARAMS_PUSH(params, "s", std::string, encId);
        }
    }

    // Filter by detail view code: PENDING = only refunding items, SETTLED = only refunded
    if (query->detailViewCode) {
        std::string viewCode = query->detailViewCode.getValue("");
        if (viewCode == "PENDING") {
            sqlCondition << " AND ci.status_enum = 'REFUNDING'";
        }
        else if (viewCode == "SETTLED") {
            sqlCondition << " AND ci.status_enum = 'REFUNDED'";
        }
        // "ALL" or empty: no status filter
    }

    return sqlCondition.str();
}

// Count matching rows
uint64_t RefundGetItemDAO::count(const RefundGetItemQuery::Wrapper& query)
{
    SqlParams params;
    std::ostringstream sql;
    sql << "SELECT COUNT(*)"
        << " FROM fin_payment_reconciliation fpr"
        << " JOIN adm_charge_item ci ON FIND_IN_SET(ci.id, fpr.charge_item_ids) AND ci.is_deleted = 0";
    sql << queryConditionBuilder(query, params);
    return sqlSession->executeQueryNumerical(sql.str(), params);
}

// Flat query — one row per (payment, charge_item) combination
// Service layer groups by payment_id and categorizes into refundingLines / preservedLines
std::list<RefundGetItemDO> RefundGetItemDAO::selectWithPage(const RefundGetItemQuery::Wrapper& query)
{
    SqlParams params;
    std::ostringstream sql;

    sql << "SELECT"
        << " fpr.encounter_id AS encounter_id,"
        << " fpr.id AS payment_id,"
        << " fpr.payment_no AS payment_bus_no,"
        << " fpr.status_enum AS payment_status_enum,"
        << " fpr.enterer_id AS enterer_user_id,"
        << " MAX(ap.name) AS collector_name,"
        // half_refund_on_payment: true if partial refund was applied
        << " CASE WHEN fpr.returned_amount > 0"
        << " AND COALESCE(fpr.returned_amount, 0) < fpr.tendered_amount"
        << " THEN 'true' ELSE 'false' END AS half_refund_on_payment,"
        // refundable_amount: tendered - already returned
        << " (fpr.tendered_amount - COALESCE(fpr.returned_amount, 0)) AS refundable_amount,"
        // Pay channel codes: aggregated from detail table via subquery
        << " COALESCE((SELECT GROUP_CONCAT(DISTINCT fprd.pay_enum)"
        << " FROM fin_payment_rec_detail fprd"
        << " WHERE fprd.reconciliation_id = fpr.id), '') AS original_pay_channel_codes,"
        // Charge item columns
        << " ci.id AS charge_item_id,"
        << " ci.status_enum AS charge_item_status_enum,"
        << " COALESCE(MAX(cid.charge_name), '') AS item_display_name,"
        << " ci.total_price AS line_total_amount"

        // Dictionary columns filled by Service fallback (DB dict uses numeric codes)
        << " , '' AS payment_status_enum_text"
        << " , '' AS charge_item_status_enum_text"

        << " FROM fin_payment_reconciliation fpr"
        << " JOIN adm_charge_item ci ON FIND_IN_SET(ci.id, fpr.charge_item_ids) AND ci.is_deleted = 0"
        << " LEFT JOIN adm_charge_item_definition cid ON ci.definition_id = cid.id"
        << " LEFT JOIN adm_practitioner ap ON ap.id = fpr.enterer_id";

    sql << queryConditionBuilder(query, params);

    // GROUP BY payment + charge_item to deduplicate from JOINs
    // MAX() on JOIN columns for ONLY_FULL_GROUP_BY compatibility
    sql << " GROUP BY fpr.id, ci.id"
        << " ORDER BY fpr.created_at, ci.id";

    return sqlSession->executeQuery<RefundGetItemDO>(sql.str(), RefundGetItemMapper(), params);
}
