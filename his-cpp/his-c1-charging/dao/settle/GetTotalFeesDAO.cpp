#include "stdafx.h"
#include "GetTotalFeesDAO.h"
#include "GetTotalFeesMapper.h"

// Build dynamic WHERE clause from query filters
inline std::string GetTotalFeesDAO::queryConditionBuilder(const GetTotalFeesQuery::Wrapper& query, SqlParams& params)
{
	std::ostringstream sqlCondition;
	sqlCondition << " WHERE 1=1 ";

	// 就诊ID
	if (query->encounter_id && !query->encounter_id->empty())
	{
		sqlCondition << " AND cs.encounter_id = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->encounter_id);
	}
	// 住院号
	if (query->inpatient_no && !query->inpatient_no->empty())
	{
		sqlCondition << " AND cs.inpatient_no = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->inpatient_no);
	}
	return sqlCondition.str();
}

// Count matching rows
uint64_t GetTotalFeesDAO::count(const GetTotalFeesQuery::Wrapper& query)
{
	SqlParams params;
	std::ostringstream sql;

	sql << " SELECT COUNT(1) "
		<< " FROM fin_cashier_settlement cs "
		<< " LEFT JOIN fin_payment_rec_detail prd ON cs.encounter_id = prd.encounter_id ";
	sql << queryConditionBuilder(query, params);

	return sqlSession->executeQueryNumerical(sql.str(), params);
}

// 查询患者费用合计汇总
std::list<GetTotalFeesDO> GetTotalFeesDAO::selectWithPage(const GetTotalFeesQuery::Wrapper& query)
{
	SqlParams params;
	std::ostringstream sql;

	sql << " SELECT "
		<< " cs.encounter_id,"
		<< " COALESCE(SUM(cs.total_hospital_fee),0) AS total_hospital_fee, "
		<< " COALESCE(SUM(cs.insurance_pay_fee),0) AS insurance_pay_fee, "
		<< " COALESCE(SUM(cs.personal_pay_fee),0) AS personal_pay_fee, "
		<< " COALESCE(SUM(prd.prepay_total_fee),0) AS prepay_total_fee, "
		<< " COALESCE(SUM(cs.final_balance_fee),0) AS final_balance_fee, "
		<< " COALESCE(SUM(cs.account_balance),0) AS account_balance "
		<< " FROM fin_cashier_settlement cs "
		<< " LEFT JOIN fin_payment_rec_detail prd ON cs.encounter_id = prd.encounter_id ";

	sql << queryConditionBuilder(query, params);

	GetTotalFeesMapper mapper;
	return sqlSession->executeQuery<GetTotalFeesDO>(
		sql.str(), GetTotalFeesMapper(), params);
}

