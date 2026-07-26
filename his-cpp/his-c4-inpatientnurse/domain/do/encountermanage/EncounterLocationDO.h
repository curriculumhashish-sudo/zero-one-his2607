#pragma once
#ifndef _ENCOUNTER_LOCATION_DO_
#define _ENCOUNTER_LOCATION_DO_

#include "../DoInclude.h"

class EncounterLocationDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(string, formEnum, FormEnum);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, startTime, StartTime);
	MYSQL_SYNTHESIZE(string, endTime, EndTime);

public:
	EncounterLocationDO() : BaseDO("adm_encounter_location")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("location_id", "s", locationId);
		MYSQL_ADD_FIELD_NULLABLE("form_enum", "s", formEnum);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("start_time", "s", startTime);
		MYSQL_ADD_FIELD_NULLABLE("end_time", "s", endTime);
	}
};

typedef std::shared_ptr<EncounterLocationDO> PtrEncounterLocationDO;

#endif
