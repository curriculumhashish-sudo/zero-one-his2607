#pragma once
#ifndef _ENCOUNTER_DIAGNOSIS_DO_
#define _ENCOUNTER_DIAGNOSIS_DO_

#include "../DoInclude.h"

class EncounterDiagnosisDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, conditionId, ConditionId);
	MYSQL_SYNTHESIZE(bool, primaryFlag, PrimaryFlag);
	MYSQL_SYNTHESIZE(int, rank, Rank);
	MYSQL_SYNTHESIZE(string, description, Description);
	MYSQL_SYNTHESIZE(string, diagTypeCode, DiagTypeCode);
	MYSQL_SYNTHESIZE(int, diagCode, DiagCode);
	MYSQL_SYNTHESIZE(int, conditionCode, ConditionCode);
	MYSQL_SYNTHESIZE(string, medType, MedType);
	MYSQL_SYNTHESIZE(bool, highestDiagFlag, HighestDiagFlag);
	MYSQL_SYNTHESIZE(string, tcmSyndromeGroup, TcmSyndromeGroup);
	MYSQL_SYNTHESIZE(bool, tcmFlag, TcmFlag);

public:
	EncounterDiagnosisDO() : BaseDO("adm_encounter_diagnosis")
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
		MYSQL_ADD_FIELD("condition_id", "s", conditionId);
		MYSQL_ADD_FIELD_NULLABLE("primary_flag", "b", primaryFlag);
		MYSQL_ADD_FIELD_NULLABLE("rank", "i", rank);
		MYSQL_ADD_FIELD_NULLABLE("description", "s", description);
		MYSQL_ADD_FIELD_NULLABLE("diag_type_code", "s", diagTypeCode);
		MYSQL_ADD_FIELD_NULLABLE("diag_code", "i", diagCode);
		MYSQL_ADD_FIELD_NULLABLE("condition_code", "i", conditionCode);
		MYSQL_ADD_FIELD_NULLABLE("med_type", "s", medType);
		MYSQL_ADD_FIELD_NULLABLE("highest_diag_flag", "b", highestDiagFlag);
		MYSQL_ADD_FIELD_NULLABLE("tcm_syndrome_group", "s", tcmSyndromeGroup);
		MYSQL_ADD_FIELD_NULLABLE("tcm_flag", "b", tcmFlag);
	}
};

typedef std::shared_ptr<EncounterDiagnosisDO> PtrEncounterDiagnosisDO;

#endif
