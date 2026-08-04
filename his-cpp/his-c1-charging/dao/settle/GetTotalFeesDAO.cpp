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
        << "(SELECT COALESCE(SUM(fee.amount),0) FROM fin_fee fee WHERE fee.encounter_id = enc.id AND fee.is_deleted=0) AS total_hospital_fee,"
        << "(SELECT COALESCE(SUM(pre.amount),0) FROM fin_prepay pre WHERE pre.encounter_id = enc.id AND pre.is_deleted=0) AS prepay_total_fee,"
        << "(SELECT COALESCE(SUM(st.amount),0) FROM fin_settle st WHERE st.encounter_id = enc.id AND st.is_deleted=0) AS final_balance_fee,"
        << "(SELECT COALESCE(SUM(pre.amount),0) - COALESCE(SUM(st.amount),0) "
        << "FROM fin_prepay pre LEFT JOIN fin_settle st ON pre.encounter_id=st.encounter_id "
        << "WHERE pre.encounter_id = enc.id AND pre.is_deleted=0) AS account_balance "
        << " FROM adm_encounter enc ";
    // 拼接WHERE条件
    sql << this->queryConditionBuilder(query, params);

    // SQL调试日志（位置正确，不在SQL字符串内）
    OATPP_LOGD("SQL_DEBUG", "【完整执行SQL】:\n%s", sql.str().c_str());

    return sqlSession->executeQuery<GetTotalFeesDO>(sql.str(), GetTotalFeesMapper(), params);
}