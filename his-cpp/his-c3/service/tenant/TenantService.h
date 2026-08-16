#pragma once
#ifndef _TENANTSERVICE_H_
#define _TENANTSERVICE_H_

#include "domain/dto/tenant/TenantAddDTO.h"
#include "domain/dto/tenant/TenantModifyDTO.h"

// 租户服务类
class TenantService
{
public:
	// 保存数据
	std::string saveData(const TenantAddDTO::Wrapper& dto);
	// 修改数据
	bool updateData(const TenantModifyDTO::Wrapper& dto);
	// Soft delete tenant by ID, recording who performed the deletion
	bool removeData(const std::string& id, const std::string& deletedBy);
};

#endif // !_TENANTSERVICE_H_

