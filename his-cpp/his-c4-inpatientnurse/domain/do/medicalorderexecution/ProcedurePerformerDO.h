#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: fanpai
 @Date: 2026/07/26 00:00:00

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
#ifndef _PROCEDURE_PERFORMER_DO_
#define _PROCEDURE_PERFORMER_DO_

#include "../DoInclude.h"

/**
 * Data Object mapping to table: cln_procedure_performer
 * Records the performers (practitioners) associated with a clinical procedure.
 */
class ProcedurePerformerDO : public BaseDO
{
    // Primary key
    MYSQL_SYNTHESIZE(string, id, Id);
    // Tenant identifier
    MYSQL_SYNTHESIZE(string, tenantId, TenantId);
    // Record creation time
    MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
    // Record last update time
    MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
    // Creator identifier
    MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
    // Last updater identifier
    MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
    // Soft delete flag: 0=active, 1=deleted
    MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
    // Soft delete timestamp
    MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
    // Deleter identifier
    MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
    // Optimistic lock version
    MYSQL_SYNTHESIZE(int, version, Version);
    // Procedure identifier (foreign key to cln_procedure.id)
    MYSQL_SYNTHESIZE(string, procedureId, ProcedureId);
    // Function code of the performer (e.g., surgeon, assistant, nurse)
    MYSQL_SYNTHESIZE(string, functionCode, FunctionCode);
    // Start time of the performer's participation
    MYSQL_SYNTHESIZE(string, startTime, StartTime);
    // End time of the performer's participation
    MYSQL_SYNTHESIZE(string, endTime, EndTime);
    // Practitioner identifier (healthcare worker ID)
    MYSQL_SYNTHESIZE(string, practitionerId, PractitionerId);
    // Source organization identifier
    MYSQL_SYNTHESIZE(string, orgFromId, OrgFromId);

public:
    /**
     * Register table name and field mappings.
     * Type placeholders: "s"=string, "i"=int, "dt"=datetime, "b"=bool
     */
    ProcedurePerformerDO() : BaseDO("cln_procedure_performer")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
        MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
        MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
        MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
        MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
        MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
        MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
        MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
        MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
        MYSQL_ADD_FIELD_NULLABLE("procedure_id", "s", procedureId, false);
        MYSQL_ADD_FIELD_NULLABLE("function_code", "s", functionCode, true);
        MYSQL_ADD_FIELD_NULLABLE("start_time", "dt", startTime, false);
        MYSQL_ADD_FIELD_NULLABLE("end_time", "dt", endTime, true);
        MYSQL_ADD_FIELD_NULLABLE("practitioner_id", "s", practitionerId, false);
        MYSQL_ADD_FIELD_NULLABLE("org_from_id", "s", orgFromId, true);
    }
};

typedef std::shared_ptr<ProcedurePerformerDO> PtrProcedurePerformerDO;

#endif // !_PROCEDURE_PERFORMER_DO_
