#pragma once

#ifndef _EPISODE_OF_CARE_ENCOUNTER_DO_
#define _EPISODE_OF_CARE_ENCOUNTER_DO_
#include "../DoInclude.h"

/**
 * 诊疗周期就诊
 * 
 */
class AdmEpisodeOfCareEncounterDO : public BaseDO
{
	// 主键
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户标识
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// 最后更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// 最后更新人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// 软删标记 0-未删 1-已删
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// 乐观锁版本号
	MYSQL_SYNTHESIZE(int, version, Version);
	// 疗程主键
	MYSQL_SYNTHESIZE(string, episodeOfCareId, EpisodeOfCareId);
	// 就诊主键
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	// 纳入疗程时间
	MYSQL_SYNTHESIZE(string, joinTime, JoinTime);
	// 离开疗程时间
	MYSQL_SYNTHESIZE(string, leaveTime, LeaveTime);
public:
	EpisodeOfCareEncounterDO() : BaseDO("adm_episode_of_care_encounter")
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
		MYSQL_ADD_FIELD_NULLABLE("episode_of_care_id", "s", episodeOfCareId, false);
		MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
		MYSQL_ADD_FIELD_NULLABLE("join_time", "dt", joinTime, false);
		MYSQL_ADD_FIELD_NULLABLE("leave_time", "dt", leaveTime, true);
	}
};
typedef std::shared_ptr<AdmEpisodeOfCareEncounterDO> PtrAdmEpisodeOfCareEncounterDO;
#endif // !_EPISODE_OF_CARE_ENCOUNTER_DO_