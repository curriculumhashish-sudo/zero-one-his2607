#include "stdafx.h"
#include "Pay_ManageGetPatientDetailsDAO.h"
#include "Pay_ManageGetPatientDetailsMapper.h"
#include <utility>

// Prevent ODBC/MySQL type conflict
#include "NacosClient.h"

Pay_ManageGetPatientDetailsDO Pay_ManageGetPatientDetailsDAO::selectByBusNo(
    const Pay_ManageGetPatientDetailsQuery::Wrapper& query)
{
    SqlParams params;

    std::ostringstream sql;
    sql << "SELECT"
        << " e.bus_no AS hospitalized_number,"
        << " p.name AS patient_name,"
        << " org.name AS department_name,"
        << " MAX(CASE WHEN loc.form_enum = 'WARD' THEN loc.name END) AS ward_name,"
        << " MAX(CASE WHEN loc.form_enum = 'BED' THEN loc.name END) AS bed_number,"
        << " COALESCE(MAX(ac.balance), 0) AS account_balance,"
        << " e.status_enum AS visit_status,"
        << " MAX(fc.name) AS cost_nature"
        << " FROM adm_encounter e"
        << " LEFT JOIN adm_patient p ON e.patient_id = p.id"
        << " LEFT JOIN adm_encounter_location el ON e.id = el.encounter_id AND el.end_time IS NULL"
        << " LEFT JOIN adm_location loc ON el.location_id = loc.id"
        << " LEFT JOIN adm_organization org ON e.organization_id = org.id"
        << " LEFT JOIN adm_account ac ON e.id = ac.encounter_id AND ac.default_flag = 1"
        << " LEFT JOIN fin_contract fc ON ac.contract_no = fc.bus_no"
        << " WHERE e.bus_no = ?"
        << " AND e.class_enum = 'IMP'"
        << " AND e.is_deleted = 0"
        << " GROUP BY e.id, p.name, org.name, e.status_enum"
        << " LIMIT 1";

    SQLPARAMS_PUSH(params, "s", std::string, query->hospitalized_number);

    auto resultList = sqlSession->executeQuery<Pay_ManageGetPatientDetailsDO>(
        sql.str(), Pay_ManageGetPatientDetailsMapper(), params);

    if (resultList.empty())
    {
        return Pay_ManageGetPatientDetailsDO();
    }

    return std::move(resultList.front());
}
