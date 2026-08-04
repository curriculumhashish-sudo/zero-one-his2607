#include "stdafx.h"
#include "Pay_ManageGetPatientPayRecordsDAO.h"
#include "Pay_ManageGetPatientPayRecordsMapper.h"

// Prevent ODBC/MySQL type conflict
#include "NacosClient.h"

uint64_t Pay_ManageGetPatientPayRecordsDAO::count(const Pay_ManageGetPatientPayRecordsQuery::Wrapper& query)
{
    SqlParams params;

    std::ostringstream sql;
    sql << "SELECT COUNT(*)"
        << " FROM fin_payment_reconciliation fpr"
        << " INNER JOIN adm_encounter e ON fpr.encounter_id = e.id"
        << " WHERE e.bus_no = ?"
        << " AND e.class_enum = 'IMP'"
        << " AND e.is_deleted = 0"
        << " AND fpr.is_deleted = 0";

    SQLPARAMS_PUSH(params, "s", std::string, query->hospitalized_number);

    return sqlSession->executeQueryNumerical(sql.str(), params);
}

std::list<Pay_ManageGetPatientPayRecordsDO> Pay_ManageGetPatientPayRecordsDAO::selectWithPage(
    const Pay_ManageGetPatientPayRecordsQuery::Wrapper& query)
{
    SqlParams params;

    // Pagination: pageIndex starts from 1
    uint64_t pageIndex = query->pageIndex.getValue(1);
    uint64_t pageSize = query->pageSize.getValue(10);
    uint64_t offset = (pageIndex - 1) * pageSize;

    std::ostringstream sql;
    sql << "SELECT"
        << " fpr.bill_date AS pay_time,"
        << " fpr.kind_enum AS service_type,"
        << " COALESCE(fpr.display_amount, 0) AS cost,"
        << " fpd.pay_enum AS pay_type,"
        << " COALESCE(wu.name, wu.user_name) AS operateman,"
        << " ai.bill_no AS bill_number,"
        << " fpr.status_enum AS status"
        << " FROM fin_payment_reconciliation fpr"
        << " INNER JOIN adm_encounter e ON fpr.encounter_id = e.id"
        << " LEFT JOIN fin_payment_rec_detail fpd ON fpr.id = fpd.reconciliation_id"
        << " LEFT JOIN adm_invoice ai ON fpr.id = ai.reconciliation_id"
        << " LEFT JOIN whale_users wu ON fpr.enterer_id = wu.id"
        << " WHERE e.bus_no = ?"
        << " AND e.class_enum = 'IMP'"
        << " AND e.is_deleted = 0"
        << " AND fpr.is_deleted = 0"
        << " ORDER BY fpr.bill_date DESC"
        << " LIMIT " << pageSize
        << " OFFSET " << offset;

    SQLPARAMS_PUSH(params, "s", std::string, query->hospitalized_number);

    return sqlSession->executeQuery<Pay_ManageGetPatientPayRecordsDO>(
        sql.str(), Pay_ManageGetPatientPayRecordsMapper(), params);
}
