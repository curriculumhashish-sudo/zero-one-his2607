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
    // Read-only query by ID (no FOR UPDATE lock)
    // Used for room/bed ID query endpoints, distinct from selectLocationByIdForUpdate
    // Queries adm_location table, filters is_deleted=0
    PtrWardLocationDO selectLocationById(
        const std::string& id
    );

	std::string selectOrganizationTenantIdForUpdate(
		const std::string& organizationId);

	// Lock and return one active location row.
	PtrWardLocationDO selectLocationByIdForUpdate(
		const std::string& id);

	// Check for a duplicate name under the same parent.
	bool existsSiblingName(
		const std::string& organizationId,
		const std::string& formEnum,
		const std::string& parentId,
		const std::string& name,
		const std::string& excludeId = "");

    // Check whether a direct, non-deleted child exists.
    bool hasChildren(
        const std::string& parentId,
        bool activeOnly);

    // Update editable fields with optimistic locking.
    int updateLocation(
        const WardLocationDO& data,
        int expectedVersion);

    // Soft-delete one location with optimistic locking.
    int softDeleteLocation(
        const std::string& id,
        const std::string& formEnum,
        int expectedVersion,
        const std::string& deletedAt,
        const std::string& deletedBy);

    // Update publication status with optimistic locking.
    int updateLocationStatus(
        const std::string& id,
        const std::string& formEnum,
        const std::string& statusEnum,
        int expectedVersion,
        const std::string& updatedAt,
        const std::string& updatedBy);

	// Insert a new location.
	int insertLocation(const WardLocationDO& data);

	// ============================================================
	// Ward/Bed query methods (c5-ward-pb)
	// ============================================================

	// Query ward room (includes bed list)
	PtrWardDO selectRoomById(const std::string& id);
	// Query single bed
	PtrBedDO selectBedById(const std::string& id);
	// Query bed list by parentId
	std::list<PtrBedDO> selectBedsByParentId(const std::string& parentId);
};

#endif
