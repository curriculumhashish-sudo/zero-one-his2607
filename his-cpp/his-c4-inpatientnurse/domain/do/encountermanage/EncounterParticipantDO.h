#pragma once
#ifndef _ENCOUNTER_PARTICIPANT_DO_
#define _ENCOUNTER_PARTICIPANT_DO_

#include "../DoInclude.h"

class EncounterParticipantDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, roleCode, RoleCode);
	MYSQL_SYNTHESIZE(string, practitionerId, PractitionerId);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, startTime, StartTime);
	MYSQL_SYNTHESIZE(string, endTime, EndTime);

public:
	EncounterParticipantDO() : BaseDO("adm_encounter_participant")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("role_code", "s", roleCode);
		MYSQL_ADD_FIELD("practitioner_id", "s", practitionerId);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("start_time", "s", startTime, true);
		MYSQL_ADD_FIELD_NULLABLE("end_time", "s", endTime, true);
	}
};

typedef std::shared_ptr<EncounterParticipantDO> PtrEncounterParticipantDO;

#endif
