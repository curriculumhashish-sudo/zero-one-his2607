#include "stdafx.h"
#include "EquipmentDAO.h"
#include "EquipmentMapper.h"

// ============================================================================
// 1. 查询条件构建器 
// ============================================================================
std::string EquipmentDAO::queryConditionBuilder(const EquipmentQuery::Wrapper& query, SqlParams& params)
{
	std::stringstream sqlCondition;

	// 基础条件：只查询未删除的数据 (is_deleted = 0)
	sqlCondition << " WHERE t.`is_deleted` = 0 ";

	// 条件 1：关键词模糊查询 (匹配名称或拼音码)
	if (query->keywords && !query->keywords->empty()) {
		sqlCondition << " AND (t.`name` LIKE CONCAT('%',?,'%') OR t.`py_str` LIKE CONCAT('%',?,'%')) ";
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
	}

	// 条件 2：器材分类精准查询
	if (query->classification && !query->classification->empty()) {
		sqlCondition << " AND t.`category_code` = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->classification.getValue(""));
	}

	// 条件 3：根据启停状态过滤
	if (query->activeFlag) {
		sqlCondition << " AND t.`active_flag` = ? ";
		SQLPARAMS_PUSH(params, "i", int, query->activeFlag.getValue(1));
	}

	return sqlCondition.str();
}

// ============================================================================
// 2. 统计数据条数 (供分页使用)
// ============================================================================
uint64_t EquipmentDAO::count(const EquipmentQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM adm_device_definition t";

	// 拼接条件
	sql += queryConditionBuilder(query, params);

	// 执行查询并返回数字
	return sqlSession->executeQueryNumerical(sql, params);
}

