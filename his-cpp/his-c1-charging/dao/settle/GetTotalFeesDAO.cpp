#include "stdafx.h"
#include "GetTotalFeesDAO.h"
#include "GetTotalFeesMapper.h"

// 构建动态 WHERE 条件：只返回 AND 条件片段
inline std::string GetTotalFeesDAO::queryConditionBuilder(const GetTotalFeesQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream sqlCondition; // 统一变量名
    bool firstCond = true;

    // 就诊ID 筛选 (核心)
    if (query->encounter_id && !query->encounter_id->empty())
    {
        if (!firstCond) sqlCondition << " AND ";
        sqlCondition << " enc.id = ? ";
        SQLPARAMS_PUSH(params, "s", std::string, *query->encounter_id);
        firstCond = false;
    }
    // 住院号筛选
    if (query->inpatient_no && !query->inpatient_no->empty())
    {
        if (!firstCond) sqlCondition << " AND ";
        sqlCondition << " enc.bus_no = ? ";
        SQLPARAMS_PUSH(params, "s", std::string, *query->inpatient_no);
        firstCond = false;
    }

    // 如果存在条件，拼接 WHERE
    if (!sqlCondition.str().empty())
    {
        return " WHERE " + sqlCondition.str();
    }
    return "";
}

// 统计总数 (通常费用合计只有一条，这里返回 1 或 0)
uint64_t GetTotalFeesDAO::count(const GetTotalFeesQuery::Wrapper& query)
{
    SqlParams params;
    std::ostringstream sql;

    sql << " SELECT COUNT(1) FROM adm_encounter enc ";
    sql << queryConditionBuilder(query, params);

    return sqlSession->executeQueryNumerical(sql.str(), params);
}

// 查询患者费用合计汇总
std::list<GetTotalFeesDO> GetTotalFeesDAO::selectWithPage(const GetTotalFeesQuery::Wrapper& query)
{
    SqlParams params;
    std::ostringstream sql;

    sql << " SELECT enc.id AS encounter_id, "
        // 住院总费用：与费用明细同口径，取 adm_charge_item 已结+未结之和
        << "(SELECT COALESCE(SUM(ci.total_price),0) FROM adm_charge_item ci "
        << "WHERE ci.encounter_id=enc.id AND ci.is_deleted=0 "
        << "AND ci.status_enum IN ('DRAFT','UNSETTLED','SETTLED')) AS total_hospital_fee,"
        // 医保支付金额：医保数据未接入，暂置 0
        << "0 AS insurance_pay_fee,"
        // 个人自付金额：无医保场景下等于总费用
        << "(SELECT COALESCE(SUM(ci.total_price),0) FROM adm_charge_item ci "
        << "WHERE ci.encounter_id=enc.id AND ci.is_deleted=0 "
        << "AND ci.status_enum IN ('DRAFT','UNSETTLED','SETTLED')) AS personal_pay_fee,"
        // 预交押金总额：住院预交金成功支付流水之和
        << "(SELECT COALESCE(SUM(fpr.tendered_amount),0) FROM fin_payment_reconciliation fpr "
        << "WHERE fpr.encounter_id=enc.id AND fpr.is_deleted=0 "
        << "AND fpr.kind_enum='HOSPITAL_DEPOSIT' AND fpr.status_enum='SUCCESS') AS prepay_total_fee,"
        // 应补/应退金额：未结算费用之和
        << "(SELECT COALESCE(SUM(ci.total_price),0) FROM adm_charge_item ci "
        << "WHERE ci.encounter_id=enc.id AND ci.is_deleted=0 "
        << "AND ci.status_enum IN ('DRAFT','UNSETTLED')) AS final_balance_fee,"
        // 账户余额：当前预交金账户可用余额
        << "(SELECT COALESCE(acc.balance,0) FROM adm_account acc "
        << "WHERE acc.encounter_id=enc.id AND acc.is_deleted=0 AND acc.default_flag=1 LIMIT 1) AS account_balance "
        << " FROM adm_encounter enc ";
    // 拼接WHERE条件
    sql << this->queryConditionBuilder(query, params);

    return sqlSession->executeQuery<GetTotalFeesDO>(sql.str(), GetTotalFeesMapper(), params);
}
