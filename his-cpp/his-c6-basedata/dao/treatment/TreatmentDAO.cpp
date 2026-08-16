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
#include "TreatmentMapper.h"
// ????????????????????? WkfActivityDefinitionMapper?????? SELECT *??
static const char* TREATMENT_FULL_COLUMNS =
	"id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by,version,"
	"category_code,bus_no,name,py_str,wb_str,package_unit_code,type_enum,status_enum,description_text,"
	"body_site_code,specimen_code,org_id,location_id,rule_id,pricing_flag,yb_flag,yb_no,yb_match_flag,"
	"chrgitm_lv,children_json,children_flag,activity_version,merchandise_name,merchandise_py_str,"
	"merchandise_wb_str,def_encounter_unit_code,def_inhospital_unit_code";

// ???????????????????? WkfActivityDefinitionChildrenMapper??
static const char* TREATMENT_CHILD_FULL_COLUMNS =
	"id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by,version,"
	"activity_id,children_id,children_num,children_name";

std::string TreatmentDAO::queryConditionBuilder(const TreatmentQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE w.is_deleted=0";
	// ???????????????????????
	if (query->keywords) {
		sqlCondition << " AND (`name` LIKE CONCAT('%',?,'%') OR `bus_no` LIKE CONCAT('%',?,'%'))";
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
	}
	// ???????
	if (query->classification) {
		sqlCondition << " AND w.`category_code`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->classification.getValue(""));
	}
	return sqlCondition.str();
}

// 构建当前租户下已启用诊疗的关键字与分类筛选条件。
std::string TreatmentDAO::selectConditionBuilder(
	const TreatmentOptionsQuery::Wrapper& query,
	const std::string& tenantId,
	SqlParams& params)
{
	std::stringstream condition;
	condition << " WHERE d.`is_deleted`=0 AND d.`tenant_id`=?"
		" AND d.`status_enum`='ACTIVE'";
	SQLPARAMS_PUSH(params, "s", std::string, tenantId);

	std::string keywords = query->keyword.getValue("");
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

	const std::string categoryCode = query->categoryCode.getValue("");
	if (!categoryCode.empty())
	{
		condition << " AND d.`category_code`=?";
		SQLPARAMS_PUSH(params, "s", std::string, categoryCode);
	}
	return condition.str();
}

