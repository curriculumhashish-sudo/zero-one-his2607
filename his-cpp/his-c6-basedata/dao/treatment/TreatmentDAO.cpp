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
#include "TreatmentMapper.h"
#include "WkfActivityDefinitionMapper.h"
#include "WkfActivityDefinitionChildrenMapper.h"

std::string TreatmentDAO::queryConditionBuilder(const TreatmentQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";
	if (query->keywords) {
		sqlCondition << " AND `keywords`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
		sqlCondition << " AND (`name` LIKE CONCAT('%',?,'%') OR `bus_no` LIKE CONCAT('%',?,'%'))";
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
		SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
	}
	if (query->classification) {
		sqlCondition << " AND `category_code`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->classification.getValue(""));
	}
	return sqlCondition.str();
}
// 统计数据条数
uint64_t TreatmentDAO::count(const TreatmentQuery::Wrapper& query)
{
	SqlParams params;
	// 构建查询条件
	string sql = "SELECT COUNT(*) FROM  wkfActivity_definition";
	// 追加查询条件
	sql += queryConditionBuilder(query, params);
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}
// 分页查询数据
std::list<TreatmentDO> TreatmentDAO::selectWithPage(const TreatmentQuery::Wrapper& query)
{
	SqlParams params;
	// 构建查询条件
	string sql = "SELECT * FROM wkf_activity_definition ";
	// 追加查询条件
	sql += queryConditionBuilder(query, params);
	// 设置排序条件
	sql += " ORDER BY IFNULL(`update_time`, `create_time`) DESC, `id` DESC ";
	// 设置分页参数
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// 执行查询
	return sqlSession->executeQuery<TreatmentDO>(sql, TreatmentMapper(), params);
}

// 删除

// 完整字段列表，对齐 WkfActivityDefinitionMapper，避免 SELECT *
static const char* TREATMENT_FULL_COLUMNS =
	"id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by,version,"
	"category_code,bus_no,name,py_str,wb_str,package_unit_code,type_enum,status_enum,description_text,"
	"body_site_code,specimen_code,org_id,location_id,rule_id,pricing_flag,yb_flag,yb_no,yb_match_flag,"
	"chrgitm_lv,children_json,children_flag,activity_version,merchandise_name,merchandise_py_str,"
	"merchandise_wb_str,def_encounter_unit_code,def_inhospital_unit_code";

// 子表完整字段列表，对齐 WkfActivityDefinitionChildrenMapper
static const char* TREATMENT_CHILD_FULL_COLUMNS =
	"id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by,version,"
	"activity_id,children_id,children_num,children_name";

std::list<TreatmentDO> TreatmentDAO::selectByName(const string& name)
{
	string sql = "SELECT ";
	sql += TREATMENT_FULL_COLUMNS;
	sql += " FROM wkf_activity_definition WHERE is_deleted=0 AND `name` LIKE CONCAT('%',?,'%')";
	return sqlSession->executeQuery<TreatmentDO>(sql, WkfActivityDefinitionMapper(), "%s", name);
}

// 通过ID查询单条，未找到返回空
PtrTreatmentDO TreatmentDAO::selectById(std::string id)
{
	string sql = "SELECT ";
	sql += TREATMENT_FULL_COLUMNS;
	sql += " FROM wkf_activity_definition WHERE `id`=? AND is_deleted=0";
	return sqlSession->executeQueryOne<PtrTreatmentDO>(sql, PtrWkfActivityDefinitionMapper(), "%s", id);
}

// 按 activity_id 查询子项
std::list<TreatmentChildDO> TreatmentDAO::selectChildrenByActivityId(const std::string& activityId)
{
	string sql = "SELECT ";
	sql += TREATMENT_CHILD_FULL_COLUMNS;
	sql += " FROM wkf_activity_definition_children WHERE activity_id=? AND is_deleted=0";
	return sqlSession->executeQuery<TreatmentChildDO>(sql, WkfActivityDefinitionChildrenMapper(), "%s", activityId);
}

// 更新前软删除子项
int32_t TreatmentDAO::softDeleteChildrenByActivityId(const std::string& activityId, const std::string& deletedBy, const std::string& deletedAt)
{
	string sql = "UPDATE wkf_activity_definition_children SET is_deleted=1, deleted_by=?, deleted_at=? "
		"WHERE activity_id=? AND is_deleted=0";
	return sqlSession->executeUpdate(sql, "%s%s%s", deletedBy, deletedAt, activityId);
}

// 按主键列表软删除
int32_t TreatmentDAO::removeData(std::list<std::string>& ids)
{
	if (ids.empty()) return 0;

	std::string sql = "UPDATE wkf_activity_definition SET is_deleted = 1, deleted_at = NOW() WHERE id IN (";
	SqlParams params;
	bool first = true;

	for (const auto& item : ids)
	{
		if (!first) sql += ",";
		first = false;
		sql += "?";
		// 添加字符串参数
		SQLPARAMS_PUSH(params, "s", std::string, item);
	}
	sql += ") AND is_deleted = 0";

	return sqlSession->executeUpdate(sql, params);
}
