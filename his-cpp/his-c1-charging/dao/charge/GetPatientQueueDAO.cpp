#include "stdafx.h"
#include "GetPatientQueueDAO.h"
#include "GetPatientQueueMapper.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

inline std::string GetPatientQueueDAO::queryConditionBuilder(
    const GetPatientQueueQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream sql;
    sql << " WHERE 1=1 ";

    // 1. 时间范围：对 adm_encounter.start_time 做范围查询
    if (query->startDate && !query->startDate->empty())
    {
        sql << " AND e.start_time >= ? ";
        SQLPARAMS_PUSH(params, "s", std::string, query->startDate);
    }
    if (query->endDate && !query->endDate->empty())
    {
        sql << " AND e.start_time <= ? ";
        SQLPARAMS_PUSH(params, "s", std::string, query->endDate);
    }

    // 2. 关键字模糊搜索：就诊流水号 / 患者姓名 / 身份证 / 手机号
    if (query->keyword && !query->keyword->empty())
    {
        sql << " AND ( e.bus_no LIKE CONCAT('%',?,'%')"
            << " OR p.name LIKE CONCAT('%',?,'%')"
            << " OR p.id_card LIKE CONCAT('%',?,'%')"
            << " OR p.phone LIKE CONCAT('%',?,'%') )";
        // keyword 在四个字段上做模糊匹配，需要绑定 4 次
        for (int i = 0; i < 4; i++)
        {
            SQLPARAMS_PUSH(params, "s", std::string, query->keyword);
        }
    }

    // 3. 页签过滤：按收费项状态聚合过滤
    if (query->queueTabCode && !query->queueTabCode->empty())
    {
        std::string tab = query->queueTabCode;
        if (tab == "PENDING")
        {
            // 至少有一条待收费项的 encounter
            sql << " AND EXISTS ("
                << " SELECT 1 FROM adm_charge_item ci"
                << " WHERE ci.encounter_id = e.id"
                << " AND ci.status_enum = 'PENDING'"
                << " )";
        }
        else if (tab == "SETTLED")
        {
            // 所有收费项均为已结算状态的 encounter
            sql << " AND EXISTS ("
                << " SELECT 1 FROM adm_charge_item ci"
                << " WHERE ci.encounter_id = e.id"
                << " AND ci.status_enum = 'SETTLED'"
                << " )"
                << " AND NOT EXISTS ("
                << " SELECT 1 FROM adm_charge_item ci"
                << " WHERE ci.encounter_id = e.id"
                << " AND ci.status_enum != 'SETTLED'"
                << " )";
        }
        // ALL: 不追加任何条件
    }

    return sql.str();
}

uint64_t GetPatientQueueDAO::count(const GetPatientQueueQuery::Wrapper& query)
{
    SqlParams params;
    std::string where = queryConditionBuilder(query, params);

    std::ostringstream sql;
    sql << "SELECT COUNT(*)"
        << " FROM adm_encounter e"
        << " LEFT JOIN adm_patient p ON e.patient_id = p.id"
        << where;

    return sqlSession->executeQueryNumerical(sql.str(), params);
}

std::list<GetPatientQueueDO> GetPatientQueueDAO::selectWithPage(
    const GetPatientQueueQuery::Wrapper& query)
{
    SqlParams params;
    std::string where = queryConditionBuilder(query, params);

    // 计算分页：pageIndex 从 1 开始
    uint64_t pageIndex = 1;
    uint64_t pageSize = 10;
    if (query->pageIndex)
        pageIndex = query->pageIndex;
    if (query->pageSize)
        pageSize = query->pageSize;
    uint64_t offset = (pageIndex - 1) * pageSize;

    std::ostringstream sql;
    sql << "SELECT"
        << " e.id AS encounter_id,"
        << " e.bus_no AS encounter_bus_no,"
        << " e.start_time AS start_time,"
        << " p.id AS patient_id,"
        << " p.name AS patient_name,"
        << " p.gender_enum AS gender_code,"
        << " p.birth_date AS birth_date,"
        << " org.name AS organization_name,"
        << " doc.name AS doctor_name,"
        << " COALESCE(amt.total_amount, 0) AS total_amount,"
        << " COALESCE(pay.paid_amount, 0) AS paid_amount,"
        << " pay.payment_id AS payment_id"
        << " FROM adm_encounter e"
        << " LEFT JOIN adm_patient p ON e.patient_id = p.id"
        << " LEFT JOIN adm_organization org ON e.organization_id = org.id"
        << " LEFT JOIN adm_encounter_participant ep"
        << "   ON e.id = ep.encounter_id AND ep.role_code = 'DOCTOR'"
        << " LEFT JOIN adm_practitioner doc ON ep.practitioner_id = doc.id"
        << " LEFT JOIN ("
        << "   SELECT encounter_id, SUM(total_price) AS total_amount"
        << "   FROM adm_charge_item"
        << "   GROUP BY encounter_id"
        << " ) amt ON e.id = amt.encounter_id"
        << " LEFT JOIN ("
        << "   SELECT encounter_id,"
        << "     MAX(id) AS payment_id,"
        << "     SUM(CASE WHEN status_enum = 'SUCCESS' THEN tendered_amount ELSE 0 END) AS paid_amount"
        << "   FROM fin_payment_reconciliation"
        << "   GROUP BY encounter_id"
        << " ) pay ON e.id = pay.encounter_id"
        << where
        << " ORDER BY e.start_time DESC"
        << " LIMIT " << pageSize
        << " OFFSET " << offset;

    return sqlSession->executeQuery<GetPatientQueueDO>(
        sql.str(), GetPatientQueueMapper(), params);
}
