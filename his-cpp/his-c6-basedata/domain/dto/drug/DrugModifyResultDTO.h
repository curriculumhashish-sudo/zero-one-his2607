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

    API_DTO_FIELD(String, orgId, ZH_WORDS_GETTER("drug.field.orgId"), false, "30001");
    API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("drug.field.busNo"), false, "YP20250409001");
    API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("drug.field.name"), true);
    API_DTO_FIELD(String, domainEnum, ZH_WORDS_GETTER("drug.field.domainEnum"), false, "3");
    API_DTO_FIELD(String, version, ZH_WORDS_GETTER("drug.field.medVersion"), false, "V1.0");
    API_DTO_FIELD(String, nameEn, ZH_WORDS_GETTER("drug.field.nameEn"), false, "Amoxicillin Capsules");
    API_DTO_FIELD(String, pyStr, ZH_WORDS_GETTER("drug.field.pyStr"), false, "AMXLJN");
    API_DTO_FIELD(String, wbStr, ZH_WORDS_GETTER("drug.field.wbStr"), false, "SGRM");
    API_DTO_FIELD_REQUIRE(String, categoryCode, ZH_WORDS_GETTER("drug.field.categoryCode"), true);
    API_DTO_FIELD_REQUIRE(String, merchandiseName, ZH_WORDS_GETTER("drug.field.merchandiseName"), true);
    API_DTO_FIELD(String, merchandisePyStr, ZH_WORDS_GETTER("drug.field.merchandisePyStr"), false, "AMX");
    API_DTO_FIELD(String, merchandiseWbStr, ZH_WORDS_GETTER("drug.field.merchandiseWbStr"), false, "SGR");
    API_DTO_FIELD_REQUIRE(String, purchaseUnitCode, ZH_WORDS_GETTER("drug.field.purchaseUnitCode"), true);
    API_DTO_FIELD_REQUIRE(String, splitUnitCode, ZH_WORDS_GETTER("drug.field.splitUnitCode"), true);
    API_DTO_FIELD(String, comprisedText, ZH_WORDS_GETTER("drug.field.comprisedText"), false,
                  ZH_WORDS_GETTER("drug.example.comprisedText"));
    API_DTO_FIELD(String, ingredient, ZH_WORDS_GETTER("drug.field.ingredient"), false,
                  ZH_WORDS_GETTER("drug.example.ingredient"));
    API_DTO_FIELD(Int32, doseFrom, ZH_WORDS_GETTER("drug.field.doseFrom"), false, 1);
    API_DTO_FIELD(String, approvalNumber, ZH_WORDS_GETTER("drug.field.approvalNumber"), false,
                  ZH_WORDS_GETTER("drug.example.approvalNumber"));
    API_DTO_FIELD(Boolean, ybMatchFlag, ZH_WORDS_GETTER("drug.field.ybMatchFlag"), false, true);
    API_DTO_FIELD(String, ybNo, ZH_WORDS_GETTER("drug.field.ybNo"), false, "YB2025001");
    API_DTO_FIELD_REQUIRE(String, pharmacologyCategoryCode, ZH_WORDS_GETTER("drug.field.pharmacologyCategoryCode"),
                          true);
    API_DTO_FIELD(Boolean, skinTestFlag, ZH_WORDS_GETTER("drug.field.skinTestFlag"), false, false);
    API_DTO_FIELD(Boolean, injectFlag, ZH_WORDS_GETTER("drug.field.injectFlag"), false, false);
    API_DTO_FIELD(String, manufacturerId, ZH_WORDS_GETTER("drug.field.manufacturerId"), false, "50001");
    API_DTO_FIELD_REQUIRE(String, manufacturerText, ZH_WORDS_GETTER("drug.field.manufacturerText"), true);
    API_DTO_FIELD_REQUIRE(String, supplyId, ZH_WORDS_GETTER("drug.field.supplyId"), true);
    API_DTO_FIELD(Boolean, restrictedFlag, ZH_WORDS_GETTER("drug.field.restrictedFlag"), false, false);
    API_DTO_FIELD(String, restrictedScope, ZH_WORDS_GETTER("drug.field.restrictedScope"), false,
                  ZH_WORDS_GETTER("drug.example.restrictedScope"));
    API_DTO_FIELD(Boolean, childrenFlag, ZH_WORDS_GETTER("drug.field.childrenFlag"), false, true);
    API_DTO_FIELD(Int32, characteristic, ZH_WORDS_GETTER("drug.field.characteristic"), false, 1);
    API_DTO_FIELD(Float64, itemMinQuantity, ZH_WORDS_GETTER("drug.field.itemMinQuantity"), false, 10.0);
    API_DTO_FIELD(Float64, itemMaxQuantity, ZH_WORDS_GETTER("drug.field.itemMaxQuantity"), false, 100.0);
    API_DTO_FIELD(String, nationalDrugCode, ZH_WORDS_GETTER("drug.field.nationalDrugCode"), false,
                  "860000123456789");
    API_DTO_FIELD_REQUIRE(String, partAttributeEnum, ZH_WORDS_GETTER("drug.field.partAttributeEnum"), true);
    API_DTO_FIELD(String, antibioticCode, ZH_WORDS_GETTER("drug.field.antibioticCode"), false, "KS001");
    API_DTO_FIELD(String, restrictedEnum, ZH_WORDS_GETTER("drug.field.restrictedEnum"), false, "1");
    API_DTO_FIELD(Boolean, selfFlag, ZH_WORDS_GETTER("drug.field.selfFlag"), false, false);
    API_DTO_FIELD(Boolean, antibioticFlag, ZH_WORDS_GETTER("drug.field.antibioticFlag"), false, true);
    API_DTO_FIELD(Boolean, basicFlag, ZH_WORDS_GETTER("drug.field.basicFlag"), false, true);
    API_DTO_FIELD_REQUIRE(String, thoPartAttributeEnum, ZH_WORDS_GETTER("drug.field.thoPartAttributeEnum"), true);
    API_DTO_FIELD(Int32, chrgitmLv, ZH_WORDS_GETTER("drug.field.chrgitmLv"), false, 2);
    API_DTO_FIELD_REQUIRE(String, typeCode, ZH_WORDS_GETTER("drug.field.typeCode"), true);
    API_DTO_FIELD(String, ybType, ZH_WORDS_GETTER("drug.field.ybType"), false, "YB001");
    API_DTO_FIELD(Boolean, rxFlag, ZH_WORDS_GETTER("drug.field.rxFlag"), false, true);
    API_DTO_FIELD(String, dosageInstruction, ZH_WORDS_GETTER("drug.field.dosageInstruction"), false,
                  ZH_WORDS_GETTER("drug.example.dosageInstruction"));
    API_DTO_FIELD(String, drug69Code, ZH_WORDS_GETTER("drug.field.drug69Code"), false, "6901234567890");
    API_DTO_FIELD_REQUIRE(String, totalVolume, ZH_WORDS_GETTER("drug.field.totalVolume"), true);
    API_DTO_FIELD_REQUIRE(String, doseFormCode, ZH_WORDS_GETTER("drug.field.doseFormCode"), true);
    API_DTO_FIELD_REQUIRE(String, doseUnitCode, ZH_WORDS_GETTER("drug.field.doseUnitCode"), true);
    API_DTO_FIELD_REQUIRE(String, packageUnitCode, ZH_WORDS_GETTER("drug.field.packageUnitCode"), true);
    API_DTO_FIELD_REQUIRE(String, stockUnitCode, ZH_WORDS_GETTER("drug.field.stockUnitCode"), true);
    API_DTO_FIELD(String, methodCode, ZH_WORDS_GETTER("drug.field.methodCode"), false, "YF001");
    API_DTO_FIELD(String, rateCode, ZH_WORDS_GETTER("drug.field.rateCode"), false, "PC001");
    API_DTO_FIELD(Float64, dose, ZH_WORDS_GETTER("drug.field.dose"), false, 0.5);
    API_DTO_FIELD(Float64, maxUnit, ZH_WORDS_GETTER("drug.field.maxUnit"), false, 1.0);
    API_DTO_FIELD(Float64, usageLimit, ZH_WORDS_GETTER("drug.field.usageLimit"), false, 2.0);
    API_DTO_FIELD(String, locationId, ZH_WORDS_GETTER("drug.field.locationId"), false, "40001");
    API_DTO_FIELD(String, ingredientItem, ZH_WORDS_GETTER("drug.field.ingredientItem"), false,
                  ZH_WORDS_GETTER("drug.example.ingredientItem"));
    API_DTO_FIELD(String, definition, ZH_WORDS_GETTER("drug.field.definition"), false,
                  ZH_WORDS_GETTER("drug.example.definition"));
    API_DTO_FIELD(String, dddCode, ZH_WORDS_GETTER("drug.field.dddCode"), false, "DDD001");
    API_DTO_FIELD(String, dddUnitCode, ZH_WORDS_GETTER("drug.field.dddUnitCode"), false, "DW001");
    API_DTO_FIELD(String, minRateCode, ZH_WORDS_GETTER("drug.field.minRateCode"), false, "PC001");
    API_DTO_FIELD(String, maxRateCode, ZH_WORDS_GETTER("drug.field.maxRateCode"), false, "PC002");
    API_DTO_FIELD_REQUIRE(Float64, doseUnitRatio, ZH_WORDS_GETTER("drug.field.doseUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, purchaseUnitRatio, ZH_WORDS_GETTER("drug.field.purchaseUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, stockUnitRatio, ZH_WORDS_GETTER("drug.field.stockUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, packageUnitRatio, ZH_WORDS_GETTER("drug.field.packageUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, purchasePrice, ZH_WORDS_GETTER("drug.field.purchasePrice"), true);
    API_DTO_FIELD_REQUIRE(Float64, retailPrice, ZH_WORDS_GETTER("drug.field.retailPrice"), true);
    API_DTO_FIELD_DEFAULT(Float64, maximumRetailPrice, ZH_WORDS_GETTER("drug.field.maximumRetailPrice"));
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("drug.field.id"), true);
    DTO_FIELD(String, drugDefId, "medicationDefId") = "20001";
    DTO_FIELD_INFO(drugDefId)
    {
        info->description = ZH_WORDS_GETTER("drug.field.drugDefId");
        info->required = false;
    }
    DTO_FIELD(String, purchaseUnitCodeDictText,
              "purchaseUnitCode_dictText") = ZH_WORDS_GETTER("drug.example.purchaseUnitText");
    DTO_FIELD_INFO(purchaseUnitCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.purchaseUnitCodeText");
        info->required = false;
    }
    DTO_FIELD(String, doseFromDictText, "doseFrom_dictText") = ZH_WORDS_GETTER("drug.example.doseFormText");
    DTO_FIELD_INFO(doseFromDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.doseFromText");
        info->required = false;
    }
    DTO_FIELD(String, antibioticCodeDictText,
              "antibioticCode_dictText") = ZH_WORDS_GETTER("drug.example.antibioticText");
    DTO_FIELD_INFO(antibioticCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.antibioticCodeText");
        info->required = false;
    }
    DTO_FIELD(String, typeCodeDictText, "typeCode_dictText") = ZH_WORDS_GETTER("drug.example.typeText");
    DTO_FIELD_INFO(typeCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.typeCodeText");
        info->required = false;
    }
    DTO_FIELD(String, doseUnitCodeDictText, "doseUnitCode_dictText") = ZH_WORDS_GETTER("drug.example.doseUnitText");
    DTO_FIELD_INFO(doseUnitCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.doseUnitCodeText");
        info->required = false;
    }
    API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("drug.field.statusEnum"), false, "ACTIVE");
    DTO_FIELD(String, statusEnumText, "statusEnum_enumText") = ZH_WORDS_GETTER("drug.example.statusEnabled");
    DTO_FIELD_INFO(statusEnumText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.statusEnumText");
        info->required = false;
    }
    API_DTO_FIELD(Boolean, activeFlag, ZH_WORDS_GETTER("drug.field.activeFlag"), false, true);
    API_DTO_FIELD(String, specification, ZH_WORDS_GETTER("drug.field.specification"), true, "5ml:0.5g");
    API_DTO_FIELD(String, activeFlagText, ZH_WORDS_GETTER("drug.field.activeFlagText"), true,
                  ZH_WORDS_GETTER("drug.example.statusEnabled"));
    DTO_FIELD(String, categoryCodeDictText, "categoryCode_dictText") = ZH_WORDS_GETTER("drug.example.typeText");
    DTO_FIELD_INFO(categoryCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.categoryCodeText");
        info->required = false;
    }
    DTO_FIELD(String, packageUnitCodeDictText,
              "packageUnitCode_dictText") = ZH_WORDS_GETTER("drug.example.purchaseUnitText");
    DTO_FIELD_INFO(packageUnitCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.packageUnitCodeText");
        info->required = false;
    }
    DTO_FIELD(String, locationIdDictText, "locationId_dictText") = ZH_WORDS_GETTER("drug.example.locationText");
    DTO_FIELD_INFO(locationIdDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.locationIdText");
        info->required = false;
    }
    DTO_FIELD(String, supplyIdDictText, "supplyId_dictText") = ZH_WORDS_GETTER("drug.example.supplierText");
    DTO_FIELD_INFO(supplyIdDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.supplyIdText");
        info->required = false;
    }
    API_DTO_FIELD(Boolean, potencyFlag, ZH_WORDS_GETTER("drug.field.potencyFlag"), false, false);
    API_DTO_FIELD(List<DrugUnitBandDTO::Wrapper>, unitBands, ZH_WORDS_GETTER("drug.field.unitBands"), false,
                  List<DrugUnitBandDTO::Wrapper>::createShared());

  public:
    DrugModifyResultDTO()
    {
        id = "10001";
        name = ZH_WORDS_GETTER("drug.example.name");
        categoryCode = "FL001";
        merchandiseName = ZH_WORDS_GETTER("drug.example.merchandiseName");
        purchaseUnitCode = "DW001";
        splitUnitCode = "DW002";
        pharmacologyCategoryCode = "YL001";
        manufacturerText = ZH_WORDS_GETTER("drug.example.manufacturerText");
        supplyId = "60001";
        partAttributeEnum = "1";
        thoPartAttributeEnum = "1";
        typeCode = "CW001";
        totalVolume = "5ml:0.5g";
        doseFormCode = "JX001";
        doseUnitCode = "DW001";
        packageUnitCode = "DW003";
        stockUnitCode = "DW004";
        doseUnitRatio = 10.0;
        purchaseUnitRatio = 10.0;
        stockUnitRatio = 10.0;
        packageUnitRatio = 10.0;
        purchasePrice = 10.5;
        retailPrice = 15.8;
        maximumRetailPrice = 18.0;
        specification = totalVolume;
        unitBands->push_back(DrugUnitBandDTO::createShared());
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DRUG_MODIFY_RESULT_DTO_H_
