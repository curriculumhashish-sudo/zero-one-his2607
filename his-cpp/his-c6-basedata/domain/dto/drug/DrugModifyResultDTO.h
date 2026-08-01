#pragma once
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
#ifndef _DRUG_MODIFY_RESULT_DTO_H_
#define _DRUG_MODIFY_RESULT_DTO_H_

#include "../../GlobalInclude.h"
#include "DrugUnitBandDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DrugModifyResultDTO : public oatpp::DTO
{
    DTO_INIT(DrugModifyResultDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, orgId, ZH_WORDS_GETTER("drug.field.orgId"), false);
    API_DTO_FIELD_REQUIRE(String, busNo, ZH_WORDS_GETTER("drug.field.busNo"), false);
    API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("drug.field.name"), true);
    API_DTO_FIELD_REQUIRE(String, domainEnum, ZH_WORDS_GETTER("drug.field.domainEnum"), false);
    DTO_FIELD(String, medVersion, "version");
    DTO_FIELD_INFO(medVersion)
    {
        info->description = ZH_WORDS_GETTER("drug.field.medVersion");
        info->required = false;
    }
    API_DTO_FIELD_REQUIRE(String, nameEn, ZH_WORDS_GETTER("drug.field.nameEn"), false);
    API_DTO_FIELD_REQUIRE(String, pyStr, ZH_WORDS_GETTER("drug.field.pyStr"), false);
    API_DTO_FIELD_REQUIRE(String, wbStr, ZH_WORDS_GETTER("drug.field.wbStr"), false);
    API_DTO_FIELD_REQUIRE(String, categoryCode, ZH_WORDS_GETTER("drug.field.categoryCode"), true);
    API_DTO_FIELD_REQUIRE(String, merchandiseName, ZH_WORDS_GETTER("drug.field.merchandiseName"), true);
    API_DTO_FIELD_REQUIRE(String, merchandisePyStr, ZH_WORDS_GETTER("drug.field.merchandisePyStr"), false);
    API_DTO_FIELD_REQUIRE(String, merchandiseWbStr, ZH_WORDS_GETTER("drug.field.merchandiseWbStr"), false);
    API_DTO_FIELD_REQUIRE(String, purchaseUnitCode, ZH_WORDS_GETTER("drug.field.purchaseUnitCode"), true);
    API_DTO_FIELD_REQUIRE(String, splitUnitCode, ZH_WORDS_GETTER("drug.field.splitUnitCode"), true);
    API_DTO_FIELD_REQUIRE(String, comprisedText, ZH_WORDS_GETTER("drug.field.comprisedText"), false);
    API_DTO_FIELD_REQUIRE(String, ingredient, ZH_WORDS_GETTER("drug.field.ingredient"), false);
    API_DTO_FIELD_REQUIRE(Int32, doseFrom, ZH_WORDS_GETTER("drug.field.doseFrom"), false);
    API_DTO_FIELD_REQUIRE(String, approvalNumber, ZH_WORDS_GETTER("drug.field.approvalNumber"), false);
    API_DTO_FIELD_REQUIRE(Boolean, ybMatchFlag, ZH_WORDS_GETTER("drug.field.ybMatchFlag"), false);
    API_DTO_FIELD_REQUIRE(String, ybNo, ZH_WORDS_GETTER("drug.field.ybNo"), false);
    API_DTO_FIELD_REQUIRE(String, pharmacologyCategoryCode, ZH_WORDS_GETTER("drug.field.pharmacologyCategoryCode"),
                          true);
    API_DTO_FIELD_REQUIRE(Boolean, skinTestFlag, ZH_WORDS_GETTER("drug.field.skinTestFlag"), false);
    API_DTO_FIELD_REQUIRE(Boolean, injectFlag, ZH_WORDS_GETTER("drug.field.injectFlag"), false);
    API_DTO_FIELD_REQUIRE(String, manufacturerId, ZH_WORDS_GETTER("drug.field.manufacturerId"), false);
    API_DTO_FIELD_REQUIRE(String, manufacturerText, ZH_WORDS_GETTER("drug.field.manufacturerText"), true);
    API_DTO_FIELD_REQUIRE(String, supplyId, ZH_WORDS_GETTER("drug.field.supplyId"), true);
    API_DTO_FIELD_REQUIRE(Boolean, restrictedFlag, ZH_WORDS_GETTER("drug.field.restrictedFlag"), false);
    API_DTO_FIELD_REQUIRE(String, restrictedScope, ZH_WORDS_GETTER("drug.field.restrictedScope"), false);
    API_DTO_FIELD_REQUIRE(Boolean, childrenFlag, ZH_WORDS_GETTER("drug.field.childrenFlag"), false);
    API_DTO_FIELD_REQUIRE(Int32, characteristic, ZH_WORDS_GETTER("drug.field.characteristic"), false);
    API_DTO_FIELD_REQUIRE(Float64, itemMinQuantity, ZH_WORDS_GETTER("drug.field.itemMinQuantity"), false);
    API_DTO_FIELD_REQUIRE(Float64, itemMaxQuantity, ZH_WORDS_GETTER("drug.field.itemMaxQuantity"), false);
    API_DTO_FIELD_REQUIRE(String, nationalDrugCode, ZH_WORDS_GETTER("drug.field.nationalDrugCode"), false);
    API_DTO_FIELD_REQUIRE(String, partAttributeEnum, ZH_WORDS_GETTER("drug.field.partAttributeEnum"), true);
    API_DTO_FIELD_REQUIRE(String, antibioticCode, ZH_WORDS_GETTER("drug.field.antibioticCode"), false);
    API_DTO_FIELD_REQUIRE(String, restrictedEnum, ZH_WORDS_GETTER("drug.field.restrictedEnum"), false);
    API_DTO_FIELD_REQUIRE(Boolean, selfFlag, ZH_WORDS_GETTER("drug.field.selfFlag"), false);
    API_DTO_FIELD_REQUIRE(Boolean, antibioticFlag, ZH_WORDS_GETTER("drug.field.antibioticFlag"), false);
    API_DTO_FIELD_REQUIRE(Boolean, basicFlag, ZH_WORDS_GETTER("drug.field.basicFlag"), false);
    API_DTO_FIELD_REQUIRE(String, thoPartAttributeEnum, ZH_WORDS_GETTER("drug.field.thoPartAttributeEnum"), true);
    API_DTO_FIELD_REQUIRE(Int32, chrgitmLv, ZH_WORDS_GETTER("drug.field.chrgitmLv"), false);
    API_DTO_FIELD_REQUIRE(String, typeCode, ZH_WORDS_GETTER("drug.field.typeCode"), true);
    API_DTO_FIELD_REQUIRE(String, ybType, ZH_WORDS_GETTER("drug.field.ybType"), false);
    API_DTO_FIELD_REQUIRE(Boolean, rxFlag, ZH_WORDS_GETTER("drug.field.rxFlag"), false);
    API_DTO_FIELD_REQUIRE(String, dosageInstruction, ZH_WORDS_GETTER("drug.field.dosageInstruction"), false);
    API_DTO_FIELD_REQUIRE(String, drug69Code, ZH_WORDS_GETTER("drug.field.drug69Code"), false);
    API_DTO_FIELD_REQUIRE(String, totalVolume, ZH_WORDS_GETTER("drug.field.totalVolume"), true);
    API_DTO_FIELD_REQUIRE(String, doseFormCode, ZH_WORDS_GETTER("drug.field.doseFormCode"), true);
    API_DTO_FIELD_REQUIRE(String, doseUnitCode, ZH_WORDS_GETTER("drug.field.doseUnitCode"), true);
    API_DTO_FIELD_REQUIRE(String, packageUnitCode, ZH_WORDS_GETTER("drug.field.packageUnitCode"), true);
    API_DTO_FIELD_REQUIRE(String, stockUnitCode, ZH_WORDS_GETTER("drug.field.stockUnitCode"), true);
    API_DTO_FIELD_REQUIRE(String, methodCode, ZH_WORDS_GETTER("drug.field.methodCode"), false);
    API_DTO_FIELD_REQUIRE(String, rateCode, ZH_WORDS_GETTER("drug.field.rateCode"), false);
    API_DTO_FIELD_REQUIRE(Float64, dose, ZH_WORDS_GETTER("drug.field.dose"), false);
    API_DTO_FIELD_REQUIRE(Float64, maxUnit, ZH_WORDS_GETTER("drug.field.maxUnit"), false);
    API_DTO_FIELD_REQUIRE(Float64, usageLimit, ZH_WORDS_GETTER("drug.field.usageLimit"), false);
    API_DTO_FIELD_REQUIRE(String, locationId, ZH_WORDS_GETTER("drug.field.locationId"), false);
    API_DTO_FIELD_REQUIRE(String, ingredientItem, ZH_WORDS_GETTER("drug.field.ingredientItem"), false);
    API_DTO_FIELD_REQUIRE(String, definition, ZH_WORDS_GETTER("drug.field.definition"), false);
    API_DTO_FIELD_REQUIRE(String, dddCode, ZH_WORDS_GETTER("drug.field.dddCode"), false);
    API_DTO_FIELD_REQUIRE(String, dddUnitCode, ZH_WORDS_GETTER("drug.field.dddUnitCode"), false);
    API_DTO_FIELD_REQUIRE(String, minRateCode, ZH_WORDS_GETTER("drug.field.minRateCode"), false);
    API_DTO_FIELD_REQUIRE(String, maxRateCode, ZH_WORDS_GETTER("drug.field.maxRateCode"), false);
    API_DTO_FIELD_REQUIRE(Float64, doseUnitRatio, ZH_WORDS_GETTER("drug.field.doseUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, purchaseUnitRatio, ZH_WORDS_GETTER("drug.field.purchaseUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, stockUnitRatio, ZH_WORDS_GETTER("drug.field.stockUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, packageUnitRatio, ZH_WORDS_GETTER("drug.field.packageUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, purchasePrice, ZH_WORDS_GETTER("drug.field.purchasePrice"), true);
    API_DTO_FIELD_REQUIRE(Float64, retailPrice, ZH_WORDS_GETTER("drug.field.retailPrice"), true);
    API_DTO_FIELD_DEFAULT(Float64, maximumRetailPrice, ZH_WORDS_GETTER("drug.field.maximumRetailPrice"));
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("drug.field.id"), true);
    DTO_FIELD(String, medicationDefId, "medicationDefId");
    DTO_FIELD_INFO(medicationDefId)
    {
        info->description = ZH_WORDS_GETTER("drug.field.drugDefId");
        info->required = false;
    }
    DTO_FIELD(String, purchaseUnitCodeDictText, "purchaseUnitCode_dictText");
    DTO_FIELD_INFO(purchaseUnitCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.purchaseUnitCodeText");
        info->required = false;
    }
    DTO_FIELD(String, doseFromDictText, "doseFrom_dictText");
    DTO_FIELD_INFO(doseFromDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.doseFromText");
        info->required = false;
    }
    DTO_FIELD(String, antibioticCodeDictText, "antibioticCode_dictText");
    DTO_FIELD_INFO(antibioticCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.antibioticCodeText");
        info->required = false;
    }
    DTO_FIELD(String, typeCodeDictText, "typeCode_dictText");
    DTO_FIELD_INFO(typeCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.typeCodeText");
        info->required = false;
    }
    DTO_FIELD(String, doseUnitCodeDictText, "doseUnitCode_dictText");
    DTO_FIELD_INFO(doseUnitCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.doseUnitCodeText");
        info->required = false;
    }
    API_DTO_FIELD_REQUIRE(String, statusEnum, ZH_WORDS_GETTER("drug.field.statusEnum"), false);
    DTO_FIELD(String, statusEnumText, "statusEnum_enumText");
    DTO_FIELD_INFO(statusEnumText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.statusEnumText");
        info->required = false;
    }
    API_DTO_FIELD_REQUIRE(Boolean, activeFlag, ZH_WORDS_GETTER("drug.field.activeFlag"), false);
    API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("drug.field.specification"), false);
    API_DTO_FIELD_REQUIRE(String, activeFlagText, ZH_WORDS_GETTER("drug.field.activeFlagText"), false);
    DTO_FIELD(String, categoryCodeDictText, "categoryCode_dictText");
    DTO_FIELD_INFO(categoryCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.categoryCodeText");
        info->required = false;
    }
    DTO_FIELD(String, packageUnitCodeDictText, "packageUnitCode_dictText");
    DTO_FIELD_INFO(packageUnitCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.packageUnitCodeText");
        info->required = false;
    }
    DTO_FIELD(String, locationIdDictText, "locationId_dictText");
    DTO_FIELD_INFO(locationIdDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.locationIdText");
        info->required = false;
    }
    DTO_FIELD(String, supplyIdDictText, "supplyId_dictText");
    DTO_FIELD_INFO(supplyIdDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.supplyIdText");
        info->required = false;
    }
    API_DTO_FIELD_REQUIRE(Boolean, potencyFlag, ZH_WORDS_GETTER("drug.field.potencyFlag"), false);
    API_DTO_FIELD_REQUIRE(List<DrugUnitBandDTO::Wrapper>, unitBands, ZH_WORDS_GETTER("drug.field.unitBands"), false);

  public:
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->orgId = "322663105141149696";
        dto->medicationDefId = "990000000000000005";
        dto->busNo = "MD0021";
        dto->name = ZH_WORDS_GETTER("drug.example.name");
        dto->medVersion = "1";
        dto->nameEn = "Amoxicillin Capsules";
        dto->pyStr = "AMXLJN";
        dto->wbStr = "SGRM";
        dto->categoryCode = "2";
        dto->merchandiseName = ZH_WORDS_GETTER("drug.example.merchandiseName");
        dto->merchandisePyStr = "AMX";
        dto->merchandiseWbStr = "SGR";
        dto->purchaseUnitCode = "2";
        dto->purchaseUnitCodeDictText = ZH_WORDS_GETTER("drug.example.purchaseUnitText");
        dto->splitUnitCode = "5";
        dto->comprisedText = ZH_WORDS_GETTER("drug.example.comprisedText");
        dto->ingredient = ZH_WORDS_GETTER("drug.example.ingredient");
        dto->doseFrom = 1;
        dto->doseFromDictText = ZH_WORDS_GETTER("drug.example.doseFormText");
        dto->approvalNumber = ZH_WORDS_GETTER("drug.example.approvalNumber");
        dto->ybMatchFlag = true;
        dto->ybNo = "YB2025001";
        dto->pharmacologyCategoryCode = "1";
        dto->skinTestFlag = false;
        dto->injectFlag = false;
        dto->manufacturerText = ZH_WORDS_GETTER("drug.example.manufacturerText");
        dto->supplyId = "4100000000000000004";
        dto->restrictedFlag = false;
        dto->restrictedScope = ZH_WORDS_GETTER("drug.example.restrictedScope");
        dto->childrenFlag = true;
        dto->characteristic = 1;
        dto->itemMinQuantity = 10.0;
        dto->itemMaxQuantity = 100.0;
        dto->nationalDrugCode = "860000123456789";
        dto->partAttributeEnum = "2";
        dto->restrictedEnum = "1";
        dto->selfFlag = false;
        dto->antibioticFlag = true;
        dto->basicFlag = true;
        dto->thoPartAttributeEnum = "2";
        dto->chrgitmLv = 2;
        dto->typeCode = "1001";
        dto->typeCodeDictText = ZH_WORDS_GETTER("drug.example.typeText");
        dto->rxFlag = true;
        dto->dosageInstruction = ZH_WORDS_GETTER("drug.example.dosageInstruction");
        dto->drug69Code = "6901234567890";
        dto->totalVolume = "5ml:0.5g";
        dto->doseFormCode = "5";
        dto->doseUnitCode = "10";
        dto->doseUnitCodeDictText = ZH_WORDS_GETTER("drug.example.doseUnitText");
        dto->packageUnitCode = "2";
        dto->stockUnitCode = "2";
        dto->dose = 0.5;
        dto->maxUnit = 1.0;
        dto->usageLimit = 2.0;
        dto->ingredientItem = ZH_WORDS_GETTER("drug.example.ingredientItem");
        dto->definition = ZH_WORDS_GETTER("drug.example.definition");
        dto->doseUnitRatio = 0.1;
        dto->purchaseUnitRatio = 20.0;
        dto->stockUnitRatio = 20.0;
        dto->packageUnitRatio = 20.0;
        dto->statusEnum = "1";
        dto->statusEnumText = ZH_WORDS_GETTER("drug.example.statusEnabled");
        dto->activeFlag = true;
        dto->purchasePrice = 6.99;
        dto->retailPrice = 6.99;
        dto->maximumRetailPrice = 6.99;
        dto->id = "990000000000000005";
        dto->specification = dto->totalVolume;
        dto->activeFlagText = ZH_WORDS_GETTER("drug.example.statusEnabled");
        dto->categoryCodeDictText = ZH_WORDS_GETTER("drug.example.typeText");
        dto->packageUnitCodeDictText = ZH_WORDS_GETTER("drug.example.purchaseUnitText");
        dto->supplyIdDictText = ZH_WORDS_GETTER("drug.example.supplierText");
        dto->potencyFlag = false;
        dto->unitBands = oatpp::List<DrugUnitBandDTO::Wrapper>::createShared();
        dto->unitBands->push_back(DrugUnitBandDTO::createExample());
        return dto;
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DRUG_MODIFY_RESULT_DTO_H_
