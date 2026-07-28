#include "InventoryMaterialDAO.h"
#include "InventoryMaterialMapper.h"
#include "InventoryMapper.h"
#include "stdafx.h"


// a ---- inventory_lock   b --- doc_inventory_item_snapshot
std::string queryConditionBuilder(const InventoryMaterialQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;

	sqlCondition << "WHERE 1 = 1";

	if (query->bus_no)
	{
		sqlCondition << " AND a.bus_no = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->bus_no.getValue(""));
	}
	if (query->time)
	{
		sqlCondition << " AND b.expiration_date = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->time.getValue(""));
	}
	if (query->numbers)
	{
		sqlCondition << " AND b.quantity = ?";
		SQLPARAMS_PUSH(params, "i", int, query->numbers.getValue(0));
	}
	return sqlCondition.str();
}


uint64_t InventoryMaterialDAO::count(const InventoryMaterialQuery::Wrapper& query)
{
	SqlParams params;

	std::string sql =
		"SELECT COUNT(*) "
		"FROM inventory_lock a "
		"LEFT JOIN doc_inventory_item_snapshot b "
		"ON a.bus_no = b.bus_no ";

	sql += queryConditionBuilder(query, params);

	return sqlSession->executeQueryNumerical(sql, params);
}


std::list<InventoryDO> InventoryMaterialDAO::selectWithPage(const InventoryMaterialQuery::Wrapper& query)
{
	SqlParams params;

	std::string sql =
		"SELECT "
		"a.id, "
		"a.created_by, "
		"a.updated_by, "
		"b.quantity, "
		"a.bus_no, "
		"b.name, "
		"b.production_date, "
		"b.expiration_date, "
		"b.remaining_days "
		"FROM inventory_lock a "
		"LEFT JOIN doc_inventory_item_snapshot b "
		"ON a.bus_no = b.bus_no ";

	sql += queryConditionBuilder(query, params);

	sql +=
		" ORDER BY IFNULL(a.`UpdatedAt`, a.`CreatedAt`) DESC, "
		"b.`quantity` DESC ";

	sql +=
		" LIMIT " +
		std::to_string((query->pageIndex - 1) * query->pageSize) +
		"," +
		std::to_string(query->pageSize);


	return sqlSession->executeQuery<InventoryDO>(
		sql,
		InventoryMapper(),
		params
	);
}


InventoryDO InventoryMaterialDAO::selectById(const std::string& bus_no)
{
	std::string sql =
		"SELECT "
		"a.id, "
		"a.created_by, "
		"a.updated_by, "
		"b.quantity, "
		"a.bus_no, "
		"b.name, "
		"b.production_date, "
		"b.expiration_date, "
		"b.remaining_days "
		"FROM inventory_lock a "
		"LEFT JOIN doc_inventory_item_snapshot b "
		"ON a.bus_no = b.bus_no "
		"WHERE a.bus_no = ?";

	return sqlSession->executeQueryOne<InventoryDO>(
		sql,
		InventoryMapper(),
		"%s",
		bus_no
	);
}