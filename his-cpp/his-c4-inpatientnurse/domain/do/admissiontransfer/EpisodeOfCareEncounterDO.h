#pragma once
#ifndef _EPISODE_OF_CARE_ENCOUNTER_DO_
#define _EPISODE_OF_CARE_ENCOUNTER_DO_

#include "../DoInclude.h"

class EpisodeOfCareEncounterDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenant_id, Tenant_id);
	MYSQL_SYNTHESIZE(string, created_at, Created_at);
	MYSQL_SYNTHESIZE(string, updated_at, Updated_at);
	MYSQL_SYNTHESIZE(string, created_by, Created_by);
	MYSQL_SYNTHESIZE(string, updated_by, Updated_by);
	MYSQL_SYNTHESIZE(bool, is_deleted, Is_deleted);
	MYSQL_SYNTHESIZE(string, deleted_at, Deleted_at);
	MYSQL_SYNTHESIZE(string, deleted_by, Deleted_by);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, episode_of_care_id, Episode_of_care_id);
	MYSQL_SYNTHESIZE(string, encounter_id, Encounter_id);
	MYSQL_SYNTHESIZE(string, join_time, Join_time);
	MYSQL_SYNTHESIZE(string, leave_time, Leave_time);

public:
	EpisodeOfCareEncounterDO() : BaseDO("adm_episode_of_care_encounter")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenant_id, true);
		MYSQL_ADD_FIELD("created_at", "s", created_at);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updated_at, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", created_by, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updated_by, true);
		MYSQL_ADD_FIELD("is_deleted", "b", is_deleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deleted_at, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deleted_by, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("episode_of_care_id", "s", episode_of_care_id);
		MYSQL_ADD_FIELD("encounter_id", "s", encounter_id);
		MYSQL_ADD_FIELD("join_time", "s", join_time);
		MYSQL_ADD_FIELD_NULLABLE("leave_time", "s", leave_time, true);
		setIs_deleted(false);
		setVersion(0);
	}
};

typedef std::shared_ptr<EpisodeOfCareEncounterDO> PtrEpisodeOfCareEncounterDO;

#endif
