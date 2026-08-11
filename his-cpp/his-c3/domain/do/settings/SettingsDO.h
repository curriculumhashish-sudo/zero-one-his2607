#pragma once
#ifndef _SETTINGSDO_H_
#define _SETTINGDO_H_
#include "../DoInclude.h"

class SettingsDO : public BaseDO {
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, value, Value);
	MYSQL_SYNTHESIZE(string, description, Description);
	MYSQL_SYNTHESIZE(string, providerName, ProviderName);
	MYSQL_SYNTHESIZE(string, providerKey, ProviderKey);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);

public:
	SettingsDO() : BaseDO("whale_settings") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("value", "s", value);
		MYSQL_ADD_FIELD("description", "s", description);
		MYSQL_ADD_FIELD("provider_name", "s", providerName);
		MYSQL_ADD_FIELD("provider_key", "s", providerKey);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
	}
};

typedef std::shared_ptr<SettingsDO> PtrSettingsDO;
#endif