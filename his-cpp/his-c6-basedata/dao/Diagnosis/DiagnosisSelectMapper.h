#pragma once
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
#ifndef _DIAGNOSIS_SELECT_MAPPER_H_
#define _DIAGNOSIS_SELECT_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/Diagnosis/ClnConditionDefinitionDO.h"

/** 映射可选诊疗查询结果。 */
class DiagnosisSelectMapper : public Mapper<ClnConditionDefinitionDO>
{
public:
	ClnConditionDefinitionDO mapper(ResultSet* resultSet) const override
	{
		ClnConditionDefinitionDO data;
		data.setId(resultSet->getString(1));
		data.setConditionCode(resultSet->getString(2));
		data.setName(resultSet->getString(3));
		data.setPyStr(resultSet->getString(4));
		data.setWbStr(resultSet->getString(5));
		data.setSourceEnum(resultSet->getString(6));
		data.setYbFlag(resultSet->getBoolean(7));
		data.setStatusEnum(resultSet->getString(8));
		return data;
	}
};

/** 映射当前登录租户。 */
class DiagnosisTenantMapper : public Mapper<std::string>
{
public:
	std::string mapper(ResultSet* resultSet) const override
	{
		return resultSet->getString(1);
	}
};

#endif // !_DIAGNOSIS_SELECT_MAPPER_H_
