/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#include "BedDAO.h"
#include "BedMapper.h"

std::string BedDAO::queryConditionBuilder(const BedQuery::Wrapper& query, SqlParams& params)
{
    std::stringstream sql;
    // Only beds
    sql << " WHERE l.form_enum = 'BED'";
    // Optional status filter
    if (query->status_enum) {
        sql << " AND l.status_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->status_enum.getValue(""));
    }
    // Optional ward filter
    if (query->parent_id) {
        sql << " AND l.parent_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->parent_id.getValue(""));
    }
    // Optional department filter
    if (query->organization_id) {
        sql << " AND l.organization_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->organization_id.getValue(""));
    }
    // Free = no valid reservation on that date
    sql << " AND NOT EXISTS (SELECT 1 FROM bkg_bed_reservation_ext r"
           " WHERE r.location_id = l.id AND r.is_deleted = 0";
    if (query->intended_admission_date) {
        sql << " AND r.intended_admission_date = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->intended_admission_date.getValue(""));
    }
    sql << ")";
    return sql.str();
}

uint64_t BedDAO::countFree(const BedQuery::Wrapper& query)
{
    SqlParams params;
    // COUNT(DISTINCT l.id): one bed may be stored as duplicate rows in adm_location,
    // we must not count it twice.
    std::string sql = "SELECT COUNT(DISTINCT l.id) FROM adm_location l ";
    sql += queryConditionBuilder(query, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

std::list<adm_locationDO> BedDAO::selectFreeWithPage(const BedQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT l.id, l.name, l.parent_id, l.form_enum, l.status_enum, l.organization_id FROM adm_location l ";
    sql += queryConditionBuilder(query, params);
    // GROUP BY l.id: collapse duplicate bed rows (same id stored twice in adm_location)
    // so the free-bed list shows each bed only once.
    sql += " GROUP BY l.id";
    sql += " ORDER BY l.id DESC ";
    sql += " LIMIT " + std::to_string((query->pageIndex - 1) * query->pageSize) + "," + std::to_string(query->pageSize);
    return sqlSession->executeQuery<adm_locationDO>(sql, BedMapper(), params);
}

int BedDAO::reserve(const bkg_bed_reservation_extDO& data)
{
    // Unique index (location_id, intended_admission_date) on bkg_bed_reservation_ext
    // guarantees no duplicate reservation on the same bed + date. A duplicate insert
    // throws, which the service layer catches and reports as "already reserved".
    return insert(data);
}
