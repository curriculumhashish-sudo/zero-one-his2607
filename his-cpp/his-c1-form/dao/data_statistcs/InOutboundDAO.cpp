#include "InOutboundDAO.h"
#include "InOutboundMapper.h"
#include "stdafx.h"

std::string queryConditionBuilder(const InOutboundQuery::Wrapper& query, SqlParams& params) {
	stringstream sqlCondition;
	sqlCondition << "WHERE 1 = 1";
	if (query->name) {
		sqlCondition << " AND name = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->name.getValue(""));
	}
	if (query->onemoney) {
		sqlCondition << " AND lot_number_cost_amount = ?";
		SQLPARAMS_PUSH(params, "i", uint32_t, query->onemoney.getValue(0));
	}
	if (query->allmoney) {
		sqlCondition << " AND lot_number_cost_total_amount = ?";
		SQLPARAMS_PUSH(params, "i", uint32_t, query->allmoney.getValue(0));
	}
	if (query->outmoney) {
		sqlCondition << " AND lot_number_out_amount = ?";
		SQLPARAMS_PUSH(params, "i", uint32_t, query->outmoney.getValue(0));
	}
	if (query->alloutmoney) {
		sqlCondition << " AND lot_number_out_total_amount = ?";
		SQLPARAMS_PUSH(params, "i", uint32_t, query->alloutmoney.getValue(0));
	}
	if (query->batch_number) {
		sqlCondition << " AND batch_number = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->batch_number.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t InOutboundDAO::count(const InOutboundQuery::Wrapper& query) {
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM doc_inventory_item_snapshot";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<InOutboundDO> InOutboundDAO::selectWithPage(
	const InOutboundQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT "
		"name, "
		"bus_no, "
		"lot_number_cost_amount, "
		"lot_number_price_amount, "
		"line_cost_total, "
		"line_price_total "
		"FROM doc_inventory_item_snapshot ";
	sql += queryConditionBuilder(query, params);
	sql +=
		" ORDER BY IFNULL(`created_at`, `updated_at`) DESC, bus_no DESC ";
	sql +=
		" LIMIT " + std::to_string((query->pageIndex - 1) * query->pageSize) + "," + std::to_string(query->pageSize);
	return sqlSession->executeQuery<InOutboundDO>(
		sql,
		InOutboundMapper(),
		params
	);
}

InOutboundDO InOutboundDAO::selectById(
	const std::string& id)
{
	std::string sql =
		"SELECT "
		"name, "
		"bus_no, "
		"lot_number_cost_amount, "
		"lot_number_price_amount, "
		"line_cost_total, "
		"line_price_total "
		"FROM doc_inventory_item_snapshot "
		"WHERE bus_no = ?";

	return sqlSession->executeQueryOne<InOutboundDO>(
		sql,
		InOutboundMapper(),
		"%s",
		id
	);
}
