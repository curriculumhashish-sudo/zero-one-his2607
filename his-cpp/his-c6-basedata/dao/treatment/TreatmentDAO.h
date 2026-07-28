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
#include "../../domain/do/treatment/WkfActivityDefinitionDo.h"
#include "../../domain/do/treatment/WkfActivityDefinitionChildrenDo.h"
#include "../../domain/query/treatment/TreatmentQuery.h"

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
public:
	// 统计数据条数
	uint64_t count(const TreatmentQuery::Wrapper& query);
	// 分页查询数据
	std::list<TreatmentDO> selectWithPage(const TreatmentQuery::Wrapper& query);
	// 通过名称模糊查询数据
	std::list<TreatmentDO> selectByName(const string& name);
	// 通过ID查询数据
	PtrTreatmentDO selectById(std::string id);
	// 按 activity_id 查询子项
	std::list<TreatmentChildDO> selectChildrenByActivityId(const std::string& activityId);
	// 按 activity_id 软删除子项
	int32_t softDeleteChildrenByActivityId(const std::string& activityId, const std::string& deletedBy, const std::string& deletedAt);
	// 按主键列表软删除
	int32_t removeData(std::list<std::string>& ids);
};
#endif // !_TREATMENT_DAO_
