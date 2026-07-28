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
#include "DiagnosisDAO.h"
#include "DiagnosisSelectMapper.h"

static const std::string STATUS_ACTIVE = "ACTIVE";
static const std::vector<std::string> WESTERN_SOURCE_CODES = {
	"general_dx", "chronic_spec", "disease_pkg", "paid_dx", "day_surgery_dx", "tumor_morph"};
static const std::vector<std::string> TCM_SOURCE_CODES = {"tcm_dx", "tcm_syndrome"};

static void appendSourceFilter(
	std::stringstream& sqlCondition,
	SqlParams& params,
	const std::vector<std::string>& sourceCodes)
{
	sqlCondition << " AND d.`source_enum` IN (";
	for (std::size_t index = 0; index < sourceCodes.size(); ++index)
	{
		if (index != 0)
		{
			sqlCondition << ",";
		}
		sqlCondition << "?";
		SQLPARAMS_PUSH(params, "s", std::string, sourceCodes[index]);
	}
	sqlCondition << ")";
}

std::string DiagnosisDAO::queryConditionBuilder(
	const DiagnosisSelectQuery::Wrapper& query,
	const std::string& tenantId,
	SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE d.`is_deleted`=? AND d.`status_enum`=? AND d.`tenant_id`=?";
	SQLPARAMS_PUSH(params, "b", bool, false);
	SQLPARAMS_PUSH(params, "s", std::string, STATUS_ACTIVE);
	SQLPARAMS_PUSH(params, "s", std::string, tenantId);

	const std::string keyword = query->keyword.getValue("");
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

	const std::string diagnosisKindCode = query->diagnosisKindCode.getValue("");
	if (diagnosisKindCode == DiagnosisSelectQuery::DIAGNOSIS_KIND_WESTERN)
	{
		appendSourceFilter(sqlCondition, params, WESTERN_SOURCE_CODES);
	}
	else if (diagnosisKindCode == DiagnosisSelectQuery::DIAGNOSIS_KIND_TRADITIONAL_CHINESE)
	{
		appendSourceFilter(sqlCondition, params, TCM_SOURCE_CODES);
	}
	return sqlCondition.str();
}

std::string DiagnosisDAO::selectCurrentTenantId(const std::string& userId)
{
	if (userId.empty())
	{
		return "";
	}

	const std::string sql =
		"SELECT s.`tenant_id` FROM `whale_user_login_session` s "
		"INNER JOIN `whale_users` u ON u.`id`=s.`user_id` "
		"AND u.`is_deleted`=0 AND u.`is_active`=1 AND u.`is_locked_out`=0 "
		"INNER JOIN `whale_user_tenants` ut ON ut.`user_id`=s.`user_id` "
		"AND ut.`tenant_id`=s.`tenant_id` AND ut.`is_deleted`=0 "
		"WHERE s.`user_id`=? AND s.`tenant_id` IS NOT NULL AND s.`tenant_id`<>'' "
		"ORDER BY s.`updated_at` DESC LIMIT 1";
	return sqlSession->executeQueryOne<std::string>(
		sql,
		DiagnosisTenantMapper(),
		"%s",
		userId);
}

uint64_t DiagnosisDAO::count(
	const DiagnosisSelectQuery::Wrapper& query,
	const std::string& tenantId)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM `cln_condition_definition` d";
	sql += queryConditionBuilder(query, tenantId, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<ClnConditionDefinitionDO> DiagnosisDAO::selectWithPage(
	const DiagnosisSelectQuery::Wrapper& query,
	const std::string& tenantId)
{
	SqlParams params;
	std::string sql =
		"SELECT d.`id`,d.`condition_code`,d.`name`,d.`py_str`,d.`wb_str`,"
		"d.`source_enum`,COALESCE(d.`yb_flag`,0),d.`status_enum` "
		"FROM `cln_condition_definition` d";
	sql += queryConditionBuilder(query, tenantId, params);
	sql += " ORDER BY COALESCE(d.`updated_at`,d.`created_at`) DESC,d.`id` DESC";

	const uint64_t pageIndex = query->pageIndex.getValue(1);
	const uint64_t pageSize = query->pageSize.getValue(10);
	const uint64_t offset = (pageIndex - 1) * pageSize;
	sql += " LIMIT ?,?";
	SQLPARAMS_PUSH(params, "ull", uint64_t, offset);
	SQLPARAMS_PUSH(params, "ull", uint64_t, pageSize);
	return sqlSession->executeQuery<ClnConditionDefinitionDO>(
		sql,
		DiagnosisSelectMapper(),
		params);
}