// 查询当前登录用户唯一有效的租户。
std::string TreatmentDAO::selectCurrentTenantId(const std::string& userId)
{
	if (userId.empty())
	{
		return "";
	}
	if (!sqlSession || !sqlSession->getConnection())
	{
		throw std::runtime_error("database connection is unavailable");
	}

	// 优先读取最近登录会话租户；无会话时仅接受唯一有效租户。
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

// 统计符合选择条件的诊疗数量。
uint64_t TreatmentDAO::countSelect(
	const TreatmentOptionsQuery::Wrapper& query,
	const std::string& tenantId)
{
	// 复用相同查询条件统计分页总数。
	SqlParams params;
	std::string sql =
		"SELECT COUNT(d.`id`) FROM `wkf_activity_definition` d";
	sql += selectConditionBuilder(query, tenantId, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

// 分页查询诊疗，并联表补充分类及单位字典文本。
std::list<TreatmentSelectBO> TreatmentDAO::selectSelectWithPage(
	const TreatmentOptionsQuery::Wrapper& query,
	const std::string& tenantId)
{
	SqlParams params;
	// 显式列出返回字段，避免使用 SELECT *。
	std::string sql =
		"SELECT d.`id`,d.`bus_no`,d.`name`,d.`category_code`,"
		"COALESCE(category_concept.`display`,''),"
		"d.`package_unit_code`,COALESCE(package_unit.`display`,''),"
		"d.`def_encounter_unit_code`,COALESCE(encounter_unit.`display`,''),"
		"d.`version` "
		"FROM `wkf_activity_definition` d "
		"LEFT JOIN `ter_code_system` category_system "
		"ON category_system.`id`=("
		"SELECT cs.`id` FROM `ter_code_system` cs "
		"WHERE cs.`name`='activity_category_code' "
		"AND cs.`is_deleted`=0 AND cs.`status` IN ('ACTIVE','2') "
		"AND (cs.`tenant_id`=d.`tenant_id` OR cs.`tenant_id` IS NULL) "
		"ORDER BY (cs.`tenant_id`=d.`tenant_id`) DESC,cs.`id` DESC LIMIT 1) "
		"LEFT JOIN `ter_concept` category_concept "
		"ON category_concept.`id`=("
		"SELECT concept.`id` FROM `ter_concept` concept "
		"WHERE concept.`code_system_id`=category_system.`id` "
		"AND concept.`code`=d.`category_code` "
		"AND concept.`is_deleted`=0 AND concept.`is_active`=1 "
		"AND (concept.`tenant_id`=d.`tenant_id` OR concept.`tenant_id` IS NULL) "
		"ORDER BY (concept.`tenant_id`=d.`tenant_id`) DESC,concept.`id` DESC LIMIT 1) "
		"LEFT JOIN `ter_code_system` unit_system "
		"ON unit_system.`id`=("
		"SELECT cs.`id` FROM `ter_code_system` cs "
		"WHERE cs.`name`='unit_code' "
		"AND cs.`is_deleted`=0 AND cs.`status` IN ('ACTIVE','2') "
		"AND (cs.`tenant_id`=d.`tenant_id` OR cs.`tenant_id` IS NULL) "
		"ORDER BY (cs.`tenant_id`=d.`tenant_id`) DESC,cs.`id` DESC LIMIT 1) "
		"LEFT JOIN `ter_concept` package_unit "
		"ON package_unit.`id`=("
		"SELECT concept.`id` FROM `ter_concept` concept "
		"WHERE concept.`code_system_id`=unit_system.`id` "
		"AND concept.`code`=d.`package_unit_code` "
		"AND concept.`is_deleted`=0 AND concept.`is_active`=1 "
		"AND (concept.`tenant_id`=d.`tenant_id` OR concept.`tenant_id` IS NULL) "
		"ORDER BY (concept.`tenant_id`=d.`tenant_id`) DESC,concept.`id` DESC LIMIT 1) "
		"LEFT JOIN `ter_concept` encounter_unit "
		"ON encounter_unit.`id`=("
		"SELECT concept.`id` FROM `ter_concept` concept "
		"WHERE concept.`code_system_id`=unit_system.`id` "
		"AND concept.`code`=d.`def_encounter_unit_code` "
		"AND concept.`is_deleted`=0 AND concept.`is_active`=1 "
		"AND (concept.`tenant_id`=d.`tenant_id` OR concept.`tenant_id` IS NULL) "
		"ORDER BY (concept.`tenant_id`=d.`tenant_id`) DESC,concept.`id` DESC LIMIT 1)";
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
	string sql = "SELECT COUNT(*) FROM wkf_activity_definition w JOIN adm_charge_item_definition a";
	// ???????????
	sql += queryConditionBuilder(query, params);
	// ??????
	return sqlSession->executeQueryNumerical(sql, params);
}
const std::string Cols = R"(
w.id,w.tenant_id,w.created_at,w.updated_at,w.created_by,w.updated_by,
w.is_deleted,w.deleted_at,w.deleted_by,w.version,w.category_code,w.bus_no,w.name,w.py_str,w.wb_str,
w.package_unit_code,w.type_enum,w.status_enum,w.description_text,w.body_site_code,w.specimen_code,
w.org_id,w.location_id,w.rule_id,w.pricing_flag,w.yb_flag,w.yb_no,w.yb_match_flag,w.chrgitm_lv,
w.children_json,w.children_flag,w.activity_version,w.merchandise_name,w.merchandise_py_str,w.merchandise_wb_str,
w.def_encounter_unit_code,w.def_inhospital_unit_code,
a.limit_price
)";
std::list<TreatmentDO> TreatmentDAO::selectWithPage(const TreatmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT ";
	sql += Cols;
	sql += " FROM wkf_activity_definition w LEFT JOIN ( "
		"SELECT instance_id,tenant_id,"
		"MAX(CASE price_type_code WHEN 'PURCHASE' THEN price_value END) AS purchase_price,"
		"MAX(CASE price_type_code WHEN 'SALE' THEN price_value END) AS sale_price,"
		"MAX(CASE price_type_code WHEN 'LIMIT' THEN price_value END) AS limit_price,"
		"yb_type FROM adm_charge_item_definition "
		"GROUP BY instance_id,tenant_id, yb_type "
		") a ON w.id = a.instance_id AND w.tenant_id = a.tenant_id ";
	// ???????????
	sql += queryConditionBuilder(query, params);
	// ???????????
	sql += " ORDER BY IFNULL(w.`updated_at`, w.`created_at`) DESC, `id` DESC ";
	// ???????????
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// ??????
	return sqlSession->executeQuery<TreatmentDO>(sql, TreatmentMapper(), params);
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

int32_t TreatmentDAO::updateStatusById(const std::string& id, const std::string& statusEnum,
	const std::string& updatedBy, int32_t version) {
	std::string sql= "UPDATE wkf_activity_definition "
		"SET status_enum=?, updated_at=NOW(), updated_by=?, version=version+1 "
		"WHERE id=? AND version=? AND is_deleted=0";
	return sqlSession->executeUpdate(sql, "%s%s%s%i",
		statusEnum, updatedBy, id, version);
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

// Price columns for adm_charge_item_definition
static const char* CHARGE_FULL_COLUMNS =
	"id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by,version,"
	"instance_table,instance_id,charge_name,status_enum,org_id,type_code,yb_type,price_type_code,"
	"lot_no,price_value,priority";

std::list<AdmChargeItemDefinitionDO> TreatmentDAO::selectPricesByInstanceId(const std::string& instanceId)
{
	string sql = "SELECT ";
	sql += CHARGE_FULL_COLUMNS;
	sql += " FROM adm_charge_item_definition "
		"WHERE instance_table='wkf_activity_definition' AND instance_id=? AND is_deleted=0 "
		"AND price_type_code IN ('PURCHASE','SALE','LIMIT')";
	return sqlSession->executeQuery<AdmChargeItemDefinitionDO>(
		sql, AdmChargeItemDefinitionMapper(), "%s", instanceId);
}

int32_t TreatmentDAO::softDeletePricesByInstanceId(const std::string& instanceId, const std::string& deletedBy, const std::string& deletedAt)
{
	string sql = "UPDATE adm_charge_item_definition SET is_deleted=1, deleted_by=?, deleted_at=? "
		"WHERE instance_table='wkf_activity_definition' AND instance_id=? AND is_deleted=0 "
		"AND price_type_code IN ('PURCHASE','SALE','LIMIT')";
	return sqlSession->executeUpdate(sql, "%s%s%s", deletedBy, deletedAt, instanceId);
}

int32_t TreatmentDAO::removeData(std::list<std::string>& ids)
{
	if (ids.empty()) return 0;

	std::string sql = "UPDATE wkf_activity_definition w LEFT JOIN adm_charge_item_definition a SET "
		"w.is_deleted = 1, w.deleted_at = NOW(),a.is_deleted=1,a.deleted_at = NOW() WHERE w.id IN(";
	SqlParams params;
	bool first = true;

	for (const auto& item : ids)
	{
		if (!first) sql += ",";
		first = false;
		sql += "?";
		SQLPARAMS_PUSH(params, "s", std::string, item);
	}
	sql += ") AND w.is_deleted=0";

	return sqlSession->executeUpdate(sql, params);
}

uint64_t TreatmentDAO::nextSequenceValue(const std::string& seqKey)
{
	if (seqKey.empty())
	{
		return 0;
	}

	SqlParams keyParams;
	SQLPARAMS_PUSH(keyParams, "s", std::string, seqKey);

	// Query whether the sequence key already exists.
	const uint64_t count = sqlSession->executeQueryNumerical(
		"SELECT COUNT(*) FROM `whale_sequence` WHERE `seq_key`=?", keyParams);
	if (count == 0)
	{
		// First use of this key: insert starting value 1.
		const int rows = sqlSession->executeUpdate(
			"INSERT INTO `whale_sequence`(`seq_key`, `current_value`, `updated_at`) VALUES(?, 1, NOW(6))",
			"%s", seqKey);
		return rows == 1 ? 1 : 0;
	}

	// Query current value with row lock, then increment and write back.
	const uint64_t current = sqlSession->executeQueryNumerical(
		"SELECT `current_value` FROM `whale_sequence` WHERE `seq_key`=? FOR UPDATE", keyParams);
	const uint64_t next = current + 1;
	SqlParams updateParams;
	SQLPARAMS_PUSH(updateParams, "ull", uint64_t, next);
	SQLPARAMS_PUSH(updateParams, "s", std::string, seqKey);
	const int rows = sqlSession->executeUpdate(
		"UPDATE `whale_sequence` SET `current_value`=?, `updated_at`=NOW(6) WHERE `seq_key`=?",
		updateParams);
	return rows == 1 ? next : 0;
}
