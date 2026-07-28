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
#include "WkfActivityDefinitionMapper.h"
#include "WkfActivityDefinitionChildrenMapper.h"

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