#pragma once
#ifndef _DEPT_DO_
#define _DEPT_DO_

#include "../DoInclude.h"

/**
 * Department DO mapped to adm_organization table.
 */
class DeptDO : public BaseDO
{
	// Department id
	MYSQL_SYNTHESIZE(string, id, Id);
	// Tenant ID
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// Created time
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// Updated time
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// Creator ID
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// Updater ID
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// Is deleted
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	// Deleted time
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// Deleter ID
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// Version number
	MYSQL_SYNTHESIZE(int, version, Version);
	// Department name
	MYSQL_SYNTHESIZE(string, name, Name);
	// Business number
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	// Parent organization ID
	MYSQL_SYNTHESIZE(string, parentId, ParentId);
	// Type enum
	MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
	// Class enum
	MYSQL_SYNTHESIZE(string, classEnum, ClassEnum);
	// Status enum: ACTIVE / INACTIVE (active_flag is deprecated)
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	// Display order
	MYSQL_SYNTHESIZE(int, displayOrder, DisplayOrder);
	// Default doctor ID
	MYSQL_SYNTHESIZE(string, defDoctorId, DefDoctorId);
	// Pinyin string
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	// Wubi string
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	// Medical insurance code
	MYSQL_SYNTHESIZE(string, ybNo, YbNo);
	// Medical insurance name
	MYSQL_SYNTHESIZE(string, ybName, YbName);
	// Medical institution ID
	MYSQL_SYNTHESIZE(string, medinsId, MedinsId);
	// Medical insurance division
	MYSQL_SYNTHESIZE(string, medinsAdmdvs, MedinsAdmdvs);
	// Institution type
	MYSQL_SYNTHESIZE(string, medinsType, MedinsType);
	// Institution level
	MYSQL_SYNTHESIZE(string, medinsLv, MedinsLv);
	// Category
	MYSQL_SYNTHESIZE(string, caty, Caty);

public:
	DeptDO() : BaseDO("adm_organization")
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
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD_NULLABLE("parent_id", "s", parentId, true);
		MYSQL_ADD_FIELD("type_enum", "s", typeEnum);
		MYSQL_ADD_FIELD_NULLABLE("class_enum", "s", classEnum, true);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("display_order", "i", displayOrder, true);
		MYSQL_ADD_FIELD_NULLABLE("def_doctor_id", "s", defDoctorId, true);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", ybNo, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_name", "s", ybName, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_id", "s", medinsId, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_admdvs", "s", medinsAdmdvs, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_type", "s", medinsType, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_lv", "s", medinsLv, true);
		MYSQL_ADD_FIELD_NULLABLE("caty", "s", caty, true);
	}
};

typedef std::shared_ptr<DeptDO> PtrDeptDO;

#endif // !_DEPT_DO_
