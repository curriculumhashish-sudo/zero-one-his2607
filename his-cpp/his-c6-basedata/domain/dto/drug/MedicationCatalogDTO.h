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

class MedicationCatalogAddDTO : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogAddDTO, DTO);

	API_DTO_FIELD(String, tenantId, ZH_WORDS_GETTER("medication.field.tenantId"), false, "1");
	API_DTO_FIELD(String, orgId, ZH_WORDS_GETTER("medication.field.orgId"), false, "322663105141149696");
	API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("medication.field.busNo"), false, "MD0021");
	API_DTO_FIELD(String, name, ZH_WORDS_GETTER("medication.field.name"), true, u8"\u5934\u5B62\u514B\u809F\u80F6\u56CA");
	API_DTO_FIELD(String, domainEnum, ZH_WORDS_GETTER("medication.field.domainEnum"), false, "WESTERN_MEDICINE");
	API_DTO_FIELD(String, nameEn, ZH_WORDS_GETTER("medication.field.nameEn"), false, "Cefixime Capsules");
	API_DTO_FIELD(String, pyStr, ZH_WORDS_GETTER("medication.field.pyStr"), false, "TBKWTJN");
	API_DTO_FIELD(String, wbStr, ZH_WORDS_GETTER("medication.field.wbStr"), false, "UKTQ");
	API_DTO_FIELD(String, categoryCode, ZH_WORDS_GETTER("medication.field.categoryCode"), true, "2");
	API_DTO_FIELD(String, merchandiseName, ZH_WORDS_GETTER("medication.field.merchandiseName"), true, u8"\u5934\u5B62\u514B\u809F\u80F6\u56CA");
	API_DTO_FIELD(String, merchandisePyStr, ZH_WORDS_GETTER("medication.field.merchandisePyStr"), false, "TBKWTJN");
	API_DTO_FIELD(String, merchandiseWbStr, ZH_WORDS_GETTER("medication.field.merchandiseWbStr"), false, "UKTQ");
	API_DTO_FIELD(String, purchaseUnitCode, ZH_WORDS_GETTER("medication.field.purchaseUnitCode"), true, "2");
	API_DTO_FIELD(String, splitUnitCode, ZH_WORDS_GETTER("medication.field.splitUnitCode"), true, "5");
	API_DTO_FIELD(String, comprisedText, ZH_WORDS_GETTER("medication.field.comprisedText"), false, u8"\u6BCF\u7C92\u542B\u5934\u5B62\u514B\u809F0.1g");
	API_DTO_FIELD(String, ingredient, ZH_WORDS_GETTER("medication.field.ingredient"), false, u8"\u5934\u5B62\u514B\u809F");
	API_DTO_FIELD(Int32, doseFrom, ZH_WORDS_GETTER("medication.field.doseFrom"), false, 1);
	API_DTO_FIELD(String, approvalNumber, ZH_WORDS_GETTER("medication.field.approvalNumber"), false, u8"\u56FD\u836F\u51C6\u5B57H20041656");
	API_DTO_FIELD(Boolean, ybMatchFlag, ZH_WORDS_GETTER("medication.field.ybMatchFlag"), false, false);
	API_DTO_FIELD(String, ybNo, ZH_WORDS_GETTER("medication.field.ybNo"), false, "YA01BAA0001001010101");
	API_DTO_FIELD(String, pharmacologyCategoryCode, ZH_WORDS_GETTER("medication.field.pharmacologyCategoryCode"), true, "1");
	API_DTO_FIELD(Boolean, skinTestFlag, ZH_WORDS_GETTER("medication.field.skinTestFlag"), false, false);
	API_DTO_FIELD(Boolean, injectFlag, ZH_WORDS_GETTER("medication.field.injectFlag"), false, false);
	API_DTO_FIELD(String, manufacturerId, ZH_WORDS_GETTER("medication.field.manufacturerId"), false, "4100000000000000001");
	API_DTO_FIELD(String, manufacturerText, ZH_WORDS_GETTER("medication.field.manufacturerText"), true, u8"\u534E\u5317\u5236\u836F\u6CB3\u5317\u534E\u6C11\u836F\u4E1A\u6709\u9650\u8D23\u4EFB\u516C\u53F8");
	API_DTO_FIELD(String, supplyId, ZH_WORDS_GETTER("medication.field.supplyId"), true, "4100000000000000004");
	API_DTO_FIELD(Boolean, restrictedFlag, ZH_WORDS_GETTER("medication.field.restrictedFlag"), false, false);
	API_DTO_FIELD(String, restrictedScope, ZH_WORDS_GETTER("medication.field.restrictedScope"), false, "");
	API_DTO_FIELD(Boolean, childrenFlag, ZH_WORDS_GETTER("medication.field.childrenFlag"), false, false);
	API_DTO_FIELD(Int32, characteristic, ZH_WORDS_GETTER("medication.field.characteristic"), false, 1);
	API_DTO_FIELD(Float64, itemMinQuantity, ZH_WORDS_GETTER("medication.field.itemMinQuantity"), false, 10.0);
	API_DTO_FIELD(Float64, itemMaxQuantity, ZH_WORDS_GETTER("medication.field.itemMaxQuantity"), false, 1000.0);
	API_DTO_FIELD(String, defEncounterUnitCode, ZH_WORDS_GETTER("medication.field.defEncounterUnitCode"), false, "5");
	API_DTO_FIELD(String, defInhospitalUnitCode, ZH_WORDS_GETTER("medication.field.defInhospitalUnitCode"), false, "5");
	API_DTO_FIELD(String, nationalDrugCode, ZH_WORDS_GETTER("medication.field.nationalDrugCode"), false, "86902680000021");
	API_DTO_FIELD(String, partAttributeEnum, ZH_WORDS_GETTER("medication.field.partAttributeEnum"), true, "2");
	API_DTO_FIELD(String, antibioticCode, ZH_WORDS_GETTER("medication.field.antibioticCode"), false, "J01DD");
	API_DTO_FIELD(String, restrictedEnum, ZH_WORDS_GETTER("medication.field.restrictedEnum"), false, "NONE");
	API_DTO_FIELD(Boolean, selfFlag, ZH_WORDS_GETTER("medication.field.selfFlag"), false, false);
	API_DTO_FIELD(Boolean, antibioticFlag, ZH_WORDS_GETTER("medication.field.antibioticFlag"), false, false);
	API_DTO_FIELD(Boolean, basicFlag, ZH_WORDS_GETTER("medication.field.basicFlag"), false, false);
	API_DTO_FIELD(String, thoPartAttributeEnum, ZH_WORDS_GETTER("medication.field.thoPartAttributeEnum"), true, "2");
	API_DTO_FIELD(Int32, chrgitmLv, ZH_WORDS_GETTER("medication.field.chrgitmLv"), false, 1);
	API_DTO_FIELD(String, typeCode, ZH_WORDS_GETTER("medication.field.typeCode"), true, "1001");
	API_DTO_FIELD(String, ybType, ZH_WORDS_GETTER("medication.field.ybType"), false, "1");
	API_DTO_FIELD(Boolean, rxFlag, ZH_WORDS_GETTER("medication.field.rxFlag"), false, false);
	API_DTO_FIELD(String, dosageInstruction, ZH_WORDS_GETTER("medication.field.dosageInstruction"), false, "1");
	API_DTO_FIELD(String, drug69Code, ZH_WORDS_GETTER("medication.field.drug69Code"), false, "6900000000021");
	API_DTO_FIELD(String, totalVolume, ZH_WORDS_GETTER("medication.field.totalVolume"), true, "0.1g");
	API_DTO_FIELD(String, doseFormCode, ZH_WORDS_GETTER("medication.field.doseFormCode"), true, "5");
	API_DTO_FIELD(String, doseUnitCode, ZH_WORDS_GETTER("medication.field.doseUnitCode"), true, "10");
	API_DTO_FIELD(String, packageUnitCode, ZH_WORDS_GETTER("medication.field.packageUnitCode"), true, "2");
	API_DTO_FIELD(String, stockUnitCode, ZH_WORDS_GETTER("medication.field.stockUnitCode"), true, "2");
	API_DTO_FIELD(String, methodCode, ZH_WORDS_GETTER("medication.field.methodCode"), false, "1");
	API_DTO_FIELD(String, rateCode, ZH_WORDS_GETTER("medication.field.rateCode"), false, "BID");
	API_DTO_FIELD(Float64, dose, ZH_WORDS_GETTER("medication.field.dose"), false, 0.1);
	API_DTO_FIELD(Float64, maxUnit, ZH_WORDS_GETTER("medication.field.maxUnit"), false, 2.0);
	API_DTO_FIELD(Float64, usageLimit, ZH_WORDS_GETTER("medication.field.usageLimit"), false, 7.0);
	API_DTO_FIELD(String, locationId, ZH_WORDS_GETTER("medication.field.locationId"), false, "322663105141149697");
	API_DTO_FIELD(String, ingredientItem, ZH_WORDS_GETTER("medication.field.ingredientItem"), false, u8"\u5934\u5B62\u514B\u809F");
	API_DTO_FIELD(String, definition, ZH_WORDS_GETTER("medication.field.definition"), false, u8"\u7B2C\u4E09\u4EE3\u5934\u5B62\u83CC\u7D20\u7C7B\u6297\u751F\u7D20");
	API_DTO_FIELD(String, dddCode, ZH_WORDS_GETTER("medication.field.dddCode"), false, "J01DD08");
	API_DTO_FIELD(String, dddUnitCode, ZH_WORDS_GETTER("medication.field.dddUnitCode"), false, "g");
	API_DTO_FIELD(String, minRateCode, ZH_WORDS_GETTER("medication.field.minRateCode"), false, "QD");
	API_DTO_FIELD(String, maxRateCode, ZH_WORDS_GETTER("medication.field.maxRateCode"), false, "TID");
	API_DTO_FIELD(Float64, doseUnitRatio, ZH_WORDS_GETTER("medication.field.doseUnitRatio"), true, 0.1);
	API_DTO_FIELD(Float64, purchaseUnitRatio, ZH_WORDS_GETTER("medication.field.purchaseUnitRatio"), true, 20.0);
	API_DTO_FIELD(Float64, stockUnitRatio, ZH_WORDS_GETTER("medication.field.stockUnitRatio"), true, 20.0);
	API_DTO_FIELD(Float64, packageUnitRatio, ZH_WORDS_GETTER("medication.field.packageUnitRatio"), true, 20.0);
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("medication.field.statusEnum"), false, "ACTIVE");
	API_DTO_FIELD(Boolean, activeFlag, ZH_WORDS_GETTER("medication.field.activeFlag"), false, true);
	API_DTO_FIELD(Float64, purchasePrice, ZH_WORDS_GETTER("medication.field.purchasePrice"), true, 6.99);
	API_DTO_FIELD(Float64, retailPrice, ZH_WORDS_GETTER("medication.field.retailPrice"), true, 6.99);
	API_DTO_FIELD(Float64, maximumRetailPrice, ZH_WORDS_GETTER("medication.field.maximumRetailPrice"), false, 28.0);
};

class MedicationCatalogModifyDTO : public MedicationCatalogAddDTO
{
	DTO_INIT(MedicationCatalogModifyDTO, MedicationCatalogAddDTO);

	API_DTO_FIELD(String, id, ZH_WORDS_GETTER("medication.field.id"), true, "322994171216400384");
	API_DTO_FIELD(String, version, ZH_WORDS_GETTER("medication.field.version"), false, "1");
};

class MedicationCatalogInitDTO : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogInitDTO, DTO);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICATION_CATALOG_DTO_H_
