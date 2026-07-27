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
#ifndef _MEDICATIONDEFINITIONDO_H_
#define _MEDICATIONDEFINITIONDO_H_

#include "../DoInclude.h"

// Complete mapping for database table med_medication_definition.
class MedicationDefinitionDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, domainEnum, DomainEnum);
	MYSQL_SYNTHESIZE(string, medVersion, MedVersion);
	MYSQL_SYNTHESIZE(string, nameEn, NameEn);
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
	MYSQL_SYNTHESIZE(string, merchandiseName, MerchandiseName);
	MYSQL_SYNTHESIZE(string, merchandisePyStr, MerchandisePyStr);
	MYSQL_SYNTHESIZE(string, merchandiseWbStr, MerchandiseWbStr);
	MYSQL_SYNTHESIZE(string, purchaseUnitCode, PurchaseUnitCode);
	MYSQL_SYNTHESIZE(string, splitUnitCode, SplitUnitCode);
	MYSQL_SYNTHESIZE(string, comprisedText, ComprisedText);
	MYSQL_SYNTHESIZE(string, ingredient, Ingredient);
	MYSQL_SYNTHESIZE(int, doseFrom, DoseFrom);
	MYSQL_SYNTHESIZE(string, approvalNumber, ApprovalNumber);
	MYSQL_SYNTHESIZE(bool, ybMatchFlag, YbMatchFlag);
	MYSQL_SYNTHESIZE(string, ybNo, YbNo);
	MYSQL_SYNTHESIZE(string, pharmacologyCategoryCode, PharmacologyCategoryCode);
	MYSQL_SYNTHESIZE(bool, skinTestFlag, SkinTestFlag);
	MYSQL_SYNTHESIZE(bool, injectFlag, InjectFlag);
	MYSQL_SYNTHESIZE(string, manufacturerId, ManufacturerId);
	MYSQL_SYNTHESIZE(string, manufacturerText, ManufacturerText);
	MYSQL_SYNTHESIZE(string, supplyId, SupplyId);
	MYSQL_SYNTHESIZE(bool, restrictedFlag, RestrictedFlag);
	MYSQL_SYNTHESIZE(string, restrictedScope, RestrictedScope);
	MYSQL_SYNTHESIZE(bool, activeFlag, ActiveFlag);
	MYSQL_SYNTHESIZE(bool, childrenFlag, ChildrenFlag);
	MYSQL_SYNTHESIZE(int, characteristic, Characteristic);
	MYSQL_SYNTHESIZE(double, itemMinQuantity, ItemMinQuantity);
	MYSQL_SYNTHESIZE(double, itemMaxQuantity, ItemMaxQuantity);
	MYSQL_SYNTHESIZE(string, defEncounterUnitCode, DefEncounterUnitCode);
	MYSQL_SYNTHESIZE(string, defInhospitalUnitCode, DefInhospitalUnitCode);
	MYSQL_SYNTHESIZE(string, nationalDrugCode, NationalDrugCode);
	MYSQL_SYNTHESIZE(string, partAttributeEnum, PartAttributeEnum);
	MYSQL_SYNTHESIZE(string, antibioticCode, AntibioticCode);
	MYSQL_SYNTHESIZE(string, restrictedEnum, RestrictedEnum);
	MYSQL_SYNTHESIZE(bool, selfFlag, SelfFlag);
	MYSQL_SYNTHESIZE(bool, antibioticFlag, AntibioticFlag);
	MYSQL_SYNTHESIZE(bool, basicFlag, BasicFlag);
	MYSQL_SYNTHESIZE(string, thoPartAttributeEnum, ThoPartAttributeEnum);
	MYSQL_SYNTHESIZE(int, chrgitmLv, ChrgitmLv);
	MYSQL_SYNTHESIZE(bool, rxFlag, RxFlag);
	MYSQL_SYNTHESIZE(string, dosageInstruction, DosageInstruction);
	MYSQL_SYNTHESIZE(string, drug69Code, Drug69Code);
	MYSQL_SYNTHESIZE(string, totalVolume, TotalVolume);
	MYSQL_SYNTHESIZE(string, doseFormCode, DoseFormCode);
	MYSQL_SYNTHESIZE(string, doseUnitCode, DoseUnitCode);
	MYSQL_SYNTHESIZE(string, packageUnitCode, PackageUnitCode);
	MYSQL_SYNTHESIZE(string, stockUnitCode, StockUnitCode);
	MYSQL_SYNTHESIZE(string, methodCode, MethodCode);
	MYSQL_SYNTHESIZE(string, rateCode, RateCode);
	MYSQL_SYNTHESIZE(double, dose, Dose);
	MYSQL_SYNTHESIZE(double, maxUnit, MaxUnit);
	MYSQL_SYNTHESIZE(double, usageLimit, UsageLimit);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(string, ingredientItem, IngredientItem);
	MYSQL_SYNTHESIZE(string, medicationdefinition, Medicationdefinition);
	MYSQL_SYNTHESIZE(string, dddCode, DddCode);
	MYSQL_SYNTHESIZE(string, dddUnitCode, DddUnitCode);
	MYSQL_SYNTHESIZE(string, minRateCode, MinRateCode);
	MYSQL_SYNTHESIZE(string, maxRateCode, MaxRateCode);
	MYSQL_SYNTHESIZE(double, doseUnitRatio, DoseUnitRatio);
	MYSQL_SYNTHESIZE(double, purchaseUnitRatio, PurchaseUnitRatio);
	MYSQL_SYNTHESIZE(double, stockUnitRatio, StockUnitRatio);
	MYSQL_SYNTHESIZE(double, packageUnitRatio, PackageUnitRatio);

