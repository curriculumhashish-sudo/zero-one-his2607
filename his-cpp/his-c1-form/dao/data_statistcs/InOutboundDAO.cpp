#include "InOutboundDAO.h"
#include "InOutboundMapper.h"
#include "stdafx.h"

std::string InOutboundDAO::queryConditionBuilder(const InOutboundQuery::Wrapper& query, SqlParams& params) {
	stringstream sqlCondition;
	sqlCondition << "WHERE is_deleted = 0";
	if (query->name && !query->name->empty()) {
		sqlCondition << " AND name = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->name.getValue(""));
	}
	if (query->onemoney && query->onemoney.getValue(0) > 0) {
		sqlCondition << " AND lot_number_cost_amount = ?";
		SQLPARAMS_PUSH(params, "d", double, query->onemoney.getValue(0));
	}
	if (query->allmoney && query->allmoney.getValue(0) > 0) {
		sqlCondition << " AND line_cost_total = ?";
		SQLPARAMS_PUSH(params, "d", double, query->allmoney.getValue(0));
	}
	if (query->outmoney && query->outmoney.getValue(0) > 0) {
		sqlCondition << " AND lot_number_price_amount = ?";
		SQLPARAMS_PUSH(params, "d", double, query->outmoney.getValue(0));
	}
	if (query->alloutmoney && query->alloutmoney.getValue(0) > 0) {
		sqlCondition << " AND line_price_total = ?";
		SQLPARAMS_PUSH(params, "d", double, query->alloutmoney.getValue(0));
	}
	if (query->batch_number && !query->batch_number->empty()) {
		sqlCondition << " AND lot_number = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->batch_number.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t InOutboundDAO::count(const InOutboundQuery::Wrapper& query) {
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM doc_inventory_item_snapshot ";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<InOutboundDO> InOutboundDAO::selectWithPage(
	const InOutboundQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT "
		"id, "
		"name, "
		"bus_no, "
		"lot_number, "
		"lot_number_cost_amount, "
		"lot_number_price_amount, "
		"line_cost_total, "
		"line_price_total "
		"FROM doc_inventory_item_snapshot ";
	sql += queryConditionBuilder(query, params);
	sql +=
		" ORDER BY IFNULL(`updated_at`, `created_at`) DESC, bus_no DESC ";
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
		"id, "
		"name, "
		"bus_no, "
		"lot_number, "
		"lot_number_cost_amount, "
		"lot_number_price_amount, "
		"line_cost_total, "
		"line_price_total "
		"FROM doc_inventory_item_snapshot "
		"WHERE id = ? AND is_deleted = 0";

	return sqlSession->executeQueryOne<InOutboundDO>(
		sql,
		InOutboundMapper(),
		"%s",
		id
	);
}
