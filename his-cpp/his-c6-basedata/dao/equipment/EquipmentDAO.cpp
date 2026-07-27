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
std::list<InventoryItemDO> EquipmentDAO::selectWithPage(const EquipmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT id,name,sex,age FROM inventory_item ";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 构建排序语句
	sql += " ORDER BY IFNULL(`update_time`, `create_time`) DESC, `id` DESC ";
	// 构建分页条件
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// 执行查询
	return sqlSession->executeQuery<InventoryItemDO>(sql, InventoryItemMapper(), params);
}
// 通过名字查询
std::list<InventoryItemDO> EquipmentDAO::selectByName(const string& name)
{
	string sql = "SELECT id,name,sex,age FROM inventory_item WHERE `name` LIKE CONCAT('%',?,'%')";
	return sqlSession->executeQuery<InventoryItemDO>(sql, InventoryItemMapper(), "%s", name);
}
// 通过ID查询
PtrInventoryItemDO EquipmentDAO::selectById(std::string id)
{
	string sql = "SELECT id,name,sex,age FROM sample WHERE `id`=?";
	return sqlSession->executeQueryOne<PtrInventoryItemDO>(sql, PtrInventoryItemMapper(), "%s", id);
}
