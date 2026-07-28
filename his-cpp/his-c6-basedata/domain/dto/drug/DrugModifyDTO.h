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
#ifndef _DRUG_MODIFY_DTO_H_
#define _DRUG_MODIFY_DTO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/PayloadDTO.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <initializer_list>
#include <utility>
#include <vector>

#include OATPP_CODEGEN_BEGIN(DTO)

/** 修改药品请求。 */
class DrugModifyDTO : public oatpp::DTO
{
    DTO_INIT(DrugModifyDTO, DTO);

    // 修改主键。
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("drug.field.id"), true);

    // 基础信息。
    API_DTO_FIELD_REQUIRE(String, orgId, ZH_WORDS_GETTER("drug.field.orgId"), false);
    API_DTO_FIELD_REQUIRE(String, medicationDefId, ZH_WORDS_GETTER("drug.field.drugDefId"), false);
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

    // 单位及医保信息。
    API_DTO_FIELD_REQUIRE(String, purchaseUnitCode, ZH_WORDS_GETTER("drug.field.purchaseUnitCode"), true);
    DTO_FIELD(String, purchaseUnitCodeDictText, "purchaseUnitCode_dictText");
    DTO_FIELD_INFO(purchaseUnitCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.purchaseUnitCodeText");
        info->required = false;
    }
    API_DTO_FIELD_REQUIRE(String, splitUnitCode, ZH_WORDS_GETTER("drug.field.splitUnitCode"), true);
    API_DTO_FIELD_REQUIRE(String, comprisedText, ZH_WORDS_GETTER("drug.field.comprisedText"), false);
    API_DTO_FIELD_REQUIRE(String, ingredient, ZH_WORDS_GETTER("drug.field.ingredient"), false);
    API_DTO_FIELD_REQUIRE(Int32, doseFrom, ZH_WORDS_GETTER("drug.field.doseFrom"), false);
    DTO_FIELD(String, doseFromDictText, "doseFrom_dictText");
    DTO_FIELD_INFO(doseFromDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.doseFromText");
        info->required = false;
    }
    API_DTO_FIELD_REQUIRE(String, approvalNumber, ZH_WORDS_GETTER("drug.field.approvalNumber"), false);
    API_DTO_FIELD_REQUIRE(Boolean, ybMatchFlag, ZH_WORDS_GETTER("drug.field.ybMatchFlag"), false);
    API_DTO_FIELD_REQUIRE(String, ybNo, ZH_WORDS_GETTER("drug.field.ybNo"), false);
    API_DTO_FIELD_REQUIRE(String, pharmacologyCategoryCode, ZH_WORDS_GETTER("drug.field.pharmacologyCategoryCode"),
                          true);

    // 供应及业务标志。
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
    DTO_FIELD(String, antibioticCodeDictText, "antibioticCode_dictText");
    DTO_FIELD_INFO(antibioticCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.antibioticCodeText");
        info->required = false;
    }
    API_DTO_FIELD_REQUIRE(String, restrictedEnum, ZH_WORDS_GETTER("drug.field.restrictedEnum"), false);
    API_DTO_FIELD_REQUIRE(Boolean, selfFlag, ZH_WORDS_GETTER("drug.field.selfFlag"), false);
    API_DTO_FIELD_REQUIRE(Boolean, antibioticFlag, ZH_WORDS_GETTER("drug.field.antibioticFlag"), false);
    API_DTO_FIELD_REQUIRE(Boolean, basicFlag, ZH_WORDS_GETTER("drug.field.basicFlag"), false);
    API_DTO_FIELD_REQUIRE(String, thoPartAttributeEnum, ZH_WORDS_GETTER("drug.field.thoPartAttributeEnum"), true);

    // 计费及用药信息。
    API_DTO_FIELD_REQUIRE(Int32, chrgitmLv, ZH_WORDS_GETTER("drug.field.chrgitmLv"), false);
    API_DTO_FIELD_REQUIRE(String, typeCode, ZH_WORDS_GETTER("drug.field.typeCode"), true);
    DTO_FIELD(String, typeCodeDictText, "typeCode_dictText");
    DTO_FIELD_INFO(typeCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.typeCodeText");
        info->required = false;
    }
    API_DTO_FIELD_REQUIRE(String, ybType, ZH_WORDS_GETTER("drug.field.ybType"), false);
    API_DTO_FIELD_REQUIRE(Boolean, rxFlag, ZH_WORDS_GETTER("drug.field.rxFlag"), false);
    API_DTO_FIELD_REQUIRE(String, dosageInstruction, ZH_WORDS_GETTER("drug.field.dosageInstruction"), false);
    API_DTO_FIELD_REQUIRE(String, drug69Code, ZH_WORDS_GETTER("drug.field.drug69Code"), false);
    API_DTO_FIELD_REQUIRE(String, totalVolume, ZH_WORDS_GETTER("drug.field.totalVolume"), true);
    API_DTO_FIELD_REQUIRE(String, doseFormCode, ZH_WORDS_GETTER("drug.field.doseFormCode"), true);
    API_DTO_FIELD_REQUIRE(String, doseUnitCode, ZH_WORDS_GETTER("drug.field.doseUnitCode"), true);
    DTO_FIELD(String, doseUnitCodeDictText, "doseUnitCode_dictText");
    DTO_FIELD_INFO(doseUnitCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.doseUnitCodeText");
        info->required = false;
    }
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

    // 换算比及价格。
    API_DTO_FIELD_REQUIRE(Float64, doseUnitRatio, ZH_WORDS_GETTER("drug.field.doseUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, purchaseUnitRatio, ZH_WORDS_GETTER("drug.field.purchaseUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, stockUnitRatio, ZH_WORDS_GETTER("drug.field.stockUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(Float64, packageUnitRatio, ZH_WORDS_GETTER("drug.field.packageUnitRatio"), true);
    API_DTO_FIELD_REQUIRE(String, statusEnum, ZH_WORDS_GETTER("drug.field.statusEnum"), false);
    DTO_FIELD(String, statusEnumText, "statusEnum_enumText");
    DTO_FIELD_INFO(statusEnumText)
    {
        info->description = ZH_WORDS_GETTER("drug.field.statusEnumText");
        info->required = false;
    }
    API_DTO_FIELD_REQUIRE(Boolean, activeFlag, ZH_WORDS_GETTER("drug.field.activeFlag"), false);
    API_DTO_FIELD_REQUIRE(Float64, purchasePrice, ZH_WORDS_GETTER("drug.field.purchasePrice"), true);
    API_DTO_FIELD_REQUIRE(Float64, retailPrice, ZH_WORDS_GETTER("drug.field.retailPrice"), true);
    API_DTO_FIELD_DEFAULT(Float64, maximumRetailPrice, ZH_WORDS_GETTER("drug.field.maximumRetailPrice"));

    // 控制器注入登录信息。
    CC_SYNTHESIZE(const PayloadDTO *, _payload, Payload);

  public:
    /** 构造 Swagger 请求示例。 */
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->id = "10001";
        dto->orgId = "30001";
        dto->medicationDefId = "20001";
        dto->busNo = "YP20250409001";
        dto->name = ZH_WORDS_GETTER("drug.example.name");
        dto->domainEnum = "3";
        dto->medVersion = "V1.0";
        dto->nameEn = "Amoxicillin Capsules";
        dto->pyStr = "AMXLJN";
        dto->wbStr = "SGRM";
        dto->categoryCode = "FL001";
        dto->merchandiseName = ZH_WORDS_GETTER("drug.example.merchandiseName");
        dto->merchandisePyStr = "AMX";
        dto->merchandiseWbStr = "SGR";
        dto->purchaseUnitCode = "DW001";
        dto->purchaseUnitCodeDictText = ZH_WORDS_GETTER("drug.example.purchaseUnitText");
        dto->splitUnitCode = "DW002";
        dto->comprisedText = ZH_WORDS_GETTER("drug.example.comprisedText");
        dto->ingredient = ZH_WORDS_GETTER("drug.example.ingredient");
        dto->doseFrom = 1;
        dto->doseFromDictText = ZH_WORDS_GETTER("drug.example.doseFormText");
        dto->approvalNumber = ZH_WORDS_GETTER("drug.example.approvalNumber");
        dto->ybMatchFlag = true;
        dto->ybNo = "YB2025001";
        dto->pharmacologyCategoryCode = "YL001";
        dto->skinTestFlag = false;
        dto->injectFlag = false;
        dto->manufacturerId = "50001";
        dto->manufacturerText = ZH_WORDS_GETTER("drug.example.manufacturerText");
        dto->supplyId = "60001";
        dto->restrictedFlag = false;
        dto->restrictedScope = ZH_WORDS_GETTER("drug.example.restrictedScope");
        dto->childrenFlag = true;
        dto->characteristic = 1;
        dto->itemMinQuantity = 10.0;
        dto->itemMaxQuantity = 100.0;
        dto->nationalDrugCode = "860000123456789";
        dto->partAttributeEnum = "1";
        dto->antibioticCode = "KS001";
        dto->antibioticCodeDictText = ZH_WORDS_GETTER("drug.example.antibioticText");
        dto->restrictedEnum = "1";
        dto->selfFlag = false;
        dto->antibioticFlag = true;
        dto->basicFlag = true;
        dto->thoPartAttributeEnum = "1";
        dto->chrgitmLv = 2;
        dto->typeCode = "CW001";
        dto->typeCodeDictText = ZH_WORDS_GETTER("drug.example.typeText");
        dto->ybType = "YB001";
        dto->rxFlag = true;
        dto->dosageInstruction = ZH_WORDS_GETTER("drug.example.dosageInstruction");
        dto->drug69Code = "6901234567890";
        dto->totalVolume = "5ml:0.5g";
        dto->doseFormCode = "JX001";
        dto->doseUnitCode = "DW001";
        dto->doseUnitCodeDictText = ZH_WORDS_GETTER("drug.example.doseUnitText");
        dto->packageUnitCode = "DW003";
        dto->stockUnitCode = "DW004";
        dto->methodCode = "YF001";
        dto->rateCode = "PC001";
        dto->dose = 0.5;
        dto->maxUnit = 1.0;
        dto->usageLimit = 2.0;
        dto->locationId = "40001";
        dto->ingredientItem = ZH_WORDS_GETTER("drug.example.ingredientItem");
        dto->definition = ZH_WORDS_GETTER("drug.example.definition");
        dto->dddCode = "DDD001";
        dto->dddUnitCode = "DW001";
        dto->minRateCode = "PC001";
        dto->maxRateCode = "PC002";
        dto->doseUnitRatio = 10.0;
        dto->purchaseUnitRatio = 10.0;
        dto->stockUnitRatio = 10.0;
        dto->packageUnitRatio = 10.0;
        dto->statusEnum = "1";
        dto->statusEnumText = ZH_WORDS_GETTER("drug.example.statusEnabled");
        dto->activeFlag = true;
        dto->purchasePrice = 10.5;
        dto->retailPrice = 15.8;
        dto->maximumRetailPrice = 18.0;
        return dto;
    }

    /** 校验修改参数及业务约束。 */
    std::string validate() const
    {
        // 校验主键。
        if (!isValidId(id))
        {
            return "id invalid.";
        }

        // 校验必填文本。
        if (!hasText(name) || !hasText(merchandiseName) || !hasText(categoryCode) ||
            !hasText(manufacturerText) || !hasText(supplyId) || !hasText(typeCode) ||
            !hasText(pharmacologyCategoryCode) || !hasText(totalVolume) || !hasText(purchaseUnitCode) ||
            !hasText(splitUnitCode) || !hasText(doseUnitCode) || !hasText(doseFormCode) ||
            !hasText(packageUnitCode) || !hasText(stockUnitCode) || !hasText(partAttributeEnum) ||
            !hasText(thoPartAttributeEnum))
        {
            return "required field invalid.";
        }

        // 校验换算比和价格。
        if (doseUnitRatio.getValue(0.0) <= 0.0 || purchaseUnitRatio.getValue(0.0) <= 0.0 ||
            stockUnitRatio.getValue(0.0) <= 0.0 || packageUnitRatio.getValue(0.0) <= 0.0 ||
            !purchasePrice || !retailPrice)
        {
            return "required number invalid.";
        }

        // 校验文本长度。
        if (!hasValidLengths(*this))
        {
            return "field length invalid.";
        }

        // 校验可选外键。
        if (!isValidOptionalId(orgId) || !isValidOptionalId(medicationDefId) ||
            !isValidOptionalId(manufacturerId) ||
            !isValidOptionalId(supplyId) || !isValidOptionalId(locationId))
        {
            return "optional id invalid.";
        }

        // 校验小数范围。
        if (!hasValidDecimals(*this))
        {
            return "decimal invalid.";
        }

        // 校验字段关系。
        if ((itemMinQuantity && itemMinQuantity.getValue(-1.0) < 0.0) ||
            (itemMaxQuantity && itemMaxQuantity.getValue(-1.0) < 0.0) ||
            (itemMinQuantity && itemMaxQuantity &&
             itemMinQuantity.getValue(0.0) > itemMaxQuantity.getValue(0.0)) ||
            (maximumRetailPrice && maximumRetailPrice.getValue(0.0) < retailPrice.getValue(0.0)) ||
            !hasValidUnitRatios(*this))
        {
            return "business rule invalid.";
        }
        return "";
    }

  private:
    static constexpr size_t MAX_ID_LENGTH = 64;
    static constexpr size_t MAX_TEXT_LENGTH = 255;
    static constexpr double MAX_QUANTITY = 9999999999999999.0;
    static constexpr double MAX_RATIO = 999999999999.0;
    static constexpr double MAX_PRICE = 9999999999999.0;

    // 判断文本非空。
    static bool hasText(const oatpp::String &value)
    {
        if (!value)
        {
            return false;
        }
        const std::string text = value.getValue("");
        return std::any_of(text.cbegin(), text.cend(), [](unsigned char character)
                           { return std::isspace(character) == 0; });
    }

    // 校验标识格式。
    static bool isValidId(const oatpp::String &value)
    {
        if (!hasText(value))
        {
            return false;
        }
        const std::string text = value.getValue("");
        return text.size() <= MAX_ID_LENGTH &&
               std::all_of(text.cbegin(), text.cend(), [](unsigned char character)
                           { return std::isalnum(character) != 0 || character == '-' || character == '_'; });
    }

    // 校验可选标识。
    static bool isValidOptionalId(const oatpp::String &value)
    {
        return value == nullptr || isValidId(value);
    }

    // 校验 UTF-8 字符数。
    static bool hasValidUtf8Length(const oatpp::String &value, size_t maximumLength)
    {
        if (!value)
        {
            return true;
        }

        const std::string text = value.getValue("");
        size_t length = 0;
        const auto *data = reinterpret_cast<const unsigned char *>(text.data());
        const size_t byteCount = text.size();
        for (size_t index = 0; index < byteCount;)
        {
            const unsigned char lead = data[index];
            size_t sequenceLength = 0;
            if (lead <= 0x7F)
            {
                sequenceLength = 1;
            }
            else if ((lead & 0xE0) == 0xC0)
            {
                sequenceLength = 2;
            }
            else if ((lead & 0xF0) == 0xE0)
            {
                sequenceLength = 3;
            }
            else if ((lead & 0xF8) == 0xF0)
            {
                sequenceLength = 4;
            }
            else
            {
                return false;
            }

            if (index + sequenceLength > byteCount)
            {
                return false;
            }
            for (size_t offset = 1; offset < sequenceLength; ++offset)
            {
                if ((data[index + offset] & 0xC0) != 0x80)
                {
                    return false;
                }
            }
            index += sequenceLength;
            if (++length > maximumLength)
            {
                return false;
            }
        }
        return true;
    }

    // 批量校验字符数。
    static bool allHaveValidLength(size_t maximumLength, std::initializer_list<oatpp::String> values)
    {
        return std::all_of(values.begin(), values.end(), [maximumLength](const oatpp::String &value)
                           { return hasValidUtf8Length(value, maximumLength); });
    }

    // 按表字段限制校验长度。
    static bool hasValidLengths(const DrugModifyDTO &dto)
    {
        if (!allHaveValidLength(
                MAX_ID_LENGTH,
                {dto.id, dto.orgId, dto.medicationDefId, dto.manufacturerId, dto.supplyId, dto.locationId}))
        {
            return false;
        }
        return allHaveValidLength(
            MAX_TEXT_LENGTH,
            {dto.busNo, dto.name, dto.domainEnum, dto.medVersion, dto.nameEn, dto.pyStr, dto.wbStr,
             dto.categoryCode, dto.merchandiseName, dto.merchandisePyStr, dto.merchandiseWbStr,
              dto.purchaseUnitCode, dto.splitUnitCode, dto.comprisedText, dto.ingredient, dto.approvalNumber,
              dto.purchaseUnitCodeDictText, dto.doseFromDictText, dto.ybNo, dto.pharmacologyCategoryCode,
              dto.manufacturerText, dto.restrictedScope,
              dto.nationalDrugCode, dto.partAttributeEnum, dto.antibioticCode, dto.restrictedEnum,
              dto.antibioticCodeDictText, dto.thoPartAttributeEnum, dto.typeCode, dto.typeCodeDictText,
              dto.ybType, dto.dosageInstruction, dto.drug69Code, dto.totalVolume, dto.doseFormCode,
              dto.doseUnitCode, dto.doseUnitCodeDictText, dto.packageUnitCode, dto.stockUnitCode,
              dto.methodCode, dto.rateCode, dto.ingredientItem, dto.definition, dto.dddCode, dto.dddUnitCode,
              dto.minRateCode, dto.maxRateCode, dto.statusEnum, dto.statusEnumText});
    }

    // 校验可选小数。
    static bool isValidOptionalDecimal(const oatpp::Float64 &value, double maximum)
    {
        if (!value)
        {
            return true;
        }
        const double number = value.getValue(0.0);
        return std::isfinite(number) && number >= 0.0 && number <= maximum;
    }

    // 校验数量、换算比和价格。
    static bool hasValidDecimals(const DrugModifyDTO &dto)
    {
        return isValidOptionalDecimal(dto.itemMinQuantity, MAX_QUANTITY) &&
               isValidOptionalDecimal(dto.itemMaxQuantity, MAX_QUANTITY) &&
               isValidOptionalDecimal(dto.dose, MAX_QUANTITY) &&
               isValidOptionalDecimal(dto.maxUnit, MAX_QUANTITY) &&
               isValidOptionalDecimal(dto.usageLimit, MAX_QUANTITY) &&
               isValidOptionalDecimal(dto.doseUnitRatio, MAX_RATIO) &&
               isValidOptionalDecimal(dto.purchaseUnitRatio, MAX_RATIO) &&
               isValidOptionalDecimal(dto.stockUnitRatio, MAX_RATIO) &&
               isValidOptionalDecimal(dto.packageUnitRatio, MAX_RATIO) &&
               isValidOptionalDecimal(dto.purchasePrice, MAX_PRICE) &&
               isValidOptionalDecimal(dto.retailPrice, MAX_PRICE) &&
               isValidOptionalDecimal(dto.maximumRetailPrice, MAX_PRICE);
    }

    // 相同单位必须使用相同换算比。
    static bool hasValidUnitRatios(const DrugModifyDTO &dto)
    {
        const std::string splitUnit = dto.splitUnitCode.getValue("");
        const std::vector<std::pair<std::string, double>> unitRatios = {
            {dto.doseUnitCode.getValue(""), dto.doseUnitRatio.getValue(0.0)},
            {dto.purchaseUnitCode.getValue(""), dto.purchaseUnitRatio.getValue(0.0)},
            {dto.stockUnitCode.getValue(""), dto.stockUnitRatio.getValue(0.0)},
            {dto.packageUnitCode.getValue(""), dto.packageUnitRatio.getValue(0.0)}};

        for (size_t left = 0; left < unitRatios.size(); ++left)
        {
            if (unitRatios[left].first == splitUnit &&
                std::abs(unitRatios[left].second - 1.0) > 0.000001)
            {
                return false;
            }
            for (size_t right = left + 1; right < unitRatios.size(); ++right)
            {
                if (unitRatios[left].first == unitRatios[right].first &&
                    std::abs(unitRatios[left].second - unitRatios[right].second) > 0.000001)
                {
                    return false;
                }
            }
        }
        return true;
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DRUG_MODIFY_DTO_H_
