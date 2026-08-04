#include "stdafx.h"
#include "Pay_ManageGetPatientQueueDAO.h"
#include "Pay_ManageGetPatientQueueMapper.h"

// Prevent ODBC/MySQL type conflict
#include "NacosClient.h"

inline std::string Pay_ManageGetPatientQueueDAO::queryConditionBuilder(
    const Pay_ManageGetPatientQueueQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream sql;
    sql << " WHERE 1=1 ";

    // Filter inpatient encounters only
    sql << " AND e.class_enum = 'IMP' ";
    sql << " AND e.is_deleted = 0 ";

    // 1. Hospitalized number fuzzy search
    if (query->hospitalized_number && !query->hospitalized_number->empty())
    {
        sql << " AND e.bus_no LIKE CONCAT('%',?,'%') ";
        SQLPARAMS_PUSH(params, "s", std::string, query->hospitalized_number);
    }

    // 2. Patient name fuzzy search
    if (query->name && !query->name->empty())
    {
        sql << " AND p.name LIKE CONCAT('%',?,'%') ";
        SQLPARAMS_PUSH(params, "s", std::string, query->name);
    }

    // 3. Bed number fuzzy search
    if (query->bed_number && !query->bed_number->empty())
    {
        sql << " AND loc.name LIKE CONCAT('%',?,'%') ";
        SQLPARAMS_PUSH(params, "s", std::string, query->bed_number);
    }

    // 4. Admission date range search (exact day match)
    if (query->hospitalized_time && !query->hospitalized_time->empty())
    {
        sql << " AND DATE(e.start_time) = ? ";
        SQLPARAMS_PUSH(params, "s", std::string, query->hospitalized_time);
    }

    return sql.str();
}

uint64_t Pay_ManageGetPatientQueueDAO::count(const Pay_ManageGetPatientQueueQuery::Wrapper& query)
{
    SqlParams params;
    std::string where = queryConditionBuilder(query, params);

    std::ostringstream sql;
    sql << "SELECT COUNT(*)"
        << " FROM adm_encounter e"
        << " LEFT JOIN adm_patient p ON e.patient_id = p.id"
        << " LEFT JOIN adm_encounter_location el ON e.id = el.encounter_id AND el.end_time IS NULL"
        << " LEFT JOIN adm_location loc ON el.location_id = loc.id"
        << where;

    return sqlSession->executeQueryNumerical(sql.str(), params);
}

std::list<Pay_ManageGetPatientQueueDO> Pay_ManageGetPatientQueueDAO::selectWithPage(
    const Pay_ManageGetPatientQueueQuery::Wrapper& query)
{
    SqlParams params;
    std::string where = queryConditionBuilder(query, params);

    // Pagination: pageIndex starts from 1
    uint64_t pageIndex = query->pageIndex.getValue(1);
    uint64_t pageSize = query->pageSize.getValue(10);
    uint64_t offset = (pageIndex - 1) * pageSize;

    std::ostringstream sql;
    sql << "SELECT"
        << " e.bus_no AS hospitalized_number,"
        << " p.name AS patient_name,"
        << " p.gender_enum AS gender_code,"
        << " p.birth_date AS birth_date,"
        << " e.start_time AS admission_time,"
        << " org.name AS department_name,"
        << " loc.name AS bed_number,"
        << " fc.name AS cost_nature,"
        << " COALESCE(ac.balance, 0) AS account_balance"
        << " FROM adm_encounter e"
        << " LEFT JOIN adm_patient p ON e.patient_id = p.id"
        << " LEFT JOIN adm_encounter_location el ON e.id = el.encounter_id AND el.end_time IS NULL"
        << " LEFT JOIN adm_location loc ON el.location_id = loc.id AND loc.form_enum = 'BED'"
        << " LEFT JOIN adm_organization org ON e.organization_id = org.id"
        << " LEFT JOIN adm_account ac ON e.id = ac.encounter_id AND ac.default_flag = 1"
        << " LEFT JOIN fin_contract fc ON ac.contract_no = fc.bus_no"
        << where
        << " ORDER BY e.start_time DESC"
        << " LIMIT " << pageSize
        << " OFFSET " << offset;

    return sqlSession->executeQuery<Pay_ManageGetPatientQueueDO>(
        sql.str(), Pay_ManageGetPatientQueueMapper(), params);
}
