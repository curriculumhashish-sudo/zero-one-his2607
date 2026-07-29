/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/24

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
#include "DiagnoseDAO.h"
#include "DiagnoseMapper.h"
#include "DiagnosisSelectMapper.h"
#include "../../domain/DiagnoseConstants.h"
#include "StringUtil.h"

std::string DiagnoseDAO::buildSelectableDiagnosisCondition(
	const DiagnosisSelectQuery::Wrapper& query,
	const std::string& tenantId,
	SqlParams& params)
{
	std::stringstream sqlCondition;
	// 追加中医或西医来源的参数化 IN 条件。
	auto appendSourceFilter = [&sqlCondition, &params](
		const char* const* sourceCodes,
		std::size_t sourceCodeCount)
	{
		sqlCondition << " AND d.`source_enum` IN (";
		for (std::size_t index = 0; index < sourceCodeCount; ++index)
		{
			if (index != 0)
			{
				sqlCondition << ",";
			}
			sqlCondition << "?";
			SQLPARAMS_PUSH(
				params,
				"s",
				std::string,
				std::string(sourceCodes[index]));
		}
		sqlCondition << ")";
	};

	// 基础条件：未删除、已启用且属于当前租户。
	sqlCondition << " WHERE d.`is_deleted`=? AND d.`status_enum`=? AND d.`tenant_id`=?";
	SQLPARAMS_PUSH(params, "b", bool, false);
	SQLPARAMS_PUSH(
		params,
		"s",
		std::string,
		std::string(DiagnoseConstants::STATUS_ACTIVE));
	SQLPARAMS_PUSH(params, "s", std::string, tenantId);

	std::string keyword = query->keyword.getValue("");
	StringUtil::trim(&keyword);
	// 关键字匹配名称、编码、拼音码和五笔码。
	if (!keyword.empty())
	{
		sqlCondition
			<< " AND (d.`name` LIKE CONCAT('%',?,'%')"
			<< " OR d.`condition_code` LIKE CONCAT('%',?,'%')"
			<< " OR d.`py_str` LIKE CONCAT('%',?,'%')"
			<< " OR d.`wb_str` LIKE CONCAT('%',?,'%'))";
		for (int index = 0; index < 4; ++index)
		{
			SQLPARAMS_PUSH(params, "s", std::string, keyword);
		}
	}

	std::string diagnosisKindCode = query->diagnosisKindCode.getValue("");
	StringUtil::trim(&diagnosisKindCode);
	// 根据诊疗类型限制可选来源。
	if (diagnosisKindCode == DiagnoseConstants::KIND_FILTER_WESTERN)
	{
		appendSourceFilter(
			DiagnoseConstants::WESTERN_SOURCE_CODES.data(),
			DiagnoseConstants::WESTERN_SOURCE_CODES.size());
	}
	else if (diagnosisKindCode == DiagnoseConstants::KIND_FILTER_TCM)
	{
		appendSourceFilter(
			DiagnoseConstants::TCM_SOURCE_CODES.data(),
			DiagnoseConstants::TCM_SOURCE_CODES.size());
	}
	return sqlCondition.str();
}

std::string DiagnoseDAO::selectCurrentTenantId(const std::string& userId)
{
	// 用户标识为空时不执行数据库查询。
	if (userId.empty())
	{
		return "";
	}

	// 优先取最近登录租户；仅有一个有效租户时安全回退。
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
		DiagnosisTenantMapper(),
		params);
}

uint64_t DiagnoseDAO::countSelectableDiagnoses(
	const DiagnosisSelectQuery::Wrapper& query,
	const std::string& tenantId)
{
	// 统计查询与分页查询复用完全相同的条件。
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM `cln_condition_definition` d";
	sql += buildSelectableDiagnosisCondition(query, tenantId, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<ClnConditionDefinitionDO> DiagnoseDAO::selectSelectableDiagnosesWithPage(
	const DiagnosisSelectQuery::Wrapper& query,
	const std::string& tenantId)
{
	SqlParams params;
	// 只查询接口需要的列，避免使用 SELECT *。
	std::string sql =
		"SELECT d.`id`,d.`condition_code`,d.`name`,d.`py_str`,d.`wb_str`,"
		"d.`source_enum`,COALESCE(d.`yb_flag`,0),d.`status_enum` "
		"FROM `cln_condition_definition` d";
	sql += buildSelectableDiagnosisCondition(query, tenantId, params);
	sql += " ORDER BY d.`updated_at` DESC,d.`id` DESC";

	const uint64_t pageIndex = query->pageIndex.getValue(1);
	const uint64_t pageSize = query->pageSize.getValue(10);
	const uint64_t offset = (pageIndex - 1) * pageSize;
	// 分页偏移和条数均使用参数绑定。
	sql += " LIMIT ?,?";
	SQLPARAMS_PUSH(params, "ull", uint64_t, offset);
	SQLPARAMS_PUSH(params, "ull", uint64_t, pageSize);
	return sqlSession->executeQuery<ClnConditionDefinitionDO>(
		sql,
		DiagnosisSelectMapper(),
		params);
}

// 统计总数
uint64_t DiagnoseDAO::count(const DiagnosePageQuery::Wrapper& query)
{
	std::string sql = "SELECT count(id) FROM cln_condition_definition WHERE is_deleted = 0 ";
	SqlParams params;
	// 关键词模糊
	if (query->keyword)
	{
		sql += " AND (name LIKE CONCAT('%',?,'%') OR condition_code LIKE CONCAT('%',?,'%')) ";
		SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
		SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
	}
	// 诊断类型type_code筛选
	if (query->diagnoseType)
	{
		sql += " AND type_code = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->diagnoseType.getValue(""));
	}

	return sqlSession->executeQueryNumerical(sql, params);
}

// 分页列表查询
std::list<PtrClnConditionDefinitionDO> DiagnoseDAO::selectAll(const DiagnosePageQuery::Wrapper& query)
{
	std::string sql = "SELECT id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by,version,source_enum,condition_code,name,py_str,wb_str,type_code,description,yb_flag,disease_code,yb_no,yb_match_flag,status_enum FROM cln_condition_definition WHERE is_deleted = 0 ";
	SqlParams params;
	if (query->keyword)
	{
		sql += " AND (name LIKE CONCAT('%',?,'%') OR condition_code LIKE CONCAT('%',?,'%')) ";
		SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
		SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
	}
	if (query->diagnoseType)
	{
		sql += " AND type_code = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->diagnoseType.getValue(""));
	}
	// 分页偏移
	sql += " LIMIT ?,?";
	SQLPARAMS_PUSH(params, "ull", uint64_t, (query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10));
	SQLPARAMS_PUSH(params, "ull", uint64_t, query->pageSize.getValue(10));
	return sqlSession->executeQuery<PtrClnConditionDefinitionDO>(sql, DiagnoseMapper(), params);
}

// 根据ID单查
PtrClnConditionDefinitionDO DiagnoseDAO::selectById(const std::string& id)
{
	std::string sql = "SELECT id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by,version,source_enum,condition_code,name,py_str,wb_str,type_code,description,yb_flag,disease_code,yb_no,yb_match_flag,status_enum FROM cln_condition_definition WHERE id = ? AND is_deleted = 0";
	return sqlSession->executeQueryOne<PtrClnConditionDefinitionDO>(sql, DiagnoseMapper(), "%s", id);
}
