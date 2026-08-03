#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/30

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
#ifndef _TREATMENT_SELECT_MAPPER_H_
#define _TREATMENT_SELECT_MAPPER_H_

#include "Mapper.h"
#include "../../domain/bo/treatment/TreatmentSelectBO.h"

/** 将选择诊疗联表结果映射为 BO。 */
class TreatmentSelectMapper : public Mapper<TreatmentSelectBO>
{
public:
	// 按 DAO 查询列顺序映射字段，列序必须与 SELECT 保持一致。
	TreatmentSelectBO mapper(ResultSet* resultSet) const override
	{
		TreatmentSelectBO data;
		data.setId(resultSet->getString(1));
		data.setBusNo(resultSet->getString(2));
		data.setName(resultSet->getString(3));
		data.setCategoryCode(resultSet->getString(4));
		data.setCategoryCodeDictText(resultSet->getString(5));
		data.setPackageUnitCode(resultSet->getString(6));
		data.setPackageUnitCodeDictText(resultSet->getString(7));
		data.setDefEncounterUnitCode(resultSet->getString(8));
		data.setDefEncounterUnitCodeDictText(resultSet->getString(9));
		return data;
	}
};

/** 将单列查询结果映射为租户 ID。 */
class TreatmentTenantMapper : public Mapper<std::string>
{
public:
	std::string mapper(ResultSet* resultSet) const override
	{
		return resultSet->getString(1);
	}
};

#endif // !_TREATMENT_SELECT_MAPPER_H_