const std::string Cols = R"(t.id,t.tenant_id,t.created_at,t.updated_at,t.created_by,t.updated_by,
t.is_deleted,t.deleted_at,t.deleted_by,t.version,t.name,t.bus_no,t.py_str,t.wb_str,t.category_code,t.type_code,
t.merchandise_name,t.merchandise_py_str,t.merchandise_wb_str,t.purchase_unit_code,t.split_unit_code,t.dose_unit_code,
t.package_unit_code,t.stock_unit_code,t.total_volume,t.item_min_quantity,t.item_max_quantity,t.dose_unit_ratio,
t.purchase_unit_ratio,t.stock_unit_ratio,t.package_unit_ratio,t.approval_number,t.manufacturer_id,t.manufacturer_text,
t.supply_id,t.model_number,t.def_encounter_unit_code,t.def_inhospital_unit_code,t.org_id,t.location_id,t.hvcm_flag,
t.allergen_flag,t.rx_flag,t.status_enum,t.active_flag,t.description,t.jurisdiction,t.device_version,t.substance_text,
t.yb_flag,t.yb_no,t.yb_org_no,t.yb_match_flag,t.chrgitm_lv,t.yb_mapping_code,
ac.purchase_price,ac.sale_price,ac.limit_price,ac.yb_type
)";
// ============================================================================
// 3. 列表分页查询
// ============================================================================
std::list<EquipmentVeiwDO> EquipmentDAO::selectWithPage(const EquipmentQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT " + Cols + " FROM adm_device_definition t LEFT JOIN ( "
		"SELECT instance_id,tenant_id,"
		"MAX(CASE price_type_code WHEN 'PURCHASE' THEN price_value END) AS purchase_price,"
		"MAX(CASE price_type_code WHEN 'SALE' THEN price_value END) AS sale_price,"
		"MAX(CASE price_type_code WHEN 'LIMIT' THEN price_value END) AS limit_price,"
		"MAX(yb_type) AS yb_type FROM adm_charge_item_definition "
		"GROUP BY instance_id, tenant_id "
		") ac ON t.id = ac.instance_id AND t.tenant_id = ac.tenant_id ";

	// 拼接条件
	sql += queryConditionBuilder(query, params);

	// 拼接排序：优先按更新时间降序，其次按创建时间，最后按 ID
	sql += " ORDER BY IFNULL(t.`updated_at`, t.`created_at`) DESC, t.`id` DESC ";

	// 拼接分页
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	std::cout << "最终执行 SQL：" << sql << std::endl;

	// 使用新的轻量级 Mapper
	return sqlSession->executeQuery<EquipmentVeiwDO>(sql, EquipmentMapper(), params);
}

// ============================================================================
// 4. 根据 ID 查询单条数据
// ============================================================================
PtrEquipmentVeiwDO EquipmentDAO::selectById(std::string id)
{
	std::string sql = "SELECT " + Cols + " FROM adm_device_definition t LEFT JOIN ( "
		"SELECT instance_id,tenant_id,"
		"MAX(CASE price_type_code WHEN 'PURCHASE' THEN price_value END) AS purchase_price,"
		"MAX(CASE price_type_code WHEN 'SALE' THEN price_value END) AS sale_price,"
		"MAX(CASE price_type_code WHEN 'LIMIT' THEN price_value END) AS limit_price,"
		"MAX(yb_type) AS yb_type FROM adm_charge_item_definition "
		"GROUP BY instance_id, tenant_id "
		") ac ON t.id = ac.instance_id AND t.tenant_id = ac.tenant_id  WHERE t.`id` = ? AND t.`is_deleted` = 0";
	return sqlSession->executeQueryOne<std::shared_ptr<EquipmentVeiwDO>>(sql, PtrEquipmentMapper(), "%s", id);
}

// ============================================================================
// 5. 新增数据
// PtrDocInventorySnapshotDO
int EquipmentDAO::insert(const AdmDeviceDefinitionDO& entity)
{
	// 构建 SQL，包含系统字段、基础信息、单位信息、业务标记
	std::string sql = "INSERT INTO adm_device_definition ("
		// 系统字段 (10个)
		"`id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`, "
		"`is_deleted`, `version`, `status_enum`, `active_flag`, "

		// 基础信息 (11个)
		"`name`, `merchandise_name`, `category_code`, `type_code`, `manufacturer_text`, "
		"`supply_id`, `approval_number`, `total_volume`, `yb_no`, `yb_org_no`, `chrgitm_lv`, "

		// 业务及标记信息 (6个)
		"`py_str`, `wb_str`, `allergen_flag`, `yb_flag`, `hvcm_flag`, `description`, "

		// 单位与库存预警信息 (11个)
		"`purchase_unit_code`, `purchase_unit_ratio`, `stock_unit_code`, `stock_unit_ratio`, "
		"`package_unit_code`, `package_unit_ratio`, `split_unit_code`, "
		"`item_min_quantity`, `item_max_quantity`, `yb_match_flag`"
		") VALUES ("
		// 10 + 11 + 6 + 11 = 38 个字段
		"?, ?, ?, ?, ?, ?, "    // id -> updated_by
		"0, 0, ?, ?, "          // is_deleted(0), version(0), status, active
		"?, ?, ?, ?, ?, "       // name -> mfr_text
		"?, ?, ?, ?, ?, ?, "    // supply -> chrgitm_lv
		"?, ?, ?, ?, ?, ?, "    // py_str -> description
		"?, ?, ?, ?, "          // purchase -> stock ratio
		"?, ?, ?, "             // package -> split
		"?, ?, ?)";             // min/max -> yb_match_flag

	SqlParams params;

	// ================== 系统基础与状态字段 ==================
	SQLPARAMS_PUSH(params, "s", std::string, entity.getId());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getTenantId());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getCreatedAt());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getUpdatedAt());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getCreatedBy());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getUpdatedBy());
	// is_deleted 和 version 已经在 SQL 中硬编码为 0
	SQLPARAMS_PUSH(params, "s", std::string, entity.getStatusEnum()); // 必填
	SQLPARAMS_PUSH(params, "b", bool, entity.getActiveFlag()); // 必填

	// ================== 页面：基础信息模块 ==================
	SQLPARAMS_PUSH(params, "s", std::string, entity.getName());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getMerchandiseName());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getCategoryCode());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getTypeCode());        // 映射前端:财务类型
	SQLPARAMS_PUSH(params, "s", std::string, entity.getManufacturerText());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getSupplyId());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getApprovalNumber());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getTotalVolume());     // 映射前端:包装规格
	SQLPARAMS_PUSH(params, "s", std::string, entity.getYbNo());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getYbOrgNo());         // 映射前端:医药机构目录编码
	SQLPARAMS_PUSH(params, "i", int, entity.getChrgitmLv());       // 映射前端:医保等级

	// ================== 页面：业务信息模块 ==================
	SQLPARAMS_PUSH(params, "s", std::string, entity.getPyStr());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getWbStr());
	SQLPARAMS_PUSH(params, "b", bool, entity.getAllergenFlag());
	SQLPARAMS_PUSH(params, "b", bool, entity.getYbFlag());
	SQLPARAMS_PUSH(params, "b", bool, entity.getHvcmFlag());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getDescription());     // 映射前端:说明

	// ================== 页面：库存与单位信息模块 ==================
	SQLPARAMS_PUSH(params, "s", std::string, entity.getPurchaseUnitCode());
	SQLPARAMS_PUSH(params, "d", double, entity.getPurchaseUnitRatio());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getStockUnitCode());
	SQLPARAMS_PUSH(params, "d", double, entity.getStockUnitRatio());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getPackageUnitCode()); // 映射前端:包装单位
	SQLPARAMS_PUSH(params, "d", double, entity.getPackageUnitRatio());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getSplitUnitCode());   // 映射前端:拆零单位

	SQLPARAMS_PUSH(params, "d", double, entity.getItemMinQuantity()); // 映射前端:警戒线最低值
	SQLPARAMS_PUSH(params, "d", double, entity.getItemMaxQuantity()); // 映射前端:警戒线最高值
	SQLPARAMS_PUSH(params, "b", bool, entity.getYbMatchFlag());     // 映射前端:医保对码

	// 执行插入
	return sqlSession->executeUpdate(sql, params);
}

