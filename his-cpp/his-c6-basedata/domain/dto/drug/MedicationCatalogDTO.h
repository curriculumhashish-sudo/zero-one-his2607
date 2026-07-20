#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: bandao
 @Date: 2026/07/20

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
#ifndef _MEDICATION_CATALOG_DTO_H_
#define _MEDICATION_CATALOG_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicationCatalogBaseDTO : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogBaseDTO, DTO);

	API_DTO_FIELD(String, orgId, ZH_WORDS_GETTER("medication.field.orgId"), false, "30001");
	API_DTO_FIELD(String, medicationDefId, ZH_WORDS_GETTER("medication.field.medicationDefId"), false, "20001");
	API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("medication.field.busNo"), false, "YP20250409001");
	API_DTO_FIELD(String, name, ZH_WORDS_GETTER("medication.field.name"), true, nullptr);
	API_DTO_FIELD(String, domainEnum, ZH_WORDS_GETTER("medication.field.domainEnum"), false, "3");
	API_DTO_FIELD(String, version, ZH_WORDS_GETTER("medication.field.medVersion"), false, "V1.0");
	API_DTO_FIELD(String, nameEn, ZH_WORDS_GETTER("medication.field.nameEn"), false, "Amoxicillin Capsules");
	API_DTO_FIELD(String, pyStr, ZH_WORDS_GETTER("medication.field.pyStr"), false, "AMXLJN");
	API_DTO_FIELD(String, wbStr, ZH_WORDS_GETTER("medication.field.wbStr"), false, "SGRM");
	API_DTO_FIELD(String, categoryCode, ZH_WORDS_GETTER("medication.field.categoryCode"), true, nullptr);
	API_DTO_FIELD(String, merchandiseName, ZH_WORDS_GETTER("medication.field.merchandiseName"), true, nullptr);
	API_DTO_FIELD(String, merchandisePyStr, ZH_WORDS_GETTER("medication.field.merchandisePyStr"), false, "AMX");
	API_DTO_FIELD(String, merchandiseWbStr, ZH_WORDS_GETTER("medication.field.merchandiseWbStr"), false, "SGR");
	API_DTO_FIELD(String, purchaseUnitCode, ZH_WORDS_GETTER("medication.field.purchaseUnitCode"), true, nullptr);
	API_DTO_FIELD(String, purchaseUnitCode_dictText,
		ZH_WORDS_GETTER("medication.field.purchaseUnitCodeText"), false,
		ZH_WORDS_GETTER("medication.example.purchaseUnitText"));
	API_DTO_FIELD(String, splitUnitCode, ZH_WORDS_GETTER("medication.field.splitUnitCode"), true, nullptr);
	API_DTO_FIELD(String, comprisedText, ZH_WORDS_GETTER("medication.field.comprisedText"), false,
		ZH_WORDS_GETTER("medication.example.comprisedText"));
	API_DTO_FIELD(String, ingredient, ZH_WORDS_GETTER("medication.field.ingredient"), false,
		ZH_WORDS_GETTER("medication.example.ingredient"));
	API_DTO_FIELD(Int32, doseFrom, ZH_WORDS_GETTER("medication.field.doseFrom"), false, 1);
	API_DTO_FIELD(String, doseFrom_dictText, ZH_WORDS_GETTER("medication.field.doseFromText"), false,
		ZH_WORDS_GETTER("medication.example.doseFormText"));
	API_DTO_FIELD(String, approvalNumber, ZH_WORDS_GETTER("medication.field.approvalNumber"), false,
		ZH_WORDS_GETTER("medication.example.approvalNumber"));
	API_DTO_FIELD(Boolean, ybMatchFlag, ZH_WORDS_GETTER("medication.field.ybMatchFlag"), false, true);
	API_DTO_FIELD(String, ybNo, ZH_WORDS_GETTER("medication.field.ybNo"), false, "YB2025001");
	API_DTO_FIELD(String, pharmacologyCategoryCode,
		ZH_WORDS_GETTER("medication.field.pharmacologyCategoryCode"), true, nullptr);
	API_DTO_FIELD(Boolean, skinTestFlag, ZH_WORDS_GETTER("medication.field.skinTestFlag"), false, false);
	API_DTO_FIELD(Boolean, injectFlag, ZH_WORDS_GETTER("medication.field.injectFlag"), false, false);
	API_DTO_FIELD(String, manufacturerId, ZH_WORDS_GETTER("medication.field.manufacturerId"), false, "50001");
	API_DTO_FIELD(String, manufacturerText, ZH_WORDS_GETTER("medication.field.manufacturerText"), true, nullptr);
	API_DTO_FIELD(String, supplyId, ZH_WORDS_GETTER("medication.field.supplyId"), true, nullptr);
	API_DTO_FIELD(Boolean, restrictedFlag, ZH_WORDS_GETTER("medication.field.restrictedFlag"), false, false);
	API_DTO_FIELD(String, restrictedScope, ZH_WORDS_GETTER("medication.field.restrictedScope"), false,
		ZH_WORDS_GETTER("medication.example.restrictedScope"));
	API_DTO_FIELD(Boolean, childrenFlag, ZH_WORDS_GETTER("medication.field.childrenFlag"), false, true);
	API_DTO_FIELD(Int32, characteristic, ZH_WORDS_GETTER("medication.field.characteristic"), false, 1);
	API_DTO_FIELD(Float64, itemMinQuantity, ZH_WORDS_GETTER("medication.field.itemMinQuantity"), false, 10.0);
	API_DTO_FIELD(Float64, itemMaxQuantity, ZH_WORDS_GETTER("medication.field.itemMaxQuantity"), false, 100.0);
	API_DTO_FIELD(String, nationalDrugCode, ZH_WORDS_GETTER("medication.field.nationalDrugCode"), false,
		"860000123456789");
	API_DTO_FIELD(String, partAttributeEnum, ZH_WORDS_GETTER("medication.field.partAttributeEnum"), true, nullptr);
	API_DTO_FIELD(String, antibioticCode, ZH_WORDS_GETTER("medication.field.antibioticCode"), false, "KS001");
	API_DTO_FIELD(String, antibioticCode_dictText,
		ZH_WORDS_GETTER("medication.field.antibioticCodeText"), false,
		ZH_WORDS_GETTER("medication.example.antibioticText"));
	API_DTO_FIELD(String, restrictedEnum, ZH_WORDS_GETTER("medication.field.restrictedEnum"), false, "1");
	API_DTO_FIELD(Boolean, selfFlag, ZH_WORDS_GETTER("medication.field.selfFlag"), false, false);
	API_DTO_FIELD(Boolean, antibioticFlag, ZH_WORDS_GETTER("medication.field.antibioticFlag"), false, true);
	API_DTO_FIELD(Boolean, basicFlag, ZH_WORDS_GETTER("medication.field.basicFlag"), false, true);
	API_DTO_FIELD(String, thoPartAttributeEnum,
		ZH_WORDS_GETTER("medication.field.thoPartAttributeEnum"), true, nullptr);
	API_DTO_FIELD(Int32, chrgitmLv, ZH_WORDS_GETTER("medication.field.chrgitmLv"), false, 2);
	API_DTO_FIELD(String, typeCode, ZH_WORDS_GETTER("medication.field.typeCode"), true, nullptr);
	API_DTO_FIELD(String, typeCode_dictText, ZH_WORDS_GETTER("medication.field.typeCodeText"), false,
		ZH_WORDS_GETTER("medication.example.typeText"));
	API_DTO_FIELD(String, ybType, ZH_WORDS_GETTER("medication.field.ybType"), false, "YB001");
	API_DTO_FIELD(Boolean, rxFlag, ZH_WORDS_GETTER("medication.field.rxFlag"), false, true);
	API_DTO_FIELD(String, dosageInstruction, ZH_WORDS_GETTER("medication.field.dosageInstruction"), false,
		ZH_WORDS_GETTER("medication.example.dosageInstruction"));
	API_DTO_FIELD(String, drug69Code, ZH_WORDS_GETTER("medication.field.drug69Code"), false, "6901234567890");
	API_DTO_FIELD(String, totalVolume, ZH_WORDS_GETTER("medication.field.totalVolume"), true, nullptr);
	API_DTO_FIELD(String, doseFormCode, ZH_WORDS_GETTER("medication.field.doseFormCode"), true, nullptr);
	API_DTO_FIELD(String, doseUnitCode, ZH_WORDS_GETTER("medication.field.doseUnitCode"), true, nullptr);
	API_DTO_FIELD(String, doseUnitCode_dictText,
		ZH_WORDS_GETTER("medication.field.doseUnitCodeText"), false,
		ZH_WORDS_GETTER("medication.example.doseUnitText"));
	API_DTO_FIELD(String, packageUnitCode, ZH_WORDS_GETTER("medication.field.packageUnitCode"), true, nullptr);
	API_DTO_FIELD(String, stockUnitCode, ZH_WORDS_GETTER("medication.field.stockUnitCode"), true, nullptr);
	API_DTO_FIELD(String, methodCode, ZH_WORDS_GETTER("medication.field.methodCode"), false, "YF001");
	API_DTO_FIELD(String, rateCode, ZH_WORDS_GETTER("medication.field.rateCode"), false, "PC001");
	API_DTO_FIELD(Float64, dose, ZH_WORDS_GETTER("medication.field.dose"), false, 0.5);
	API_DTO_FIELD(Float64, maxUnit, ZH_WORDS_GETTER("medication.field.maxUnit"), false, 1.0);
	API_DTO_FIELD(Float64, usageLimit, ZH_WORDS_GETTER("medication.field.usageLimit"), false, 2.0);
	API_DTO_FIELD(String, locationId, ZH_WORDS_GETTER("medication.field.locationId"), false, "40001");
	API_DTO_FIELD(String, ingredientItem, ZH_WORDS_GETTER("medication.field.ingredientItem"), false,
		ZH_WORDS_GETTER("medication.example.ingredientItem"));
	API_DTO_FIELD(String, definition, ZH_WORDS_GETTER("medication.field.definition"), false,
		ZH_WORDS_GETTER("medication.example.definition"));
	API_DTO_FIELD(String, dddCode, ZH_WORDS_GETTER("medication.field.dddCode"), false, "DDD001");
	API_DTO_FIELD(String, dddUnitCode, ZH_WORDS_GETTER("medication.field.dddUnitCode"), false, "DW001");
	API_DTO_FIELD(String, minRateCode, ZH_WORDS_GETTER("medication.field.minRateCode"), false, "PC001");
	API_DTO_FIELD(String, maxRateCode, ZH_WORDS_GETTER("medication.field.maxRateCode"), false, "PC002");
	API_DTO_FIELD(Float64, doseUnitRatio, ZH_WORDS_GETTER("medication.field.doseUnitRatio"), true, nullptr);
	API_DTO_FIELD(Float64, purchaseUnitRatio, ZH_WORDS_GETTER("medication.field.purchaseUnitRatio"), true, nullptr);
	API_DTO_FIELD(Float64, stockUnitRatio, ZH_WORDS_GETTER("medication.field.stockUnitRatio"), true, nullptr);
	API_DTO_FIELD(Float64, packageUnitRatio, ZH_WORDS_GETTER("medication.field.packageUnitRatio"), true, nullptr);
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("medication.field.statusEnum"), false, "1");
	API_DTO_FIELD(String, statusEnum_enumText, ZH_WORDS_GETTER("medication.field.statusEnumText"), false,
		ZH_WORDS_GETTER("medication.example.statusEnabled"));
	API_DTO_FIELD(Boolean, activeFlag, ZH_WORDS_GETTER("medication.field.activeFlag"), false, true);
	API_DTO_FIELD(Float64, purchasePrice, ZH_WORDS_GETTER("medication.field.purchasePrice"), true, nullptr);
	API_DTO_FIELD(Float64, retailPrice, ZH_WORDS_GETTER("medication.field.retailPrice"), true, nullptr);
	API_DTO_FIELD(Float64, maximumRetailPrice,
		ZH_WORDS_GETTER("medication.field.maximumRetailPrice"), false, 18.0);
};

class MedicationCatalogModifyDTO : public MedicationCatalogBaseDTO
{
	DTO_INIT(MedicationCatalogModifyDTO, MedicationCatalogBaseDTO);

	API_DTO_FIELD(String, id, ZH_WORDS_GETTER("medication.field.id"), true, nullptr);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICATION_CATALOG_DTO_H_
