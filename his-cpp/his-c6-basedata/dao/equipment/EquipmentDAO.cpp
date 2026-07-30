#include "stdafx.h"
#include "EquipmentDAO.h"
#include "EquipmentMapper.h"


// 1. 查询条件构建器 
std::string EquipmentDAO::queryConditionBuilder(const EquipmentQuery::Wrapper& query, SqlParams& params)
{
	std::stringstream sqlCondition;

	// 基础条件：只查询未删除的数据 (is_deleted = 0)
	sqlCondition << " WHERE wii.`is_deleted` = 0 ";

	// 条件1：关键词模糊查询 
	if (query->keywords && !query->keywords->empty()) {
		sqlCondition << " AND (`name` LIKE CONCAT('%',?,'%') OR `py_str` LIKE CONCAT('%',?,'%')) ";
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
	}

	// 条件2：器材分类精准查询
	if (query->classification && !query->classification->empty()) {
		sqlCondition << " AND `category_code` = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->classification.getValue(""));
	}

	return sqlCondition.str();
}

// ============================================================================
// 2. 统计数据条数 (供分页使用)
// ============================================================================
uint64_t EquipmentDAO::count(const EquipmentQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM wkf_inventory_item wii JOIN doc_inventory_item_snapshot diis";

	// 拼接条件
	sql += queryConditionBuilder(query, params);

	// 执行查询并返回数字
	return sqlSession->executeQueryNumerical(sql, params);
}

const std::string& listCols =
"wii.id, diis.product_bus_no, wii.name, diis.total_volume,"
"CASE wii.inventory_status_enum WHEN 'ACTIVE' THEN 1 WHEN 'INACTIVE' THEN 0 END AS inventory_status_enum,"
"diis.device_category_code, diis.manufacturer, "
//"CAST(line_price_total AS DECIMAL(18,2)) / quantity AS price"
"wii.tenant_id, wii.created_at, wii.updated_at, wii.created_by, wii.updated_by, wii.is_deleted, wii.deleted_at, wii.deleted_by, wii.version, "
"wii.category_code, wii.item_table, wii.item_id, wii.py_str, wii.wb_str, wii.supplier_id, wii.description_text, wii.unit_code, wii.quantity, "
"wii.characteristic_json, wii.lot_number, wii.production_date, wii.expiration_date, wii.validity_mon, wii.location_store_id, wii.location_id, "
"wii.trace_no, wii.packaging_levels,diis.lot_number_price_amount";
// ============================================================================
// 3. 列表分页查询
// ============================================================================
std::list<WkfInventoryItemViewDO> EquipmentDAO::selectWithPage(const EquipmentQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT  "+ listCols +" FROM wkf_inventory_item wii JOIN doc_inventory_item_snapshot diis ON wii.id=diis.inventory_item_id";

	// 拼接条件
	sql += queryConditionBuilder(query, params);

	// 拼接排序：优先按更新时间降序，其次按创建时间，最后按ID
	sql += " ORDER BY IFNULL(`updated_at`, `created_at`) DESC, `id` DESC ";

	// 拼接分页
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// 执行查询并通过 Mapper 映射结果
	return sqlSession->executeQuery<WkfInventoryItemViewDO>(sql, WkfInventoryItemMapper(), params);
}

//这个真的有必要保留吗？
// 通过名字查询
//std::list<WkfInventoryItemDO> EquipmentDAO::selectByName(const string& name)
//{
//	string sql = "SELECT * FROM wkf_inventory_item WHERE `name` LIKE CONCAT('%',?,'%')";
//	return sqlSession->executeQuery<WkfInventoryItemDO>(sql, WkfInventoryItemMapper(), "%s", name);
//}


// ============================================================================
// 4. 根据 ID 查询单条数据
// ============================================================================
PtrDocInventorySnapshotDO EquipmentDAO::selectById(std::string id)
{
	std::string sql = "SELECT * FROM doc_inventory_item_snapshot WHERE `id` = ?";
	return sqlSession->executeQueryOne<PtrDocInventorySnapshotDO>(sql, PtrDocInventorySnapshotMapper(), "%s", id);
}

