#pragma once
#ifndef _WARD_DAO_H_
#define _WARD_DAO_H_

#include <list>
#include <string>

#include "BaseDAO.h"
#include "domain/do/ward/WardTreeDO.h"
#include "WardTreeMapper.h"
#include"domain/dto/ward/WardLocationAddDTO.h"
#include"domain/do/ward/WardLocationDO.h"
#include "WardLocationMapper.h"
#include "OrganizationTenantIdMapper.h"
class WardDAO : public BaseDAO
{
public:
    std::list<WardTreeDO> selectTreeRows(
        const std::string& organizationId
    );
	std::string selectOrganizationTenantIdForUpdate(
		const std::string& organizationId);

	// 查询父级完整记录PtrWardLocationDO
	PtrWardLocationDO selectLocationByIdForUpdate(
		const std::string& id);

	// 查询同级同名
	bool existsSiblingName(
		const std::string& organizationId,
		const std::string& formEnum,
		const std::string& parentId,
		const std::string& name);

	// 插入
	int insertLocation(const WardLocationDO& data);
};

#endif