// ============================================================================
// 6. 修改数据
// ============================================================================
int EquipmentDAO::update(const AdmDeviceDefinitionDO& entity)
{
	// 构建 UPDATE SQL
	// 注意：不更新 created_at, created_by, tenant_id 等不可变字段
	std::string sql = "UPDATE adm_device_definition SET "

		// ================== 基础信息模块 ==================
		"`name` = ?, `merchandise_name` = ?, `category_code` = ?, `type_code` = ?, "
		"`manufacturer_text` = ?, `supply_id` = ?, `approval_number` = ?, `total_volume` = ?, "
		"`yb_no` = ?, `yb_org_no` = ?, `chrgitm_lv` = ?, "

		// ================== 业务及标记信息 ==================
		"`py_str` = ?, `wb_str` = ?, `allergen_flag` = ?, `yb_flag` = ?, `hvcm_flag` = ?, `description` = ?, "

		// ================== 单位与库存信息 ==================
		"`purchase_unit_code` = ?, `purchase_unit_ratio` = ?, `stock_unit_code` = ?, `stock_unit_ratio` = ?, "
		"`package_unit_code` = ?, `package_unit_ratio` = ?, `split_unit_code` = ?, "
		"`item_min_quantity` = ?, `item_max_quantity` = ?, `yb_match_flag` = ?, "

		// ================== 系统状态与审计 ==================
		"`status_enum` = ?, `active_flag` = ?, `updated_by` = ?, `updated_at` = ? "

		// ================== 限制条件 ==================
		"WHERE `id` = ? AND `is_deleted` = 0";

	SqlParams params;

	// ================== 页面：基础信息模块 ==================
	SQLPARAMS_PUSH(params, "s", std::string, entity.getName());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getMerchandiseName());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getCategoryCode());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getTypeCode());        // 映射前端:财务类型
	SQLPARAMS_PUSH(params, "s", std::string, entity.getManufacturerText());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getSupplyId());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getApprovalNumber());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getTotalVolume());     // 映射前端:包装规格
	SQLPARAMS_PUSH(params, "s", std::string, entity.getYbNo());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getYbOrgNo());         // 映射前端:医药机构目录编码
	SQLPARAMS_PUSH(params, "i", int, entity.getChrgitmLv());       // 映射前端:医保等级

	// ================== 页面：业务及标记信息 ==================
	SQLPARAMS_PUSH(params, "s", std::string, entity.getPyStr());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getWbStr());
	SQLPARAMS_PUSH(params, "b", bool, entity.getAllergenFlag());
	SQLPARAMS_PUSH(params, "b", bool, entity.getYbFlag());
	SQLPARAMS_PUSH(params, "b", bool, entity.getHvcmFlag());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getDescription());     // 映射前端:说明

	// ================== 页面：库存与单位信息 ==================
	SQLPARAMS_PUSH(params, "s", std::string, entity.getPurchaseUnitCode());
	SQLPARAMS_PUSH(params, "d", double, entity.getPurchaseUnitRatio());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getStockUnitCode());
	SQLPARAMS_PUSH(params, "d", double, entity.getStockUnitRatio());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getPackageUnitCode()); // 映射前端:包装单位
	SQLPARAMS_PUSH(params, "d", double, entity.getPackageUnitRatio());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getSplitUnitCode());   // 映射前端:拆零单位

	SQLPARAMS_PUSH(params, "d", double, entity.getItemMinQuantity()); // 映射前端:警戒线最低值
	SQLPARAMS_PUSH(params, "d", double, entity.getItemMaxQuantity()); // 映射前端:警戒线最高值
	SQLPARAMS_PUSH(params, "b", bool, entity.getYbMatchFlag());     // 映射前端:医保对码

	// ================== 系统状态与审计字段 ==================
	SQLPARAMS_PUSH(params, "s", std::string, entity.getStatusEnum());
	SQLPARAMS_PUSH(params, "b", bool, entity.getActiveFlag());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getUpdatedBy());
	SQLPARAMS_PUSH(params, "s", std::string, entity.getUpdatedAt());

	SQLPARAMS_PUSH(params, "s", std::string, entity.getId());

	// 执行更新
	return sqlSession->executeUpdate(sql, params);
}


