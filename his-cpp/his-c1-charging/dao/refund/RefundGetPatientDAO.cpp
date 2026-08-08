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
#include "RefundGetPatientDAO.h"
#include "RefundGetPatientMapper.h"

// Build dynamic WHERE clause from query filters
// Uses parameterized queries via SqlParams to prevent SQL injection
inline std::string RefundGetPatientDAO::queryConditionBuilder(const RefundGetPatientQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream sqlCondition;
    sqlCondition << " WHERE 1=1";

    // Keyword search: fuzzy match on patient name, exact match on encounter id or bus_no
    if (query->keyword) {
        std::string kw = query->keyword.getValue("");
        if (!kw.empty()) {
            sqlCondition << " AND (p.name LIKE ? OR e.id = ? OR e.bus_no = ?)";
            SQLPARAMS_PUSH(params, "s", std::string, "%" + kw + "%");
            SQLPARAMS_PUSH(params, "s", std::string, kw);
            SQLPARAMS_PUSH(params, "s", std::string, kw);
        }
    }

    // Queue tab filter: PENDING = has refundable items, SETTLED = all items refunded
    if (query->queueTabCode) {
        std::string tabCode = query->queueTabCode.getValue("");
        if (tabCode == "PENDING") {
            // Encounters that have at least one charge item not yet refunded
            sqlCondition << " AND EXISTS ("
                "SELECT 1 FROM adm_charge_item ci_filter"
                " WHERE ci_filter.encounter_id = e.id"
                " AND ci_filter.status_enum != 'REFUNDED'"
                ")";
        }
        else if (tabCode == "SETTLED") {
            // Encounters that have charge items AND all are refunded
            sqlCondition << " AND EXISTS ("
                "SELECT 1 FROM adm_charge_item ci_filter"
                " WHERE ci_filter.encounter_id = e.id"
                ") AND NOT EXISTS ("
                "SELECT 1 FROM adm_charge_item ci_filter2"
                " WHERE ci_filter2.encounter_id = e.id"
                " AND ci_filter2.status_enum != 'REFUNDED'"
                ")";
        }
        // "ALL" or empty: no queue filter
    }

    // Date range filter on encounter start_time
    if (query->startDate) {
        std::string sd = query->startDate.getValue("");
        if (!sd.empty()) {
            sqlCondition << " AND e.start_time >= ?";
            SQLPARAMS_PUSH(params, "s", std::string, sd + " 00:00:00");
        }
    }
    if (query->endDate) {
        std::string ed = query->endDate.getValue("");
        if (!ed.empty()) {
            sqlCondition << " AND e.start_time <= ?";
            SQLPARAMS_PUSH(params, "s", std::string, ed + " 23:59:59");
        }
    }

    return sqlCondition.str();
}

// Query total count of matching encounters
uint64_t RefundGetPatientDAO::count(const RefundGetPatientQuery::Wrapper& query)
{
    SqlParams params;
    std::ostringstream sql;
    sql << "SELECT COUNT(DISTINCT e.id)"
        << " FROM adm_encounter e"
        << " LEFT JOIN adm_patient p ON e.patient_id = p.id";
    sql << queryConditionBuilder(query, params);
    return sqlSession->executeQueryNumerical(sql.str(), params);
}

// Paginated query — multi-table JOIN with dictionary lookup and aggregation
std::list<RefundGetPatientDO> RefundGetPatientDAO::selectWithPage(const RefundGetPatientQuery::Wrapper& query)
{
    // Final version:
    // - ageText: returned as raw birth_date (age computed in Service layer to avoid
    //   MySQL C++ Connector prepared-statement crash with date functions)
    // - genderEnumText / chargeItemStatusSummary: populated via dictionary JOINs
    // - queueLabelCode: computed from charge_item status aggregation
    SqlParams params;
    std::ostringstream sql;

    sql << "SELECT"
        << " e.id AS encounter_id,"
        << " e.bus_no AS encounter_bus_no,"
        << " e.patient_id AS patient_id,"
        << " MAX(p.name) AS patient_name,"
        << " MAX(p.gender_enum) AS gender_code,"
        << " COALESCE(MAX(tc_gender.display), '') AS gender_enum_text,"
        // Raw birth_date — age calculation done in C++ Service layer
        << " MAX(p.birth_date) AS age_text,"

        << " CASE WHEN COUNT(CASE WHEN ci.id IS NOT NULL AND ci.status_enum != 'REFUNDED' THEN 1 END) > 0"
        << " THEN 'PENDING' ELSE 'SETTLED' END AS queue_label_code,"

        << " COALESCE(MAX(tc_status.display), '') AS charge_item_status_summary,"

        << " MAX(o.name) AS organization_name,"
        << " MAX(doc.name) AS doctor_name,"
        << " DATE_FORMAT(e.start_time, '%Y-%m-%d %H:%i:%s') AS start_time,"
        << " COALESCE(SUM(ci.total_price), 0) AS total_amount,"
        << " COALESCE(SUM(fpr.tendered_amount), 0) AS paid_amount"

        << " FROM adm_encounter e"
        << " LEFT JOIN adm_patient p ON e.patient_id = p.id"
        << " LEFT JOIN adm_organization o ON e.organization_id = o.id"
        << " LEFT JOIN adm_encounter_participant aep ON aep.encounter_id = e.id AND aep.is_deleted = 0"
        << " LEFT JOIN adm_practitioner doc ON aep.practitioner_id = doc.id"
        << " LEFT JOIN adm_charge_item ci ON ci.encounter_id = e.id AND ci.is_deleted = 0"
        << " LEFT JOIN fin_payment_reconciliation fpr ON fpr.encounter_id = e.id AND fpr.is_deleted = 0"
        // Gender dictionary: ter_code_system.name='sys_user_sex' → ter_concept
        << " LEFT JOIN ter_code_system tcs_gender ON tcs_gender.name = 'sys_user_sex'"
        << " LEFT JOIN ter_concept tc_gender ON tc_gender.code_system_id = tcs_gender.id"
        << " AND tc_gender.code = p.gender_enum"
        // Charge item status dictionary
        << " LEFT JOIN ter_code_system tcs_status ON tcs_status.name = 'sys_charge_item_status'"
        << " LEFT JOIN ter_concept tc_status ON tc_status.code_system_id = tcs_status.id"
        << " AND tc_status.code = ci.status_enum";

    sql << queryConditionBuilder(query, params);

    sql << " GROUP BY e.id"
        << " ORDER BY e.start_time DESC"
        << " LIMIT " << std::to_string((int64_t)((query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10)))
        << "," << std::to_string((int64_t)query->pageSize.getValue(10));

    return sqlSession->executeQuery<RefundGetPatientDO>(sql.str(), RefundGetPatientMapper(), params);
}
