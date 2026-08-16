#pragma once
#ifndef _TENANTDO_H_
#define _TENANTDO_H_
#include "../DoInclude.h"

/**
 * 租户数据库实体类
 */
class TenantDO : public BaseDO
{
	// 租户ID
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户名称
	MYSQL_SYNTHESIZE(string, name, Name);
	// 租户代码
	MYSQL_SYNTHESIZE(string, code, Code);
	// 租户描述
	MYSQL_SYNTHESIZE(string, description, Description);
	// 租户状态
	MYSQL_SYNTHESIZE(int, isActive, IsActive);
	// 租户扩展属性
	MYSQL_SYNTHESIZE(string, extraProperties, ExtraProperties);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// 更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建者
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// 更新者
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// 删除标记
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除者
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);

public:
	TenantDO() : BaseDO("whale_tenants")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("code", "s", code);
		MYSQL_ADD_FIELD("description", "s", description);
		MYSQL_ADD_FIELD("is_active", "i", isActive);
		MYSQL_ADD_FIELD("extra_properties", "s", extraProperties);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
	}
};

// TenantDO 的共享指针别名
typedef std::shared_ptr<TenantDO> PtrTenantDO;
#endif // !_TENANTDO_H_