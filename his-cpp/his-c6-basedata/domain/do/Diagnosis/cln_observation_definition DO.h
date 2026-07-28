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
#ifndef _CLN_OBSERVATION_DEFINITION_DO_H_
#define _CLN_OBSERVATION_DEFINITION_DO_H_

#include "../DoInclude.h"

/**
 * Database model for cln_observation_definition 观察项目定义
 */
class ClnObsDefinitionDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, code, Code);
	MYSQL_SYNTHESIZE(string, display, Display);
	MYSQL_SYNTHESIZE(string, valueTypeEnum, ValueTypeEnum);
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);

public:
	ClnObsDefinitionDO() : BaseDO("cln_observation_definition")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("org_id", "s", orgId);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);
		MYSQL_ADD_FIELD("deleted_at", "dt", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
		MYSQL_ADD_FIELD_NULLABLE("code", "s", code, false);
		MYSQL_ADD_FIELD_NULLABLE("display", "s", display, false);
		MYSQL_ADD_FIELD_NULLABLE("value_type_enum", "s", valueTypeEnum, false);
		MYSQL_ADD_FIELD("unit_code", "s", unitCode);
		MYSQL_ADD_FIELD_NULLABLE("category_code", "s", categoryCode, false);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
	}
};

typedef std::shared_ptr<ClnObsDefinitionDO> PtrClnObsDefinitionDO;

#endif // !_CLN_OBSERVATION_DEFINITION_DO_H_#pragma once
