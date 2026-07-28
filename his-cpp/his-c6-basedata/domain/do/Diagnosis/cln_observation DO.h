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
#ifndef _CLN_OBSERVATION_DO_H_
#define _CLN_OBSERVATION_DO_H_

#include "../DoInclude.h"

/**
 * Database model for cln_observation 临床观测指标表
 */
class ClnObservationDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, serviceRequestId, ServiceRequestId);
	MYSQL_SYNTHESIZE(string, definitionId, DefinitionId);
	MYSQL_SYNTHESIZE(string, code, Code);
	MYSQL_SYNTHESIZE(double, valueQuantity, ValueQuantity);
	MYSQL_SYNTHESIZE(string, valueString, ValueString);
	MYSQL_SYNTHESIZE(string, valueCodeableConcept, ValueCodeableConcept);
	MYSQL_SYNTHESIZE(string, interpretationCode, InterpretationCode);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, effectiveTime, EffectiveTime);
	MYSQL_SYNTHESIZE(string, sourceObservationId, SourceObservationId);
	MYSQL_SYNTHESIZE(string, sourceModuleEnum, SourceModuleEnum);
	MYSQL_SYNTHESIZE(string, performerId, PerformerId);

public:
	ClnObservationDO() : BaseDO("cln_observation")
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
		MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
		MYSQL_ADD_FIELD("service_request_id", "s", serviceRequestId);
		MYSQL_ADD_FIELD_NULLABLE("definition_id", "s", definitionId, false);
		MYSQL_ADD_FIELD("code", "s", code);
		MYSQL_ADD_FIELD("value_quantity", "d", valueQuantity);
		MYSQL_ADD_FIELD("value_string", "s", valueString);
		MYSQL_ADD_FIELD("value_codeable_concept", "s", valueCodeableConcept);
		MYSQL_ADD_FIELD("interpretation_code", "s", interpretationCode);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
		MYSQL_ADD_FIELD("effective_time", "dt", effectiveTime);
		MYSQL_ADD_FIELD("source_observation_id", "s", sourceObservationId);
		MYSQL_ADD_FIELD("source_module_enum", "s", sourceModuleEnum);
		MYSQL_ADD_FIELD("performer_id", "s", performerId);
	}
};

typedef std::shared_ptr<ClnObservationDO> PtrClnObservationDO;

#endif // !_CLN_OBSERVATION_DO_H_#pragma once
