#pragma once
#ifndef _ENCOUNTER_HISTORY_DO_
#define _ENCOUNTER_HISTORY_DO_

#include "../DoInclude.h"

/**
 * @Author: Swan_Woo
 * @Date: 2026/07/27
 * @Table: cus_encounter_history
 */
class EncounterHistoryDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, requestId, RequestId);
	MYSQL_SYNTHESIZE(string, requestTableName, RequestTableName);
	MYSQL_SYNTHESIZE(string, originalOrganizationId, OriginalOrganizationId);
	MYSQL_SYNTHESIZE(string, targetOrganizationId, TargetOrganizationId);
	MYSQL_SYNTHESIZE(string, originalLocationId, OriginalLocationId);
	MYSQL_SYNTHESIZE(string, targetLocationId, TargetLocationId);
	MYSQL_SYNTHESIZE(string, outWayCode, OutWayCode);

public:
	EncounterHistoryDO() : BaseDO("cus_encounter_history")
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
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD_NULLABLE("request_id", "s", requestId, true);
		MYSQL_ADD_FIELD_NULLABLE("request_table_name", "s", requestTableName, true);
		MYSQL_ADD_FIELD_NULLABLE("original_organization_id", "s", originalOrganizationId, true);
		MYSQL_ADD_FIELD_NULLABLE("target_organization_id", "s", targetOrganizationId, true);
		MYSQL_ADD_FIELD_NULLABLE("original_location_id", "s", originalLocationId, true);
		MYSQL_ADD_FIELD_NULLABLE("target_location_id", "s", targetLocationId, true);
		MYSQL_ADD_FIELD_NULLABLE("out_way_code", "s", outWayCode, true);
	}
};

typedef std::shared_ptr<EncounterHistoryDO> PtrEncounterHistoryDO;

#endif
