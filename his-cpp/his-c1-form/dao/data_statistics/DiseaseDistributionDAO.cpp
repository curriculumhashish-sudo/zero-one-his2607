/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
 @Date: 2026/07/27 00:00:00

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
#include "DiseaseDistributionDAO.h"
#include "DiseaseDistributionMapper.h"

std::string DiseaseDistributionDAO::queryConditionBuilder(const DiseaseDistributionQuery::Wrapper& query, SqlParams& params)
{
    std::stringstream sqlCondition;
    sqlCondition << " WHERE e.is_deleted =0";
    // Start date filter
    if (query->dateFrom) {
        sqlCondition << " AND e.start_time >=?";
        SQLPARAMS_PUSH(params, "s", std::string, query->dateFrom.getValue(""));
    }
    // End date filter
    if (query->dateTo) {
        sqlCondition << " AND e.start_time <=?";
        SQLPARAMS_PUSH(params, "s", std::string, query->dateTo.getValue(""));
    }
    // Organization filter (data isolation)
    if (query->organizationId) {
        sqlCondition << " AND e.organization_id =?";
        SQLPARAMS_PUSH(params, "s", std::string, query->organizationId.getValue(""));
    }
    return sqlCondition.str();
}

uint64_t DiseaseDistributionDAO::count(const DiseaseDistributionQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql =
        "SELECT COUNT(DISTINCT cd.name) "
        "FROM adm_encounter e "
        "JOIN adm_encounter_diagnosis ed ON ed.encounter_id = e.id "
        "JOIN cln_condition c ON c.id = ed.condition_id "
        "JOIN cln_condition_definition cd ON cd.id = c.definition_id ";
    sql += queryConditionBuilder(query, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

std::list<DiseaseDistributionDO> DiseaseDistributionDAO::selectDistribution(const DiseaseDistributionQuery::Wrapper& query)
{
    SqlParams params;

    // Build subquery condition first (parameter order matters for positional binding)
    std::stringstream subCond;
    subCond << " WHERE e2.is_deleted =0";
    if (query->dateFrom) {
        subCond << " AND e2.start_time >=?";
        SQLPARAMS_PUSH(params, "s", std::string, query->dateFrom.getValue(""));
    }
    if (query->dateTo) {
        subCond << " AND e2.start_time <=?";
        SQLPARAMS_PUSH(params, "s", std::string, query->dateTo.getValue(""));
    }
    if (query->organizationId) {
        subCond << " AND e2.organization_id =?";
        SQLPARAMS_PUSH(params, "s", std::string, query->organizationId.getValue(""));
    }

    // Build outer query condition (same values, for outer query placeholders)
    std::stringstream outerCond;
    outerCond << " WHERE e.is_deleted =0";
    if (query->dateFrom) {
        outerCond << " AND e.start_time >=?";
        SQLPARAMS_PUSH(params, "s", std::string, query->dateFrom.getValue(""));
    }
    if (query->dateTo) {
        outerCond << " AND e.start_time <=?";
        SQLPARAMS_PUSH(params, "s", std::string, query->dateTo.getValue(""));
    }
    if (query->organizationId) {
        outerCond << " AND e.organization_id =?";
        SQLPARAMS_PUSH(params, "s", std::string, query->organizationId.getValue(""));
    }

    // Four-table JOIN + CROSS JOIN denominator subquery to calculate percentage
    // JOIN chain: adm_encounter -> adm_encounter_diagnosis -> cln_condition -> cln_condition_definition
    // Uses COUNT(*) (encounters) instead of COUNT(DISTINCT patient_id) so percentages sum to 100%
    std::string sql =
        "SELECT cd.name AS name, "
        "       COUNT(*) AS patientCount, "
        "       IFNULL(ROUND(COUNT(*) * 100.0 / NULLIF(total.cnt, 0), 2), 0.0) AS percentage "
        "FROM adm_encounter e "
        "JOIN adm_encounter_diagnosis ed ON ed.encounter_id = e.id "
        "JOIN cln_condition c ON c.id = ed.condition_id "
        "JOIN cln_condition_definition cd ON cd.id = c.definition_id "
        "CROSS JOIN ("
        "    SELECT COUNT(*) AS cnt "
        "    FROM adm_encounter e2 "
        + subCond.str() +
        ") total "
        + outerCond.str() +
        " GROUP BY cd.name ORDER BY patientCount DESC ";

    // Pagination: use getValue for safe defaults
    auto pageIndex = query->pageIndex.getValue(1);
    auto pageSize = query->pageSize.getValue(10);
    sql += " LIMIT " + std::to_string((pageIndex - 1) * pageSize) + "," + std::to_string(pageSize);

    return sqlSession->executeQuery<DiseaseDistributionDO>(sql, DiseaseDistributionMapper(), params);
}
