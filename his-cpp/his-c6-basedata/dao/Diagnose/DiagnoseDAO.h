#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei

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
#ifndef _diagnose_DAO_H_
#define _diagnose_DAO_H_

#include "BaseDAO.h"
#include "../../domain/do/Diagnose/DiagnoseDO.h"
#include "../../domain/do/Diagnose/ClnConditionDefinitionDO.h"
#include "../../domain/query/Diagnose/DiagnoseSelectQuery.h"
#include "../../domain/query/Diagnose/DiagnosePageQuery.h"
#include "../../domain/query/Diagnose/DiagnoseCatalogQuery.h"
/** 选择诊疗的数据访问对象。 */
class DiagnoseDAO : public BaseDAO
{
private:
	//// 构造统计和分页查询共用的筛选条件。
	//std::string buildSelectablediagnoseCondition(
	//	const DiagnoseSelectQuery::Wrapper& query,
	//	const std::string& tenantId,
	//	SqlParams& params);

public:
	// 获取数据条数
	uint64_t count(const DiagnosePageQuery::Wrapper& query);
	// 获取诊断列表
	std::list<PtrClnConditionDefinitionDO> selectAll(const DiagnosePageQuery::Wrapper& query);
	// 通过编号获取指定诊断
	PtrClnConditionDefinitionDO selectById(const std::string& id);
	//// 根据登录用户解析当前有效租户。
	//std::string selectCurrentTenantId(const std::string& userId);
	//// 统计符合条件的诊疗数量。
	//uint64_t countSelectableDiagnoses(
	//	const DiagnoseSelectQuery::Wrapper& query,
	//	const std::string& tenantId);
	//// 查询当前页的选择诊疗数据。
	//std::list<ClnConditionDefinitionDO> selectSelectableDiagnosesWithPage(
	//	const DiagnoseSelectQuery::Wrapper& query,
	//	const std::string& tenantId);
	//std::list<DiagnoseDO> selectByName(const std::string& name);
	uint64_t DiagnoseDAO::count(const DiagnoseSelectQuery::Wrapper& query);
	std::list<PtrClnConditionDefinitionDO> DiagnoseDAO::selectAll(const DiagnoseSelectQuery::Wrapper& query);



};


#endif // !_diagnose_DAO_H_
