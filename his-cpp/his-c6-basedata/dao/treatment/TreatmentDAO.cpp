/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 14:26:52

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "TreatmentDAO.h"
#include "TreatmentSelectMapper.h"
#include "WkfActivityDefinitionMapper.h"
#include "WkfActivityDefinitionChildrenMapper.h"
#include "StringUtil.h"

// 主表完整字段列表（对齐 WkfActivityDefinitionMapper，避免 SELECT *）
static const char* TREATMENT_FULL_COLUMNS =
	"id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by,version,"
	"category_code,bus_no,name,py_str,wb_str,package_unit_code,type_enum,status_enum,description_text,"
	"body_site_code,specimen_code,org_id,location_id,rule_id,pricing_flag,yb_flag,yb_no,yb_match_flag,"
	"chrgitm_lv,children_json,children_flag,activity_version,merchandise_name,merchandise_py_str,"
	"merchandise_wb_str,def_encounter_unit_code,def_inhospital_unit_code";

// 子表完整字段列表（对齐 WkfActivityDefinitionChildrenMapper）
static const char* TREATMENT_CHILD_FULL_COLUMNS =
	"id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by,version,"
	"activity_id,children_id,children_num,children_name";

/** 将接口状态值转换为数据库枚举。 */
static const char* treatmentStatusCode(int status)
{
	switch (status)
	{
	case 1:
		return "DRAFT";
	case 2:
		return "ACTIVE";
	case 3:
		return "INACTIVE";
	case 4:
		return "DISCARDED";
	default:
		return "";
	}
}

std::string TreatmentDAO::queryConditionBuilder(const TreatmentQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE is_deleted=0";
	// 关键词：名称或业务编码模糊匹配
	if (query->keywords) {
		sqlCondition << " AND (`name` LIKE CONCAT('%',?,'%') OR `bus_no` LIKE CONCAT('%',?,'%'))";
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
	}
	// 分类过滤
	if (query->classification) {
		sqlCondition << " AND `category_code`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->classification.getValue(""));
	}
	return sqlCondition.str();
}

std::string TreatmentDAO::selectConditionBuilder(
	const TreatmentSelectQuery::Wrapper& query,
	const std::string& tenantId,
	SqlParams& params)
{
	std::stringstream condition;
	condition << " WHERE d.`is_deleted`=0 AND d.`tenant_id`=?";
	SQLPARAMS_PUSH(params, "s", std::string, tenantId);

	std::string keywords = query->keywords.getValue("");
	StringUtil::trim(&keywords);
	if (!keywords.empty())
	{
		condition << " AND (d.`name` LIKE CONCAT('%',?,'%')"
			" OR d.`bus_no` LIKE CONCAT('%',?,'%')"
			" OR d.`py_str` LIKE CONCAT('%',?,'%')"
			" OR d.`wb_str` LIKE CONCAT('%',?,'%'))";
		for (int index = 0; index < 4; ++index)
		{
			SQLPARAMS_PUSH(params, "s", std::string, keywords);
		}
	}

	if (query->status)
	{
		condition << " AND d.`status_enum`=?";
		const std::string status =
			treatmentStatusCode(query->status.getValue(0));
		SQLPARAMS_PUSH(params, "s", std::string, status);
	}
	return condition.str();
}

std::string TreatmentDAO::selectCurrentTenantId(const std::string& userId)
{
	if (userId.empty())
	{
		return "";
	}

	// 优先使用最近登录租户，仅有一个有效租户时安全回退。
	const std::string sql =
		"SELECT COALESCE("
		"(SELECT s.`tenant_id` FROM `whale_user_login_session` s "
		"INNER JOIN `whale_users` u ON u.`id`=s.`user_id` "
		"AND u.`is_deleted`=0 AND u.`is_active`=1 AND u.`is_locked_out`=0 "
		"INNER JOIN `whale_user_tenants` ut ON ut.`user_id`=s.`user_id` "
		"AND ut.`tenant_id`=s.`tenant_id` AND ut.`is_deleted`=0 "
		"INNER JOIN `whale_tenants` t ON t.`id`=s.`tenant_id` "
		"AND t.`is_deleted`=0 AND t.`is_active`=1 "
		"WHERE s.`user_id`=? AND s.`tenant_id` IS NOT NULL "
		"AND s.`tenant_id`<>'' ORDER BY s.`updated_at` DESC LIMIT 1),"
		"(SELECT CASE WHEN COUNT(DISTINCT ut.`tenant_id`)=1 "
		"THEN MIN(ut.`tenant_id`) ELSE NULL END "
		"FROM `whale_user_tenants` ut "
		"INNER JOIN `whale_users` u ON u.`id`=ut.`user_id` "
		"AND u.`is_deleted`=0 AND u.`is_active`=1 AND u.`is_locked_out`=0 "
		"INNER JOIN `whale_tenants` t ON t.`id`=ut.`tenant_id` "
		"AND t.`is_deleted`=0 AND t.`is_active`=1 "
		"WHERE ut.`user_id`=? AND ut.`is_deleted`=0)"
		")";
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, userId);
	SQLPARAMS_PUSH(params, "s", std::string, userId);
	return sqlSession->executeQueryOne<std::string>(
		sql,
		TreatmentTenantMapper(),
		params);
}

