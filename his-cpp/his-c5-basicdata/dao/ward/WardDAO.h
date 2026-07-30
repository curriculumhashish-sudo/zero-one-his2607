#pragma once
#ifndef _WARD_DAO_H_
#define _WARD_DAO_H_

#include <list>
#include <string>

#include "BaseDAO.h"
#include "domain/do/ward/WardTreeDO.h"
#include "WardTreeMapper.h"
#include "domain/dto/ward/WardLocationAddDTO.h"
#include "domain/do/ward/WardLocationDO.h"
#include "WardLocationMapper.h"
#include "OrganizationTenantIdMapper.h"
#include "domain/do/ward/WardDO.h"

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

	// 查询病房（包含床位）
	PtrWardDO selectRoomById(const std::string& id);
	// 查询单个床位
	PtrBedDO selectBedById(const std::string& id);
	// 根据 parentId 查询床位列表
	std::list<PtrBedDO> selectBedsByParentId(const std::string& parentId);
};

#endif