public:
	MedicationDefinitionDO() : BaseDO("med_medication_definition")
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
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD_NULLABLE("domain_enum", "s", domainEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("med_version", "s", medVersion, true);
		MYSQL_ADD_FIELD_NULLABLE("name_en", "s", nameEn, true);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("category_code", "s", categoryCode, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_name", "s", merchandiseName, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_py_str", "s", merchandisePyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_wb_str", "s", merchandiseWbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("purchase_unit_code", "s", purchaseUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("split_unit_code", "s", splitUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("comprised_text", "s", comprisedText, true);
		MYSQL_ADD_FIELD_NULLABLE("ingredient", "s", ingredient, true);
		MYSQL_ADD_FIELD_NULLABLE("dose_from", "i", doseFrom, true);
		MYSQL_ADD_FIELD_NULLABLE("approval_number", "s", approvalNumber, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_match_flag", "b", ybMatchFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", ybNo, true);
		MYSQL_ADD_FIELD_NULLABLE("pharmacology_category_code", "s", pharmacologyCategoryCode, true);
		MYSQL_ADD_FIELD_NULLABLE("skin_test_flag", "b", skinTestFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("inject_flag", "b", injectFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("manufacturer_id", "s", manufacturerId, true);
		MYSQL_ADD_FIELD_NULLABLE("manufacturer_text", "s", manufacturerText, true);
		MYSQL_ADD_FIELD_NULLABLE("supply_id", "s", supplyId, true);
		MYSQL_ADD_FIELD_NULLABLE("restricted_flag", "b", restrictedFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("restricted_scope", "s", restrictedScope, true);
		MYSQL_ADD_FIELD_NULLABLE("active_flag", "b", activeFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("children_flag", "b", childrenFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("characteristic", "i", characteristic, true);
		MYSQL_ADD_FIELD_NULLABLE("item_min_quantity", "d", itemMinQuantity, true);
		MYSQL_ADD_FIELD_NULLABLE("item_max_quantity", "d", itemMaxQuantity, true);
		MYSQL_ADD_FIELD_NULLABLE("def_encounter_unit_code", "s", defEncounterUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("def_inhospital_unit_code", "s", defInhospitalUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("national_drug_code", "s", nationalDrugCode, true);
		MYSQL_ADD_FIELD_NULLABLE("part_attribute_enum", "s", partAttributeEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("antibiotic_code", "s", antibioticCode, true);
		MYSQL_ADD_FIELD_NULLABLE("restricted_enum", "s", restrictedEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("self_flag", "b", selfFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("antibiotic_flag", "b", antibioticFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("basic_flag", "b", basicFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("tho_part_attribute_enum", "s", thoPartAttributeEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("chrgitm_lv", "i", chrgitmLv, true);
		MYSQL_ADD_FIELD_NULLABLE("rx_flag", "b", rxFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("dosage_instruction", "s", dosageInstruction, true);
		MYSQL_ADD_FIELD_NULLABLE("drug69_code", "s", drug69Code, true);
		MYSQL_ADD_FIELD_NULLABLE("total_volume", "s", totalVolume, true);
		MYSQL_ADD_FIELD_NULLABLE("dose_form_code", "s", doseFormCode, true);
		MYSQL_ADD_FIELD_NULLABLE("dose_unit_code", "s", doseUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("package_unit_code", "s", packageUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("stock_unit_code", "s", stockUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("method_code", "s", methodCode, true);
		MYSQL_ADD_FIELD_NULLABLE("rate_code", "s", rateCode, true);
		MYSQL_ADD_FIELD_NULLABLE("dose", "d", dose, true);
		MYSQL_ADD_FIELD_NULLABLE("max_unit", "d", maxUnit, true);
		MYSQL_ADD_FIELD_NULLABLE("usage_limit", "d", usageLimit, true);
		MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, true);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("org_id", "s", orgId);
		MYSQL_ADD_FIELD_NULLABLE("ingredient_item", "s", ingredientItem, true);
		MYSQL_ADD_FIELD_NULLABLE("medicationdefinition", "s", medicationdefinition, true);
		MYSQL_ADD_FIELD_NULLABLE("ddd_code", "s", dddCode, true);
		MYSQL_ADD_FIELD_NULLABLE("ddd_unit_code", "s", dddUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("min_rate_code", "s", minRateCode, true);
		MYSQL_ADD_FIELD_NULLABLE("max_rate_code", "s", maxRateCode, true);
		MYSQL_ADD_FIELD_NULLABLE("dose_unit_ratio", "d", doseUnitRatio, true);
		MYSQL_ADD_FIELD_NULLABLE("purchase_unit_ratio", "d", purchaseUnitRatio, true);
		MYSQL_ADD_FIELD_NULLABLE("stock_unit_ratio", "d", stockUnitRatio, true);
		MYSQL_ADD_FIELD_NULLABLE("package_unit_ratio", "d", packageUnitRatio, true);
	}
};

typedef std::shared_ptr<MedicationDefinitionDO> PtrMedicationDefinitionDO;

#endif // _MEDICATIONDEFINITIONDO_H_
