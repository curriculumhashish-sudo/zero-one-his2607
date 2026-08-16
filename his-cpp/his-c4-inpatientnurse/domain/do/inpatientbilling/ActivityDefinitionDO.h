#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/27 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _ACTIVITYDEFINITIONDO_H_
#define _ACTIVITYDEFINITIONDO_H_

#include "../DoInclude.h"

// Complete mapping for database table wkf_activity_definition.
class ActivityDefinitionDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	MYSQL_SYNTHESIZE(string, packageUnitCode, PackageUnitCode);
	MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, descriptionText, DescriptionText);
	MYSQL_SYNTHESIZE(string, bodySiteCode, BodySiteCode);
	MYSQL_SYNTHESIZE(string, specimenCode, SpecimenCode);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(int, ruleId, RuleId);
	MYSQL_SYNTHESIZE(bool, pricingFlag, PricingFlag);
	MYSQL_SYNTHESIZE(bool, ybFlag, YbFlag);
	MYSQL_SYNTHESIZE(string, ybNo, YbNo);
	MYSQL_SYNTHESIZE(bool, ybMatchFlag, YbMatchFlag);
	MYSQL_SYNTHESIZE(int, chrgitmLv, ChrgitmLv);
	MYSQL_SYNTHESIZE(string, childrenJson, ChildrenJson);
	MYSQL_SYNTHESIZE(bool, childrenFlag, ChildrenFlag);
	MYSQL_SYNTHESIZE(string, activityVersion, ActivityVersion);
	MYSQL_SYNTHESIZE(string, merchandiseName, MerchandiseName);
	MYSQL_SYNTHESIZE(string, merchandisePyStr, MerchandisePyStr);
	MYSQL_SYNTHESIZE(string, merchandiseWbStr, MerchandiseWbStr);
	MYSQL_SYNTHESIZE(string, defEncounterUnitCode, DefEncounterUnitCode);
	MYSQL_SYNTHESIZE(string, defInhospitalUnitCode, DefInhospitalUnitCode);

public:
	ActivityDefinitionDO() : BaseDO("wkf_activity_definition")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("category_code", "s", categoryCode);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("package_unit_code", "s", packageUnitCode, true);
		MYSQL_ADD_FIELD("type_enum", "s", typeEnum);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("description_text", "s", descriptionText, true);
		MYSQL_ADD_FIELD_NULLABLE("body_site_code", "s", bodySiteCode, true);
		MYSQL_ADD_FIELD_NULLABLE("specimen_code", "s", specimenCode, true);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
		MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, true);
		MYSQL_ADD_FIELD_NULLABLE("rule_id", "i", ruleId, true);
		MYSQL_ADD_FIELD_NULLABLE("pricing_flag", "b", pricingFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_flag", "b", ybFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", ybNo, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_match_flag", "b", ybMatchFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("chrgitm_lv", "i", chrgitmLv, true);
		MYSQL_ADD_FIELD_NULLABLE("children_json", "s", childrenJson, true);
		MYSQL_ADD_FIELD_NULLABLE("children_flag", "b", childrenFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("activity_version", "s", activityVersion, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_name", "s", merchandiseName, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_py_str", "s", merchandisePyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_wb_str", "s", merchandiseWbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("def_encounter_unit_code", "s", defEncounterUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("def_inhospital_unit_code", "s", defInhospitalUnitCode, true);
	}
};

typedef std::shared_ptr<ActivityDefinitionDO> PtrActivityDefinitionDO;

#endif // _ACTIVITYDEFINITIONDO_H_
