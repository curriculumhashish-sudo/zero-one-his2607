/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/24

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
#ifndef _ADM_SPECIMEN_DEFINITION_DO_H_
#define _ADM_SPECIMEN_DEFINITION_DO_H_

#include "../DoInclude.h"

/**
 * Database model for adm_specimen_definition 样本/标本定义表
 */
class AdmSpecimenDefinitionDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
	MYSQL_SYNTHESIZE(string, typeCode, TypeCode);
	MYSQL_SYNTHESIZE(string, purchaseUnitCode, PurchaseUnitCode);
	MYSQL_SYNTHESIZE(string, totalVolume, TotalVolume);
	MYSQL_SYNTHESIZE(double, itemMinQuantity, ItemMinQuantity);
	MYSQL_SYNTHESIZE(double, itemMaxQuantity, ItemMaxQuantity);
	MYSQL_SYNTHESIZE(string, splitUnitCode, SplitUnitCode);
	MYSQL_SYNTHESIZE(string, merchandiseName, MerchandiseName);
	MYSQL_SYNTHESIZE(string, merchandisePyStr, MerchandisePyStr);
	MYSQL_SYNTHESIZE(string, merchandiseWbStr, MerchandiseWbStr);
	MYSQL_SYNTHESIZE(string, defEncounterUnitCode, DefEncounterUnitCode);
	MYSQL_SYNTHESIZE(string, defInhospitalUnitCode, DefInhospitalUnitCode);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(string, modelNumber, ModelNumber);
	MYSQL_SYNTHESIZE(bool, hvcmFlag, HvcmFlag);
	MYSQL_SYNTHESIZE(string, doseUnitCode, DoseUnitCode);
	MYSQL_SYNTHESIZE(string, packageUnitCode, PackageUnitCode);
	MYSQL_SYNTHESIZE(string, stockUnitCode, StockUnitCode);
	MYSQL_SYNTHESIZE(string, approvalNumber, ApprovalNumber);
	MYSQL_SYNTHESIZE(string, manufacturerId, ManufacturerId);
	MYSQL_SYNTHESIZE(string, manufacturerText, ManufacturerText);
	MYSQL_SYNTHESIZE(string, supplyId, SupplyId);
	MYSQL_SYNTHESIZE(string, description, Description);
	MYSQL_SYNTHESIZE(string, jurisdiction, Jurisdiction);
	MYSQL_SYNTHESIZE(string, deviceVersion, DeviceVersion);
	MYSQL_SYNTHESIZE(string, substanceText, SubstanceText);
	MYSQL_SYNTHESIZE(bool, allergenFlag, AllergenFlag);
	MYSQL_SYNTHESIZE(bool, rxFlag, RxFlag);
	MYSQL_SYNTHESIZE(bool, ybFlag, YbFlag);
	MYSQL_SYNTHESIZE(string, ybNo, YbNo);
	MYSQL_SYNTHESIZE(bool, ybMatchFlag, YbMatchFlag);
	MYSQL_SYNTHESIZE(int, chrgitmLv, ChrgitmLv);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(bool, activeFlag, ActiveFlag);
	MYSQL_SYNTHESIZE(bool, childrenFlag, ChildrenFlag);
	MYSQL_SYNTHESIZE(int, characteristic, Characteristic);
	MYSQL_SYNTHESIZE(double, doseUnitRatio, DoseUnitRatio);
	MYSQL_SYNTHESIZE(double, purchaseUnitRatio, PurchaseUnitRatio);
	MYSQL_SYNTHESIZE(double, stockUnitRatio, StockUnitRatio);
	MYSQL_SYNTHESIZE(double, packageUnitRatio, PackageUnitRatio);

public:
	AdmSpecimenDefinitionDO() : BaseDO("adm_specimen_definition")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);
		MYSQL_ADD_FIELD("deleted_at", "dt", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
		MYSQL_ADD_FIELD_NULLABLE("name", "s", name, false);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("py_str", "s", pyStr);
		MYSQL_ADD_FIELD("wb_str", "s", wbStr);
		MYSQL_ADD_FIELD("category_code", "s", categoryCode);
		MYSQL_ADD_FIELD("type_code", "s", typeCode);
		MYSQL_ADD_FIELD("purchase_unit_code", "s", purchaseUnitCode);
		MYSQL_ADD_FIELD("total_volume", "s", totalVolume);
		MYSQL_ADD_FIELD("item_min_quantity", "d", itemMinQuantity);
		MYSQL_ADD_FIELD("item_max_quantity", "d", itemMaxQuantity);
		MYSQL_ADD_FIELD("split_unit_code", "s", splitUnitCode);
		MYSQL_ADD_FIELD("merchandise_name", "s", merchandiseName);
		MYSQL_ADD_FIELD("merchandise_py_str", "s", merchandisePyStr);
		MYSQL_ADD_FIELD("merchandise_wb_str", "s", merchandiseWbStr);
		MYSQL_ADD_FIELD("def_encounter_unit_code", "s", defEncounterUnitCode);
		MYSQL_ADD_FIELD("def_inhospital_unit_code", "s", defInhospitalUnitCode);
		MYSQL_ADD_FIELD("org_id", "s", orgId);
		MYSQL_ADD_FIELD("location_id", "s", locationId);
		MYSQL_ADD_FIELD("model_number", "s", modelNumber);
		MYSQL_ADD_FIELD_NULLABLE("hvcm_flag", "b", hvcmFlag, true);
		MYSQL_ADD_FIELD("dose_unit_code", "s", doseUnitCode);
		MYSQL_ADD_FIELD("package_unit_code", "s", packageUnitCode);
		MYSQL_ADD_FIELD("stock_unit_code", "s", stockUnitCode);
		MYSQL_ADD_FIELD("approval_number", "s", approvalNumber);
		MYSQL_ADD_FIELD("manufacturer_id", "s", manufacturerId);
		MYSQL_ADD_FIELD("manufacturer_text", "s", manufacturerText);
		MYSQL_ADD_FIELD("supply_id", "s", supplyId);
		MYSQL_ADD_FIELD("description", "s", description);
		MYSQL_ADD_FIELD("jurisdiction", "s", jurisdiction);
		MYSQL_ADD_FIELD("device_version", "s", deviceVersion);
		MYSQL_ADD_FIELD("substance_text", "s", substanceText);
		MYSQL_ADD_FIELD_NULLABLE("allergen_flag", "b", allergenFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("rx_flag", "b", rxFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_flag", "b", ybFlag, true);
		MYSQL_ADD_FIELD("yb_no", "s", ybNo);
		MYSQL_ADD_FIELD_NULLABLE("yb_match_flag", "b", ybMatchFlag, true);
		MYSQL_ADD_FIELD("chrgitm_lv", "i", chrgitmLv);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("active_flag", "b", activeFlag, false);
		MYSQL_ADD_FIELD_NULLABLE("children_flag", "b", childrenFlag, true);
		MYSQL_ADD_FIELD("characteristic", "i", characteristic);
		MYSQL_ADD_FIELD("dose_unit_ratio", "d", doseUnitRatio);
		MYSQL_ADD_FIELD("purchase_unit_ratio", "d", purchaseUnitRatio);
		MYSQL_ADD_FIELD("stock_unit_ratio", "d", stockUnitRatio);
		MYSQL_ADD_FIELD("package_unit_ratio", "d", packageUnitRatio);
	}
};

typedef std::shared_ptr<AdmSpecimenDefinitionDO> PtrAdmSpecimenDefinitionDO;

#endif // !_ADM_SPECIMEN_DEFINITION_DO_H_#pragma once
