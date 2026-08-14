#pragma once
#ifndef _SETTINGITEMSDO_H_
#define _SETTINGITEMSDO_H_
#include "../DoInclude.h"

/**
 * 租户数据库实体类
 */
class SettingItemsDO : public BaseDO
{
	// 主键
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, name, Name);
	MYSQL_SYNTHESIZE(std::string, value, Value);
	MYSQL_SYNTHESIZE(std::string, providerName, ProviderName);
	MYSQL_SYNTHESIZE(std::string, providerKey, ProviderKey);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(std::string, description, Description);
	MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);


public:
	SettingItemsDO() : BaseDO("whale_settings")
	{
		//字段名，字段类型，数据变量名
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("value", "s", value);
		MYSQL_ADD_FIELD("provider_name", "s", providerName);
		MYSQL_ADD_FIELD("provider_key", "s", providerKey);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("description", "s", description);
		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("deleted_at", "dt", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
	}
};

// SettingItemsDO 的共享指针别名
typedef std::shared_ptr<SettingItemsDO> PtrSettingItemsDO;
#endif // !_SETTINGITEMSDO_H_