// ============================================================================
// 5. 新增数据
// PtrDocInventorySnapshotDO
int EquipmentDAO::insert(const WkfInventoryItemDO& entity)
{
	std::string sql = "INSERT INTO wkf_inventory_item "
		"(`id`, `name`, `category_code`, `inventory_status_enum`, `created_by`, `created_at`, `is_deleted`, `version`, "
		"`item_table`, `item_id`, `supplier_id`, `unit_code`, `quantity`, `lot_number`, `location_id`) "
		"VALUES (?, ?, ?, ?, ?, ?, 0, 0, ?, ?, ?, ?, ?, ?, ?)";

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, entity.getId());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getName());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getCategoryCode());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getInventoryStatusEnum());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getCreatedBy());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getCreatedAt());

	SQLPARAMS_PUSH(params, "s", std::string, entity.getItemTable());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getItemId());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getSupplierId());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getUnitCode());
	SQLPARAMS_PUSH(params, "d", double, entity.getQuantity());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getLotNumber());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getLocationId());

	return sqlSession->executeUpdate(sql, params);
}

// ============================================================================
// 6. 修改数据
// ============================================================================
int EquipmentDAO::update(const WkfInventoryItemDO& entity)
{
	std::string sql = "UPDATE wkf_inventory_item "
		"SET `name` = ?, `category_code` = ?, `inventory_status_enum` = ?, `updated_by` = ?, `updated_at` = ? "
		"WHERE `id` = ? AND `is_deleted` = 0";

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, entity.getName());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getCategoryCode());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getInventoryStatusEnum());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getUpdatedBy());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getUpdatedAt());

	SQLPARAMS_PUSH(params, "s", std::string, entity.getId());

	return sqlSession->executeUpdate(sql, params);
}


// ============================================================================
// 7. 批量逻辑删除
// ============================================================================
int EquipmentDAO::softDeleteByIds(const std::list<std::string>& ids, const std::string& deletedBy, const std::string& deletedAt)
{
	if (ids.empty()) return 0;

	std::stringstream sql;
	SqlParams params;

	// 构建前缀
	sql << "UPDATE wkf_inventory_item SET `is_deleted` = 1, `deleted_by` = ?, `deleted_at` = ? WHERE `id` IN (";
	SQLPARAMS_PUSH(params, "s", std::string, deletedBy);
	SQLPARAMS_PUSH(params, "s", std::string, deletedAt);

	// 动态拼接 IN (?, ?, ?) 占位符并压入参数
	int i = 0;
	for (const auto& id : ids) {
		sql << "?";
		if (i < ids.size() - 1) sql << ","; // 如果不是最后一个，加逗号
		SQLPARAMS_PUSH(params, "s", std::string, id);
		i++;
	}
	sql << ")";

	return sqlSession->executeUpdate(sql.str(), params);
}

// ============================================================================
// 8. 批量修改状态
// ============================================================================
int EquipmentDAO::updateStatusByIds(const std::list<std::string>& ids, const std::string& statusEnum, const std::string& updatedBy, const std::string& updatedAt)
{
	if (ids.empty()) return 0;

	std::stringstream sql;
	SqlParams params;

	sql << "UPDATE wkf_inventory_item SET `inventory_status_enum` = ?, `updated_by` = ?, `updated_at` = ? WHERE `id` IN (";
	SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
	SQLPARAMS_PUSH(params, "s", std::string, updatedBy);
	SQLPARAMS_PUSH(params, "s", std::string, updatedAt);

	int i = 0;
	for (const auto& id : ids) {
		sql << "?";
		if (i < ids.size() - 1) sql << ",";
		SQLPARAMS_PUSH(params, "s", std::string, id);
		i++;
	}
	sql << ")";

	return sqlSession->executeUpdate(sql.str(), params);
}
