#pragma once
#ifndef _LOCATION_DO_
#define _LOCATION_DO_

#include "../DoInclude.h"

class adm_locationDO : public BaseDO 
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
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, bus_no, Bus_no);
	MYSQL_SYNTHESIZE(string, status_enum, Status_enum);
	MYSQL_SYNTHESIZE(string, operational_enum, Operational_enum);
	MYSQL_SYNTHESIZE(string, form_enum, Form_enum);
	MYSQL_SYNTHESIZE(string, mode_enum, Mode_enum);
	MYSQL_SYNTHESIZE(string, type_json, Type_json);
	MYSQL_SYNTHESIZE(string, organization_id, Organization_id);
	MYSQL_SYNTHESIZE(int, display_order, Display_order);
	MYSQL_SYNTHESIZE(string, py_str, Py_str);
	MYSQL_SYNTHESIZE(string, wb_str, Wb_str);
	MYSQL_SYNTHESIZE(string, parent_id, Parent_id);

public:
	adm_locationDO() : BaseDO("adm_location") 
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
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("bus_no", "s", bus_no);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD_NULLABLE("operational_enum", "s", operational_enum, true);
		MYSQL_ADD_FIELD("form_enum", "s", form_enum);
		MYSQL_ADD_FIELD("mode_enum", "s", mode_enum);
		MYSQL_ADD_FIELD_NULLABLE("type_json", "s", type_json, true);
		MYSQL_ADD_FIELD("organization_id", "s", organization_id);
		MYSQL_ADD_FIELD_NULLABLE("display_order", "i", display_order, true);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", py_str, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wb_str, true);
		MYSQL_ADD_FIELD_NULLABLE("parent_id", "s", parent_id, true);
	}
};

typedef std::shared_ptr<adm_locationDO> PtrLocationDO;

#endif
