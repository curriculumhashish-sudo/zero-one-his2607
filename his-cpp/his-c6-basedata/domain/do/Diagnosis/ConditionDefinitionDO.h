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
#ifndef _CONDITION_DEFINITION_DO_H_
#define _CONDITION_DEFINITION_DO_H_

#include "../DoInclude.h"

/**
 * Database model for the cln_condition_definition table.
 */
class ConditionDefinitionDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, sourceEnum, SourceEnum);
	MYSQL_SYNTHESIZE(string, conditionCode, ConditionCode);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	MYSQL_SYNTHESIZE(string, typeCode, TypeCode);
	MYSQL_SYNTHESIZE(string, description, Description);
	MYSQL_SYNTHESIZE(int, ybFlag, YbFlag);
	MYSQL_SYNTHESIZE(string, diseaseCode, DiseaseCode);
	MYSQL_SYNTHESIZE(string, ybNo, YbNo);
	MYSQL_SYNTHESIZE(int, ybMatchFlag, YbMatchFlag);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);

public:
	ConditionDefinitionDO() : BaseDO("cln_condition_definition")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
		MYSQL_ADD_FIELD("deleted_at", "dt", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
		MYSQL_ADD_FIELD_NULLABLE("source_enum", "s", sourceEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("condition_code", "s", conditionCode, false);
		MYSQL_ADD_FIELD_NULLABLE("name", "s", name, false);
		MYSQL_ADD_FIELD("py_str", "s", pyStr);
		MYSQL_ADD_FIELD("wb_str", "s", wbStr);
		MYSQL_ADD_FIELD("type_code", "s", typeCode);
		MYSQL_ADD_FIELD("description", "s", description);
		MYSQL_ADD_FIELD("yb_flag", "i", ybFlag);
		MYSQL_ADD_FIELD("disease_code", "s", diseaseCode);
		MYSQL_ADD_FIELD("yb_no", "s", ybNo);
		MYSQL_ADD_FIELD("yb_match_flag", "i", ybMatchFlag);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
	}
};

#endif // !_CONDITION_DEFINITION_DO_H_