// ============================================================================
// 7. 批量逻辑删除
// ============================================================================
int EquipmentDAO::softDeleteByIds(const std::list<std::string>& ids, const std::string& deletedBy, const std::string& deletedAt)
{
	// 如果传进来的 id 列表为空，直接返回 0，防止拼接出 WHERE id IN () 导致 SQL 语法报错
	if (ids.empty()) return 0;

	std::stringstream sql;
	SqlParams params;

	sql << "UPDATE adm_device_definition SET `is_deleted` = 1, `deleted_by` = ?, `deleted_at` = ? WHERE `id` IN (";

	// 压入固定的更新人、更新时间参数
	SQLPARAMS_PUSH(params, "s", std::string, deletedBy);
	SQLPARAMS_PUSH(params, "s", std::string, deletedAt);

	// 动态拼接 IN (?, ?, ?) 占位符，并循环压入 ID 参数
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

	sql << "UPDATE adm_device_definition SET `status_enum` = ?, `updated_by` = ?, `updated_at` = ? WHERE `id` IN (";

	// 压入状态、更新人、更新时间参数
	SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
	SQLPARAMS_PUSH(params, "s", std::string, updatedBy);
	SQLPARAMS_PUSH(params, "s", std::string, updatedAt);

	// 动态拼接 IN (?, ?, ?) 占位符，并循环压入 ID 参数
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

// ============================================================================
// 9. 批量启停用
// ============================================================================
int EquipmentDAO::updateActiveFlagByIds(const std::list<std::string>& ids, int activeFlag, const std::string& updatedBy, const std::string& updatedAt)
{
	if (ids.empty()) return 0;

	std::stringstream sql;
	SqlParams params;

	// 注意：这里更新的是 active_flag
	sql << "UPDATE adm_device_definition SET `active_flag` = ?, `updated_by` = ?, `updated_at` = ? WHERE `id` IN (";

	// 压入状态 (0或1)、更新人、更新时间参数
	SQLPARAMS_PUSH(params, "i", int, activeFlag);
	SQLPARAMS_PUSH(params, "s", std::string, updatedBy);
	SQLPARAMS_PUSH(params, "s", std::string, updatedAt);

	// 动态拼接 IN (?, ?, ?) 占位符
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

// ============================================================================
// 10. 检查名称是否重复 
// ============================================================================
bool EquipmentDAO::checkNameExists(const std::string& name, const std::string& excludeId)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM adm_device_definition WHERE `name` = ? AND `is_deleted` = 0";
	SQLPARAMS_PUSH(params, "s", std::string, name);

	// 如果传了 excludeId，说明是“修改”操作，要排除自己当前的 ID
	if (!excludeId.empty()) {
		sql += " AND `id` != ?";
		SQLPARAMS_PUSH(params, "s", std::string, excludeId);
	}

	uint64_t count = sqlSession->executeQueryNumerical(sql, params);
	return count > 0;
}