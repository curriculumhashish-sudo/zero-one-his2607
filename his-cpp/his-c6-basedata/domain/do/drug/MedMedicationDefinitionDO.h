#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/23

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
#ifndef _MED_MEDICATION_DEFINITION_DO_H_
#define _MED_MEDICATION_DEFINITION_DO_H_

#include "../DoInclude.h"

/**
 * Persistence model for med_medication_definition.
 */
class MedMedicationDefinitionDO : public BaseDO
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
    MYSQL_SYNTHESIZE(string, medicationDefinition, MedicationDefinition);
    MYSQL_SYNTHESIZE(string, dddCode, DddCode);
    MYSQL_SYNTHESIZE(string, dddUnitCode, DddUnitCode);
    MYSQL_SYNTHESIZE(string, minRateCode, MinRateCode);
    MYSQL_SYNTHESIZE(string, maxRateCode, MaxRateCode);
    MYSQL_SYNTHESIZE(double, doseUnitRatio, DoseUnitRatio);
    MYSQL_SYNTHESIZE(double, purchaseUnitRatio, PurchaseUnitRatio);
    MYSQL_SYNTHESIZE(double, stockUnitRatio, StockUnitRatio);
    MYSQL_SYNTHESIZE(double, packageUnitRatio, PackageUnitRatio);

  public:
    MedMedicationDefinitionDO() : BaseDO("med_medication_definition")
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
        MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, false);
        MYSQL_ADD_FIELD_NULLABLE("name", "s", name, false);
        MYSQL_ADD_FIELD("domain_enum", "s", domainEnum);
        MYSQL_ADD_FIELD("med_version", "s", medVersion);
        MYSQL_ADD_FIELD("name_en", "s", nameEn);
        MYSQL_ADD_FIELD("py_str", "s", pyStr);
        MYSQL_ADD_FIELD("wb_str", "s", wbStr);
        MYSQL_ADD_FIELD("category_code", "s", categoryCode);
        MYSQL_ADD_FIELD("merchandise_name", "s", merchandiseName);
        MYSQL_ADD_FIELD("merchandise_py_str", "s", merchandisePyStr);
        MYSQL_ADD_FIELD("merchandise_wb_str", "s", merchandiseWbStr);
        MYSQL_ADD_FIELD("purchase_unit_code", "s", purchaseUnitCode);
        MYSQL_ADD_FIELD("split_unit_code", "s", splitUnitCode);
        MYSQL_ADD_FIELD("comprised_text", "s", comprisedText);
        MYSQL_ADD_FIELD("ingredient", "s", ingredient);
        MYSQL_ADD_FIELD("dose_from", "i", doseFrom);
        MYSQL_ADD_FIELD("approval_number", "s", approvalNumber);
        MYSQL_ADD_FIELD("yb_match_flag", "b", ybMatchFlag);
        MYSQL_ADD_FIELD("yb_no", "s", ybNo);
        MYSQL_ADD_FIELD("pharmacology_category_code", "s", pharmacologyCategoryCode);
        MYSQL_ADD_FIELD("skin_test_flag", "b", skinTestFlag);
        MYSQL_ADD_FIELD("inject_flag", "b", injectFlag);
        MYSQL_ADD_FIELD("manufacturer_id", "s", manufacturerId);
        MYSQL_ADD_FIELD("manufacturer_text", "s", manufacturerText);
        MYSQL_ADD_FIELD("supply_id", "s", supplyId);
        MYSQL_ADD_FIELD("restricted_flag", "b", restrictedFlag);
        MYSQL_ADD_FIELD("restricted_scope", "s", restrictedScope);
        MYSQL_ADD_FIELD("active_flag", "b", activeFlag);
        MYSQL_ADD_FIELD("children_flag", "b", childrenFlag);
        MYSQL_ADD_FIELD("characteristic", "i", characteristic);
        MYSQL_ADD_FIELD("item_min_quantity", "d", itemMinQuantity);
        MYSQL_ADD_FIELD("item_max_quantity", "d", itemMaxQuantity);
        MYSQL_ADD_FIELD("def_encounter_unit_code", "s", defEncounterUnitCode);
        MYSQL_ADD_FIELD("def_inhospital_unit_code", "s", defInhospitalUnitCode);
        MYSQL_ADD_FIELD("national_drug_code", "s", nationalDrugCode);
        MYSQL_ADD_FIELD("part_attribute_enum", "s", partAttributeEnum);
        MYSQL_ADD_FIELD("antibiotic_code", "s", antibioticCode);
        MYSQL_ADD_FIELD("restricted_enum", "s", restrictedEnum);
        MYSQL_ADD_FIELD("self_flag", "b", selfFlag);
        MYSQL_ADD_FIELD("antibiotic_flag", "b", antibioticFlag);
        MYSQL_ADD_FIELD("basic_flag", "b", basicFlag);
        MYSQL_ADD_FIELD("tho_part_attribute_enum", "s", thoPartAttributeEnum);
        MYSQL_ADD_FIELD("chrgitm_lv", "i", chrgitmLv);
        MYSQL_ADD_FIELD("rx_flag", "b", rxFlag);
        MYSQL_ADD_FIELD("dosage_instruction", "s", dosageInstruction);
        MYSQL_ADD_FIELD("drug69_code", "s", drug69Code);
        MYSQL_ADD_FIELD("total_volume", "s", totalVolume);
        MYSQL_ADD_FIELD("dose_form_code", "s", doseFormCode);
        MYSQL_ADD_FIELD("dose_unit_code", "s", doseUnitCode);
        MYSQL_ADD_FIELD("package_unit_code", "s", packageUnitCode);
        MYSQL_ADD_FIELD("stock_unit_code", "s", stockUnitCode);
        MYSQL_ADD_FIELD("method_code", "s", methodCode);
        MYSQL_ADD_FIELD("rate_code", "s", rateCode);
        MYSQL_ADD_FIELD("dose", "d", dose);
        MYSQL_ADD_FIELD("max_unit", "d", maxUnit);
        MYSQL_ADD_FIELD("usage_limit", "d", usageLimit);
        MYSQL_ADD_FIELD("location_id", "s", locationId);
        MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
        MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, false);
        MYSQL_ADD_FIELD("ingredient_item", "s", ingredientItem);
        MYSQL_ADD_FIELD("medicationdefinition", "s", medicationDefinition);
        MYSQL_ADD_FIELD("ddd_code", "s", dddCode);
        MYSQL_ADD_FIELD("ddd_unit_code", "s", dddUnitCode);
        MYSQL_ADD_FIELD("min_rate_code", "s", minRateCode);
        MYSQL_ADD_FIELD("max_rate_code", "s", maxRateCode);
        MYSQL_ADD_FIELD("dose_unit_ratio", "d", doseUnitRatio);
        MYSQL_ADD_FIELD("purchase_unit_ratio", "d", purchaseUnitRatio);
        MYSQL_ADD_FIELD("stock_unit_ratio", "d", stockUnitRatio);
        MYSQL_ADD_FIELD("package_unit_ratio", "d", packageUnitRatio);
    }
};

using PtrMedMedicationDefinitionDO = std::shared_ptr<MedMedicationDefinitionDO>;

#endif // !_MED_MEDICATION_DEFINITION_DO_H_
