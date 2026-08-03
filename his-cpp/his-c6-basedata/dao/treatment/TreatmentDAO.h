#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 14:23:49

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
#ifndef _TREATMENT_DAO_
#define _TREATMENT_DAO_
#include "BaseDAO.h"
#include "../../domain/bo/treatment/TreatmentSelectBO.h"
#include "../../domain/do/treatment/WkfActivityDefinitionDo.h"
#include "../../domain/do/treatment/WkfActivityDefinitionChildrenDo.h"
#include "../../domain/query/treatment/TreatmentQuery.h"
#include "../../domain/query/treatment/TreatmentSelectQuery.h"

/**
 * 诊疗表数据库操作实现
 */
class TreatmentDAO : public BaseDAO
{
private:
	//************************************
	// Method:      queryConditionBuilder
	// FullName:    TreatmentDAO::queryConditionBuilder
	// Access:      private
	// Returns:     std::string 返回条件SQL语句，即WHERE部分的语句
	// Parameter:   const TreatmentQuery::Wrapper& query 查询数据对象
	// Parameter:   SqlParams& params 存放查询数据的参数对象
	// Description: 查询条件构建器，根据TreatmentQuery对象动态构建查询条件相关参数
	//************************************
	inline std::string queryConditionBuilder(const TreatmentQuery::Wrapper& query, SqlParams& params);
	// 构建选择诊疗列表的租户、关键字和状态条件。
	inline std::string selectConditionBuilder(
		const TreatmentSelectQuery::Wrapper& query,
		const std::string& tenantId,
		SqlParams& params,
		const std::string& categoryCode = "");
	
public:
	// 统计数据条数
	uint64_t count(const TreatmentQuery::Wrapper& query);
	// 分页查询数据
	std::list<TreatmentDO> selectWithPage(const TreatmentQuery::Wrapper& query);
	// 查询当前用户唯一有效的租户。
	std::string selectCurrentTenantId(const std::string& userId);
	// 统计可选诊疗数据。
	uint64_t countSelect(
		const TreatmentSelectQuery::Wrapper& query,
		const std::string& tenantId,
		const std::string& categoryCode = "");
	// 分页查询可选诊疗数据。
	std::list<TreatmentSelectBO> selectSelectWithPage(
		const TreatmentSelectQuery::Wrapper& query,
		const std::string& tenantId,
		const std::string& categoryCode = "");
	//乐观锁更新状态
	int32_t updateStatusById(const std::string& id, const std::string& statusEnum,
		const std::string& updatedBy, int32_t version);
	// 通过名称模糊查询数据
	std::list<TreatmentDO> selectByName(const string& name);
	// 通过ID查询数据
	PtrTreatmentDO selectById(std::string id);
	// 按 activity_id 查询子项
	std::list<TreatmentChildDO> selectChildrenByActivityId(const std::string& activityId);
	// 按 activity_id 软删除子项
	int32_t softDeleteChildrenByActivityId(const std::string& activityId, const std::string& deletedBy, const std::string& deletedAt);
	// Query price rows by treatment id (PURCHASE/SALE/LIMIT)
	std::list<AdmChargeItemDefinitionDO> selectPricesByInstanceId(const std::string& instanceId);
	// Soft-delete price rows by treatment id
	int32_t softDeletePricesByInstanceId(const std::string& instanceId, const std::string& deletedBy, const std::string& deletedAt);
	// 按主键列表软删除
	int32_t removeData(std::list<std::string>& ids);
};
#endif // !_TREATMENT_DAO_
