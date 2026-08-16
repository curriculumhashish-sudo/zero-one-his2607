#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Date: 2026/07/24 18:00:00

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
#ifndef _TREATMENTCHILDDO_H_
#define _TREATMENTCHILDDO_H_

#include "../DoInclude.h"

/**
 * 诊疗子项 DO，对应表 wkf_activity_definition_children
 */
class TreatmentChildDO : public BaseDO
{
	// 主键
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户标识
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// 更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// 更新人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// 软删标记
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// 乐观锁版本号
	MYSQL_SYNTHESIZE(int, version, Version);

	// 父级诊疗主键
	MYSQL_SYNTHESIZE(string, activityId, ActivityId);
	// 子项引用的诊疗主键
	MYSQL_SYNTHESIZE(string, childrenId, ChildrenId);
	// 子项执行次数
	MYSQL_SYNTHESIZE(int, childrenNum, ChildrenNum);
	// 子项名称快照
	MYSQL_SYNTHESIZE(string, childrenName, ChildrenName);

public:
	TreatmentChildDO() : BaseDO("wkf_activity_definition_children")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "s", createdAt, false);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);

		MYSQL_ADD_FIELD_NULLABLE("activity_id", "s", activityId, false);
		MYSQL_ADD_FIELD_NULLABLE("children_id", "s", childrenId, false);
		MYSQL_ADD_FIELD_NULLABLE("children_num", "i", childrenNum, true);
		MYSQL_ADD_FIELD_NULLABLE("children_name", "s", childrenName, true);
	}
};

typedef std::shared_ptr<TreatmentChildDO> PtrTreatmentChildDO;

#endif // !_TREATMENTCHILDDO_H_
