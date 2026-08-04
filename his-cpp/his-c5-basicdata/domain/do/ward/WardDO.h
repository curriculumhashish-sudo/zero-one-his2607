#ifndef _WARD_DO_
#define _WARD_DO_

#include "../DoInclude.h"
#include <vector>

class BedDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, parentId, ParentId);
	MYSQL_SYNTHESIZE(string, organizationId, OrganizationId);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, typeJson, TypeJson);
	MYSQL_SYNTHESIZE(string, displayOrder, DisplayOrder);
	MYSQL_SYNTHESIZE(string, version, Version);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);

public:
	BedDO() : BaseDO("adm_location")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("parent_id", "s", parentId);
		MYSQL_ADD_FIELD("organization_id", "s", organizationId);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("type_json", "s", typeJson);
		MYSQL_ADD_FIELD("display_order", "i", displayOrder);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
	}
};

typedef std::shared_ptr<BedDO> PtrBedDO;
/**
 * WardDO - 病区病房
 */
class WardDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, organizationId, OrganizationId);
	MYSQL_SYNTHESIZE(string, parentId, ParentId);
	MYSQL_SYNTHESIZE(string, formEnum, FormEnum);
	MYSQL_SYNTHESIZE(string, modeEnum, ModeEnum);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);

	MYSQL_SYNTHESIZE(string, typeJson, TypeJson)
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	MYSQL_SYNTHESIZE(int, displayOrder, DisplayOrder);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);

	MYSQL_SYNTHESIZE(int, totalBeds, TotalBeds);         // total_beds
	MYSQL_SYNTHESIZE(int, availableBeds, AvailableBeds); // available_beds
	MYSQL_SYNTHESIZE(int, occupiedBeds, OccupiedBeds);   // occupied_beds
	MYSQL_SYNTHESIZE(int, reservedBeds, ReservedBeds);   // reserved_beds

	MYSQL_SYNTHESIZE(string, floor, Floor);                             // floor
	MYSQL_SYNTHESIZE(string, wardCategory, WardCategory);               // ward_category
	MYSQL_SYNTHESIZE(string, defaultNurseStationId, DefaultNurseStationId); // default_nurse_station_id
	MYSQL_SYNTHESIZE(string, nursingLevel, NursingLevel);               // nursing_level

public:
	// 床位列表
	std::vector<PtrBedDO> beds;

public:
	WardDO() : BaseDO("adm_location")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("organization_id", "s", organizationId);
		MYSQL_ADD_FIELD("parent_id", "s", parentId);
		MYSQL_ADD_FIELD("form_enum", "s", formEnum);
		MYSQL_ADD_FIELD("mode_enum", "s", modeEnum);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("type_json", "s", typeJson);
		MYSQL_ADD_FIELD("py_str", "s", pyStr);
		MYSQL_ADD_FIELD("wb_str", "s", wbStr);
		MYSQL_ADD_FIELD("display_order", "i", displayOrder);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);

		MYSQL_ADD_FIELD("total_beds", "i", totalBeds);
		MYSQL_ADD_FIELD("available_beds", "i", availableBeds);
		MYSQL_ADD_FIELD("occupied_beds", "i", occupiedBeds);
		MYSQL_ADD_FIELD("reserved_beds", "i", reservedBeds);

		MYSQL_ADD_FIELD("floor", "s", floor);
		MYSQL_ADD_FIELD("ward_category", "s", wardCategory);
		MYSQL_ADD_FIELD("default_nurse_station_id", "s", defaultNurseStationId);
		MYSQL_ADD_FIELD("nursing_level", "s", nursingLevel);


	}
};

typedef std::shared_ptr<WardDO> PtrWardDO;

#endif // !_WARD_DO_