uint64_t TreatmentDAO::countSelect(
	const TreatmentSelectQuery::Wrapper& query,
	const std::string& tenantId)
{
	SqlParams params;
	std::string sql =
		"SELECT COUNT(d.`id`) FROM `wkf_activity_definition` d";
	sql += selectConditionBuilder(query, tenantId, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<TreatmentSelectBO> TreatmentDAO::selectSelectWithPage(
	const TreatmentSelectQuery::Wrapper& query,
	const std::string& tenantId)
{
	SqlParams params;
	// 联表一次返回接口需要的字段，避免 SELECT * 和逐条补查。
	std::string sql =
		"SELECT d.`id`,d.`bus_no`,d.`name`,d.`category_code`,"
		"COALESCE(category_concept.`display`,''),"
		"COALESCE(adm_org.`name`,whale_org.`display_name`,''),"
		"COALESCE(price.`retail_price`,0),'' AS specification,"
		"'' AS manufacturer_text,COALESCE(price.`item_type_code`,''),"
		"COALESCE(item_type_concept.`display`,''),"
		"d.`package_unit_code`,COALESCE(package_unit.`display`,''),"
		"d.`def_encounter_unit_code`,COALESCE(encounter_unit.`display`,''),"
		"d.`yb_no`,d.`status_enum` "
		"FROM `wkf_activity_definition` d "
		"LEFT JOIN `ter_code_system` category_system "
		"ON category_system.`name`='activity_category_code' "
		"AND category_system.`is_deleted`=0 "
		"AND (category_system.`tenant_id`=d.`tenant_id` "
		"OR category_system.`tenant_id` IS NULL) "
		"LEFT JOIN `ter_concept` category_concept "
		"ON category_concept.`code_system_id`=category_system.`id` "
		"AND category_concept.`code`=d.`category_code` "
		"AND category_concept.`is_deleted`=0 "
		"AND category_concept.`is_active`=1 "
		"AND (category_concept.`tenant_id`=d.`tenant_id` "
		"OR category_concept.`tenant_id` IS NULL) "
		"LEFT JOIN `adm_organization` adm_org "
		"ON adm_org.`id`=d.`org_id` AND adm_org.`is_deleted`=0 "
		"AND (adm_org.`tenant_id`=d.`tenant_id` "
		"OR adm_org.`tenant_id` IS NULL) "
		"LEFT JOIN `whale_organization_units` whale_org "
		"ON whale_org.`id`=d.`org_id` AND whale_org.`is_deleted`=0 "
		"AND (whale_org.`tenant_id`=d.`tenant_id` "
		"OR whale_org.`tenant_id` IS NULL) "
		"LEFT JOIN `ter_code_system` unit_system "
		"ON unit_system.`name`='unit_code' "
		"AND unit_system.`is_deleted`=0 "
		"AND (unit_system.`tenant_id`=d.`tenant_id` "
		"OR unit_system.`tenant_id` IS NULL) "
		"LEFT JOIN `ter_concept` package_unit "
		"ON package_unit.`code_system_id`=unit_system.`id` "
		"AND package_unit.`code`=d.`package_unit_code` "
		"AND package_unit.`is_deleted`=0 AND package_unit.`is_active`=1 "
		"AND (package_unit.`tenant_id`=d.`tenant_id` "
		"OR package_unit.`tenant_id` IS NULL) "
		"LEFT JOIN `ter_concept` encounter_unit "
		"ON encounter_unit.`code_system_id`=unit_system.`id` "
		"AND encounter_unit.`code`=d.`def_encounter_unit_code` "
		"AND encounter_unit.`is_deleted`=0 AND encounter_unit.`is_active`=1 "
		"AND (encounter_unit.`tenant_id`=d.`tenant_id` "
		"OR encounter_unit.`tenant_id` IS NULL) "
		"LEFT JOIN ("
		"SELECT `tenant_id`,`instance_id`,MAX(`price_value`) AS retail_price,"
		"MAX(`type_code`) AS item_type_code "
		"FROM `adm_charge_item_definition` "
		"WHERE `instance_table`='wkf_activity_definition' "
		"AND `price_type_code`='SALE' AND `status_enum`='ACTIVE' "
		"AND `is_deleted`=0 GROUP BY `tenant_id`,`instance_id`"
		") price ON price.`instance_id`=d.`id` "
		"AND price.`tenant_id`=d.`tenant_id` "
		"LEFT JOIN `ter_code_system` item_type_system "
		"ON item_type_system.`name`='fin_type_code' "
		"AND item_type_system.`is_deleted`=0 "
		"AND (item_type_system.`tenant_id`=d.`tenant_id` "
		"OR item_type_system.`tenant_id` IS NULL) "
		"LEFT JOIN `ter_concept` item_type_concept "
		"ON item_type_concept.`code_system_id`=item_type_system.`id` "
		"AND item_type_concept.`code`=price.`item_type_code` "
		"AND item_type_concept.`is_deleted`=0 "
		"AND item_type_concept.`is_active`=1 "
		"AND (item_type_concept.`tenant_id`=d.`tenant_id` "
		"OR item_type_concept.`tenant_id` IS NULL)";
	sql += selectConditionBuilder(query, tenantId, params);
	sql += " ORDER BY COALESCE(d.`updated_at`,d.`created_at`) DESC,d.`id` DESC";

	const uint64_t pageIndex = query->pageIndex.getValue(1);
	const uint64_t pageSize = query->pageSize.getValue(10);
	const uint64_t offset = (pageIndex - 1) * pageSize;
	sql += " LIMIT ?,?";
	SQLPARAMS_PUSH(params, "ull", uint64_t, offset);
	SQLPARAMS_PUSH(params, "ull", uint64_t, pageSize);
	return sqlSession->executeQuery<TreatmentSelectBO>(
		sql,
		TreatmentSelectMapper(),
		params);
}

uint64_t TreatmentDAO::count(const TreatmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM wkf_activity_definition ";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<TreatmentDO> TreatmentDAO::selectWithPage(const TreatmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT ";
	sql += TREATMENT_FULL_COLUMNS;
	sql += " FROM wkf_activity_definition ";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 构建排序语句
	sql += " ORDER BY IFNULL(`updated_at`, `created_at`) DESC, `id` DESC ";
	// 构建分页条件
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// 执行查询
	return sqlSession->executeQuery<TreatmentDO>(sql, WkfActivityDefinitionMapper(), params);
}

std::list<TreatmentDO> TreatmentDAO::selectByName(const string& name)
{
	string sql = "SELECT ";
	sql += TREATMENT_FULL_COLUMNS;
	sql += " FROM wkf_activity_definition WHERE is_deleted=0 AND `name` LIKE CONCAT('%',?,'%')";
	return sqlSession->executeQuery<TreatmentDO>(sql, WkfActivityDefinitionMapper(), "%s", name);
}

PtrTreatmentDO TreatmentDAO::selectById(std::string id)
{
	string sql = "SELECT ";
	sql += TREATMENT_FULL_COLUMNS;
	sql += " FROM wkf_activity_definition WHERE `id`=? AND is_deleted=0";
	return sqlSession->executeQueryOne<PtrTreatmentDO>(sql, PtrWkfActivityDefinitionMapper(), "%s", id);
}

std::list<TreatmentChildDO> TreatmentDAO::selectChildrenByActivityId(const std::string& activityId)
{
	string sql = "SELECT ";
	sql += TREATMENT_CHILD_FULL_COLUMNS;
	sql += " FROM wkf_activity_definition_children WHERE activity_id=? AND is_deleted=0";
	return sqlSession->executeQuery<TreatmentChildDO>(sql, WkfActivityDefinitionChildrenMapper(), "%s", activityId);
}

int32_t TreatmentDAO::softDeleteChildrenByActivityId(const std::string& activityId, const std::string& deletedBy, const std::string& deletedAt)
{
	string sql = "UPDATE wkf_activity_definition_children SET is_deleted=1, deleted_by=?, deleted_at=? "
		"WHERE activity_id=? AND is_deleted=0";
	return sqlSession->executeUpdate(sql, "%s%s%s", deletedBy, deletedAt, activityId);
}

int32_t TreatmentDAO::removeData(std::list<std::string>& ids)
{
	if (ids.empty()) return 0;

	std::string sql = "UPDATE wkf_activity_definition SET is_deleted=1, deleted_at=NOW() WHERE id IN (";
	SqlParams params;
	bool first = true;

	for (const auto& item : ids)
	{
		if (!first) sql += ",";
		first = false;
		sql += "?";
		SQLPARAMS_PUSH(params, "s", std::string, item);
	}
	sql += ") AND is_deleted=0";

	return sqlSession->executeUpdate(sql, params);
}
