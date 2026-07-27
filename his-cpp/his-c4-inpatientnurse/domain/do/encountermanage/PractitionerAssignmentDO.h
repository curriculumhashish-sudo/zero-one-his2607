#pragma once
#ifndef _PRACTITIONER_ASSIGNMENT_DO_
#define _PRACTITIONER_ASSIGNMENT_DO_

#include "../DoInclude.h"

/**
 * @Author: Swan_Woo
 * @Date: 2026/07/27
 * @Table: adm_pr_assign
 */
class PractitionerAssignmentDO : public BaseDO
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
	MYSQL_SYNTHESIZE(bool, activeFlag, ActiveFlag);
	MYSQL_SYNTHESIZE(string, practitionerId, PractitionerId);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(string, specialtyCode, SpecialtyCode);
	MYSQL_SYNTHESIZE(int, typeCode, TypeCode);
	MYSQL_SYNTHESIZE(string, roleId, RoleId);

public:
	PractitionerAssignmentDO() : BaseDO("adm_pr_assign")
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
		MYSQL_ADD_FIELD_NULLABLE("name", "s", name, true);
		MYSQL_ADD_FIELD("active_flag", "b", activeFlag);
		MYSQL_ADD_FIELD("practitioner_id", "s", practitionerId);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
		MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, true);
		MYSQL_ADD_FIELD_NULLABLE("specialty_code", "s", specialtyCode, true);
		MYSQL_ADD_FIELD_NULLABLE("type_code", "i", typeCode, true);
		MYSQL_ADD_FIELD("role_id", "s", roleId);
	}
};

typedef std::shared_ptr<PractitionerAssignmentDO> PtrPractitionerAssignmentDO;

#endif
