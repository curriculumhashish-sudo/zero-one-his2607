#pragma once

#ifndef _BED_DO_
#define _BED_DO_
#include "../DoInclude.h"

class adm_locationDO : public BaseDO 
{
	// Primary key
	MYSQL_SYNTHESIZE(string, id, Id);
	// Tenant identifier
	MYSQL_SYNTHESIZE(string, tenant_id, Tenant_id);
	// Creation time
	MYSQL_SYNTHESIZE(string, created_at, Created_at);
	// Last update time
	MYSQL_SYNTHESIZE(string, updated_at, Updated_at);
	// Creator
	MYSQL_SYNTHESIZE(string, created_by, Created_by);
	// Last updater
	MYSQL_SYNTHESIZE(string, updated_by, Updated_by);
	// Soft-delete flag
	MYSQL_SYNTHESIZE(bool, is_deleted, Is_deleted);
	// Deletion time
	MYSQL_SYNTHESIZE(string, deleted_at, Deleted_at);
	// Deleter
	MYSQL_SYNTHESIZE(string, deleted_by, Deleted_by);
	// Optimistic lock version
	MYSQL_SYNTHESIZE(int, version, Version);
	// Location name (e.g. bed "301-1")
	MYSQL_SYNTHESIZE(string, name, Name);
	// Business number
	MYSQL_SYNTHESIZE(string, bus_no, Bus_no);
	// Status enum value
	MYSQL_SYNTHESIZE(string, status_enum, Status_enum);
	// Operational enum value
	MYSQL_SYNTHESIZE(string, operational_enum, Operational_enum);
	// Form enum value (BED / WARD / ...)
	MYSQL_SYNTHESIZE(string, form_enum, Form_enum);
	// Mode enum value
	MYSQL_SYNTHESIZE(string, mode_enum, Mode_enum);
	// Type JSON snapshot
	MYSQL_SYNTHESIZE(string, type_json, Type_json);
	// Organization / department id
	MYSQL_SYNTHESIZE(string, organization_id, Organization_id);
	// Display order
	MYSQL_SYNTHESIZE(int, display_order, Display_order);
	// Pinyin code
	MYSQL_SYNTHESIZE(string, py_str, Py_str);
	// Wubi code
	MYSQL_SYNTHESIZE(string, wb_str, Wb_str);
	// Parent location id (ward / department)
	MYSQL_SYNTHESIZE(string, parent_id, Parent_id);

public:
	adm_locationDO() : BaseDO("adm_location") 
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
		MYSQL_ADD_FIELD("created_at", "s", created_at);
		MYSQL_ADD_FIELD("updated_at", "s", updated_at);
		MYSQL_ADD_FIELD("created_by", "s", created_by);
		MYSQL_ADD_FIELD("updated_by", "s", updated_by);
		MYSQL_ADD_FIELD("is_deleted", "b", is_deleted);
		MYSQL_ADD_FIELD("deleted_at", "s", deleted_at);
		MYSQL_ADD_FIELD("deleted_by", "s", deleted_by);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("bus_no", "s", bus_no);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("operational_enum", "s", operational_enum);
		MYSQL_ADD_FIELD("form_enum", "s", form_enum);
		MYSQL_ADD_FIELD("mode_enum", "s", mode_enum);
		MYSQL_ADD_FIELD("type_json", "s", type_json);
		MYSQL_ADD_FIELD("organization_id", "s", organization_id);
		MYSQL_ADD_FIELD("display_order", "i", display_order);
		MYSQL_ADD_FIELD("py_str", "s", py_str);
		MYSQL_ADD_FIELD("wb_str", "s", wb_str);
		MYSQL_ADD_FIELD("parent_id", "s", parent_id);
		// safe defaults for NOT NULL columns
		setIs_deleted(false);
		setVersion(0);
	}

};

typedef std::shared_ptr<adm_locationDO> PtrBedDO;
#endif
