#pragma once

#ifndef _ADMEPISODEOFCARE_H_
#define _ADMEPISODEOFCARE_H_

#include "../DoInclude.h"

/**
 * 诊疗周期实体
 */
class AdmEpisodeOfCareDO : public BaseDO
{
	// 诊疗ID
	MYSQL_SYNTHESIZE(string, id, Id);
	// 住户ID
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// 最后更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// 最后更新人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// 软删标识
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// 乐观锁版本号
	MYSQL_SYNTHESIZE(int, version, Version);
	// 患者标识
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// 机构标识
	MYSQL_SYNTHESIZE(bool, organizationId, OrganizationId);
	
	// bus编号
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	// 
	MYSQL_SYNTHESIZE(string, careTeamId, CareTeamId);
	// 
	MYSQL_SYNTHESIZE(bool, careType, CareType);
	// 照护目标
	MYSQL_SYNTHESIZE(string, careGoal, CareGoal);
	// 枚举值
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	// 标志位
	MYSQL_SYNTHESIZE(bool, activeFlag, ActiveFlag);
	// 
	MYSQL_SYNTHESIZE(string, startTime, StartTime);
	// 
	MYSQL_SYNTHESIZE(string, endTime, EndTime);
public:
	AdmEpisodeOfCareDO() : BaseDO("adm_episode_of_care")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);

		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);

		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);

		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);

		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);

		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);

		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);

		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);

		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);

		MYSQL_ADD_FIELD_NULLABLE("patient_id", "s", patientId, false);

		MYSQL_ADD_FIELD_NULLABLE("organization_id", "b", organizationId, false);

		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, false);

		MYSQL_ADD_FIELD_NULLABLE("care_team_id", "s", careTeamId, true);

		MYSQL_ADD_FIELD_NULLABLE("care_type", "s", careType, true);

		MYSQL_ADD_FIELD_NULLABLE("care_goal", "s", careGoal, true);

		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);

		MYSQL_ADD_FIELD_NULLABLE("active_flag", "b", activeFlag, false);

		MYSQL_ADD_FIELD_NULLABLE("start_time", "dt", startTime, true);

		MYSQL_ADD_FIELD_NULLABLE("end_time", "dt", endTime, true);
	}
};

// 定义一个器材实体的智能指针
typedef std::shared_ptr<InventoryLockDO> PtrAdmEpisodeOfCareDO;

#endif // !_ADMEPISODEOFCARE_H_