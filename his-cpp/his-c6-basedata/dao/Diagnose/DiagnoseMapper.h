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
#ifndef _diagnose_SELECT_MAPPER_H_
#define _diagnose_SELECT_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/Diagnose/ClnConditionDefinitionDO.h"

//其实这个表对应的是page
/** 将查询结果映射为诊疗定义 DO。 */
class DiagnoseMapper : public Mapper<PtrClnConditionDefinitionDO>
{
public:
	PtrClnConditionDefinitionDO mapper(ResultSet* resultSet) const override
	{


		//// 按 SELECT 字段顺序读取诊疗数据。
		//PtrClnConditionDefinitionDO data;
		//data.setId(resultSet->getString(1));
		//data.setConditionCode(resultSet->getString(2));
		//data.setName(resultSet->getString(3));
		//data.setPyStr(resultSet->getString(4));
		//data.setWbStr(resultSet->getString(5));
		//data.setSourceEnum(resultSet->getString(6));
		//data.setYbFlag(resultSet->getBoolean(7));
		//data.setStatusEnum(resultSet->getString(8));
		//return data;
		PtrClnConditionDefinitionDO diagnoseDO = std::make_shared<ClnConditionDefinitionDO>();
		
		diagnoseDO->setId(resultSet->getString("id"));
		diagnoseDO->setTenantId(resultSet->getString("tenant_id"));
		diagnoseDO->setCreatedAt(resultSet->getString("created_at"));
		diagnoseDO->setUpdatedAt(resultSet->getString("updated_at"));
		diagnoseDO->setCreatedBy(resultSet->getString("created_by"));
		diagnoseDO->setUpdatedBy(resultSet->getString("updated_by"));
		diagnoseDO->setIsDeleted(resultSet->getInt("is_deleted"));
		diagnoseDO->setDeletedAt(resultSet->getString("deleted_at"));
		diagnoseDO->setDeletedBy(resultSet->getString("deleted_by"));
		diagnoseDO->setVersion(resultSet->getInt("version"));
		diagnoseDO->setSourceEnum(resultSet->getString("source_enum"));
        diagnoseDO->setConditionCode(resultSet->getString("condition_code"));
		diagnoseDO->setName(resultSet->getString("name"));
		diagnoseDO->setPyStr(resultSet->getString("py_str"));
		diagnoseDO->setWbStr(resultSet->getString("wb_str"));
		diagnoseDO->setTypeCode(resultSet->getString("type_code"));
		diagnoseDO->setDescription(resultSet->getString("description"));
		diagnoseDO->setYbFlag(resultSet->getInt("yb_flag"));
		diagnoseDO->setDiseaseCode(resultSet->getString("disease_code"));
		diagnoseDO->setYbNo(resultSet->getString("yb_no"));
		diagnoseDO->setYbMatchFlag(resultSet->getInt("yb_match_flag"));
		diagnoseDO->setStatusEnum(resultSet->getString("status_enum"));

		return diagnoseDO;
	}
};

///** 将单列查询结果映射为租户 ID。 */
//class DiagnoseTenantMapper : public Mapper<std::string>
//{
//public:
//	std::string mapper(ResultSet* resultSet) const override
//	{
//		// 读取单列租户标识。?
//		return resultSet->getString(1);
//	}
//};这。。。这是

#endif // !_diagnose_SELECT_MAPPER_H_
