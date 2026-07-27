#include "stdafx.h"
#include "GetSettleHistoryDAO.h"
#include "GetSettleHistoryMapper.h"

std::string GetSettleHistoryDAO::queryConditionBuilder(const GetSettleHistoryQuery::Wrapper& query, SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE fpr.is_deleted=0"
		<< " AND fpr.kind_enum='SETTLEMENT'"
		<< " AND fpr.patient_id=?";
	SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));

	const std::string encounterId = query->encounterId.getValue("");
	if (!encounterId.empty())
	{
		sqlCondition << " AND fpr.encounter_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	}

	const std::string statusEnum = query->statusEnum.getValue("");
	if (!statusEnum.empty() && statusEnum != "ALL")
	{
		sqlCondition << " AND fpr.status_enum=?";
		SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
	}
	return sqlCondition.str();
}

uint64_t GetSettleHistoryDAO::count(const GetSettleHistoryQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(fpr.id) FROM fin_payment_reconciliation fpr";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PtrGetSettleHistoryDO> GetSettleHistoryDAO::selectWithPage(const GetSettleHistoryQuery::Wrapper& query)
{
	std::string sql =
		"SELECT fpr.id AS settlement_id,"
		" fpr.payment_no AS payment_no,"
		" fpr.patient_id AS patient_id,"
		" p.name AS patient_name,"
		" fpr.encounter_id AS encounter_id,"
		" e.bus_no AS encounter_bus_no,"
		" fpr.kind_enum AS kind_enum,"
		" fpr.status_enum AS status_enum,"
		" fpr.display_amount AS display_amount,"
		" fpr.tendered_amount AS tendered_amount,"
		" COALESCE(fpr.returned_amount,0) AS returned_amount,"
		" fpr.bill_date AS bill_date,"
		" fpr.invoice_id AS invoice_id,"
		" (SELECT ybs.setl_id"
		" FROM fin_payment_yb_settlement ybs"
		" WHERE ybs.reconciliation_id=fpr.id"
		" AND ybs.status='CONFIRMED'"
		" AND ybs.is_deleted=0"
		" ORDER BY ybs.created_at DESC,ybs.id DESC"
		" LIMIT 1) AS setl_id"
		" FROM fin_payment_reconciliation fpr"
		" INNER JOIN adm_patient p"
		" ON p.id=fpr.patient_id AND p.is_deleted=0"
		" INNER JOIN adm_encounter e"
		" ON e.id=fpr.encounter_id AND e.is_deleted=0";

	SqlParams params;
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY fpr.bill_date DESC,fpr.id DESC LIMIT ?,?";

	const uint64_t requestedPageIndex = query->pageIndex.getValue(1);
	const uint64_t requestedPageSize = query->pageSize.getValue(10);
	const uint64_t pageIndex = requestedPageIndex == 0 ? 1 : requestedPageIndex;
	const uint64_t pageSize = requestedPageSize == 0 ? 10 : requestedPageSize;
	SQLPARAMS_PUSH(params, "ull", uint64_t, (pageIndex - 1) * pageSize);
	SQLPARAMS_PUSH(params, "ull", uint64_t, pageSize);
	return sqlSession->executeQuery<PtrGetSettleHistoryDO>(sql, GetSettleHistoryMapper(), params);
}

std::list<PtrSettleHistoryPaymentDetailDO>
GetSettleHistoryDAO::selectPaymentDetails(const std::vector<std::string>& settlementIds)
{
	if (settlementIds.empty())
	{
		return {};
	}

	std::stringstream sql;
	SqlParams params;
	sql << "SELECT d.id AS payment_detail_id,"
		<< " d.reconciliation_id AS reconciliation_id,"
		<< " d.pay_enum AS pay_enum,"
		<< " d.amount AS amount,"
		<< " COALESCE(d.return_amount,0) AS return_amount,"
		<< " d.pay_trans_no AS pay_trans_no,"
		<< " d.pay_trans_date AS pay_trans_date,"
		<< " d.result_enum AS result_enum"
		<< " FROM fin_payment_rec_detail d"
		<< " WHERE d.is_deleted=0"
		<< " AND d.reconciliation_id IN (";

	for (size_t index = 0; index < settlementIds.size(); ++index)
	{
		if (index > 0)
		{
			sql << ",";
		}
		sql << "?";
		SQLPARAMS_PUSH(params, "s", std::string, settlementIds[index]);
	}
	sql << ") ORDER BY d.reconciliation_id ASC,d.created_at ASC,d.id ASC";
	return sqlSession->executeQuery<PtrSettleHistoryPaymentDetailDO>(sql.str(), SettleHistoryPaymentDetailMapper(), params);
}
