#include "stdafx.h"
#include "EquipmentDAO.h"
#include "EquipmentMapper.h"

std::string EquipmentDAO::queryConditionBuilder(const EquipmentQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";
	if (query->keywords) {

		sqlCondition << " AND `name`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));

	}
	if (query->classification) {

		sqlCondition << " AND `name`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->classification.getValue(""));

	}
	return sqlCondition.str();
}
// 统计数据条数
uint64_t EquipmentDAO::count(const EquipmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM sample ";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}
// 列表分页
std::list<WkfInventoryItemDO> EquipmentDAO::selectWithPage(const EquipmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT * FROM wkf_inventory_item ";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 构建排序语句
	sql += " ORDER BY IFNULL(`update_at`, `create_at`) DESC, `id` DESC ";
	// 构建分页条件
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// 执行查询
	return sqlSession->executeQuery<WkfInventoryItemDO>(sql, WkfInventoryItemMapper(), params);
}
// 通过名字查询
std::list<WkfInventoryItemDO> EquipmentDAO::selectByName(const string& name)
{
	string sql = "SELECT * FROM wkf_inventory_item WHERE `name` LIKE CONCAT('%',?,'%')";
	return sqlSession->executeQuery<WkfInventoryItemDO>(sql, WkfInventoryItemMapper(), "%s", name);
}
// 通过ID查询
PtrWkfInventoryItemDO EquipmentDAO::selectById(std::string id)
{
	string sql = "SELECT * FROM wkf_inventory_item WHERE `id`=?";
	return sqlSession->executeQueryOne<PtrWkfInventoryItemDO>(sql, PtrWkfInventoryItemMapper(), "%s", id);
}
