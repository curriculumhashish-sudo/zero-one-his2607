#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/27 00:00:00

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
#ifndef _PROCEDUREPERFORMERDO_H_
#define _PROCEDUREPERFORMERDO_H_

#include "../DoInclude.h"

// Complete mapping for database table cln_procedure_performer.
class ProcedurePerformerDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, procedureId, ProcedureId);
	MYSQL_SYNTHESIZE(string, functionCode, FunctionCode);
	MYSQL_SYNTHESIZE(string, startTime, StartTime);
	MYSQL_SYNTHESIZE(string, endTime, EndTime);
	MYSQL_SYNTHESIZE(string, practitionerId, PractitionerId);
	MYSQL_SYNTHESIZE(string, orgFromId, OrgFromId);

public:
	ProcedurePerformerDO() : BaseDO("cln_procedure_performer")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("procedure_id", "s", procedureId);
		MYSQL_ADD_FIELD_NULLABLE("function_code", "s", functionCode, true);
		MYSQL_ADD_FIELD("start_time", "dt", startTime);
		MYSQL_ADD_FIELD_NULLABLE("end_time", "dt", endTime, true);
		MYSQL_ADD_FIELD("practitioner_id", "s", practitionerId);
		MYSQL_ADD_FIELD_NULLABLE("org_from_id", "s", orgFromId, true);
	}
};

typedef std::shared_ptr<ProcedurePerformerDO> PtrProcedurePerformerDO;

#endif // _PROCEDUREPERFORMERDO_H_
