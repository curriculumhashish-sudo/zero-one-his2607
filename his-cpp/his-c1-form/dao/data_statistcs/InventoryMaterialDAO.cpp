#include "InventoryMaterialDAO.h"
#include "InventoryMaterialMapper.h"
#include "InventoryMapper.h"
#include "stdafx.h"


// a ---- inventory_lock   b --- doc_inventory_item_snapshot
std::string InventoryMaterialDAO::queryConditionBuilder(const InventoryMaterialQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;

	sqlCondition << "WHERE a.is_deleted = 0";

	if (query->bus_no && !query->bus_no->empty())
	{
		sqlCondition << " AND a.bus_no = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->bus_no.getValue(""));
	}
	if (query->time && !query->time->empty())
	{
		sqlCondition << " AND b.expiration_date = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->time.getValue(""));
	}
	if (query->numbers && query->numbers.getValue(0) > 0)
	{
		sqlCondition << " AND b.quantity = ?";
		SQLPARAMS_PUSH(params, "d", double, query->numbers.getValue(0));
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
		"ON a.inventory_id = b.inventory_item_id "
		"AND b.is_deleted = 0 ";

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
		"ON a.inventory_id = b.inventory_item_id "
		"AND b.is_deleted = 0 ";

	sql += queryConditionBuilder(query, params);

	sql +=
		" ORDER BY IFNULL(a.`updated_at`, a.`created_at`) DESC, "
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


InventoryDO InventoryMaterialDAO::selectById(const std::string& id)
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
		"ON a.inventory_id = b.inventory_item_id "
		"AND b.is_deleted = 0 "
		"WHERE a.id = ? AND a.is_deleted = 0";

	return sqlSession->executeQueryOne<InventoryDO>(
		sql,
		InventoryMapper(),
		"%s",
		id
	);
}
