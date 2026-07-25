#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Date: 2026/07/25 11:20:00

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
#ifndef _WKF_ACTIVITY_DEFINITION_CHILDREN_MAPPER_H_
#define _WKF_ACTIVITY_DEFINITION_CHILDREN_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/treatment/WkfActivityDefinitionChildrenDo.h"

/**
 * 诊疗子项表字段映射（按父诊疗查多行列表）
 * 对应表：wkf_activity_definition_children
 */
class WkfActivityDefinitionChildrenMapper : public Mapper<TreatmentChildDO>
{
public:
	TreatmentChildDO mapper(ResultSet* resultSet) const override
	{
		TreatmentChildDO data;
		data.setId(resultSet->getString("id"));
		data.setTenantId(resultSet->getString("tenant_id"));
		data.setCreatedAt(resultSet->getString("created_at"));
		data.setUpdatedAt(resultSet->getString("updated_at"));
		data.setCreatedBy(resultSet->getString("created_by"));
		data.setUpdatedBy(resultSet->getString("updated_by"));
		data.setIsDeleted(resultSet->getInt("is_deleted"));
		data.setDeletedAt(resultSet->getString("deleted_at"));
		data.setDeletedBy(resultSet->getString("deleted_by"));
		data.setVersion(resultSet->getInt("version"));

		data.setActivityId(resultSet->getString("activity_id"));
		data.setChildrenId(resultSet->getString("children_id"));
		data.setChildrenNum(resultSet->getInt("children_num"));
		data.setChildrenName(resultSet->getString("children_name"));
		return data;
	}
};

#endif // !_WKF_ACTIVITY_DEFINITION_CHILDREN_MAPPER_H_
