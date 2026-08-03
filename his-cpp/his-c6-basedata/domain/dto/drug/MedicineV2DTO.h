#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/08/03

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
#ifndef _MEDICINE_V2_DTO_H_
#define _MEDICINE_V2_DTO_H_

#include "../../DrugConstants.h"
#include "../../GlobalInclude.h"
#include "domain/dto/PayloadDTO.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <set>

/** 药品写接口业务码。 */
class MedicineV2BusinessCode final
{
  public:
    static constexpr int SUCCESS = 10000;
    static constexpr int PARAM_INVALID = 10001;
    static constexpr int UNAUTHORIZED = 10002;
    static constexpr int FORBIDDEN = 10003;
    static constexpr int NOT_FOUND = 10004;
    static constexpr int PARTIAL_SUCCESS = 10008;
    static constexpr int VERSION_CONFLICT = 10009;
    static constexpr int BUSINESS_VALIDATION_FAILED = 10010;
    static constexpr int SERVER_ERROR = 10099;
};

#include OATPP_CODEGEN_BEGIN(DTO)

/** 药品编号和乐观锁版本。 */
class MedicineVersionItemDTO : public oatpp::DTO
{
    DTO_INIT(MedicineVersionItemDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("drug.field.id"), true);
    API_DTO_FIELD_REQUIRE(Int32, version, ZH_WORDS_GETTER("drug.field.version"), true);

  public:
    static Wrapper createExample(const char *value = "10001", int currentVersion = 3)
    {
        auto dto = createShared();
        dto->id = value;
        dto->version = currentVersion;
        return dto;
    }

    bool isValid() const
    {
        if (!id || !version || version.getValue(-1) < 0)
        {
            return false;
        }
        const std::string value = id.getValue("");
        return !value.empty() && value.size() <= 64 &&
               std::all_of(value.cbegin(), value.cend(), [](unsigned char character)
                           { return std::isalnum(character) != 0 || character == '-' || character == '_'; });
    }
};

/** PATCH 药品请求，仅修改显式传入的字段。 */
class MedicineUpdateDTO : public oatpp::DTO
{
    DTO_INIT(MedicineUpdateDTO, DTO);

    API_DTO_FIELD_REQUIRE(Int32, version, ZH_WORDS_GETTER("drug.field.version"), true);
    API_DTO_FIELD_DEFAULT(String, busNo, ZH_WORDS_GETTER("drug.field.busNo"));
    API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("drug.field.name"));
    API_DTO_FIELD_DEFAULT(String, domainEnum, ZH_WORDS_GETTER("drug.field.domainEnum"));
    API_DTO_FIELD_DEFAULT(String, medVersion, ZH_WORDS_GETTER("drug.field.medVersion"));
    API_DTO_FIELD_DEFAULT(String, nameEn, ZH_WORDS_GETTER("drug.field.nameEn"));
    API_DTO_FIELD_DEFAULT(String, categoryCode, ZH_WORDS_GETTER("drug.field.categoryCode"));
    API_DTO_FIELD_DEFAULT(String, merchandiseName, ZH_WORDS_GETTER("drug.field.merchandiseName"));
    API_DTO_FIELD_DEFAULT(String, purchaseUnitCode, ZH_WORDS_GETTER("drug.field.purchaseUnitCode"));
    API_DTO_FIELD_DEFAULT(String, splitUnitCode, ZH_WORDS_GETTER("drug.field.splitUnitCode"));
    API_DTO_FIELD_DEFAULT(String, comprisedText, ZH_WORDS_GETTER("drug.field.comprisedText"));
    API_DTO_FIELD_DEFAULT(String, ingredient, ZH_WORDS_GETTER("drug.field.ingredient"));
    API_DTO_FIELD_DEFAULT(Int32, doseFrom, ZH_WORDS_GETTER("drug.field.doseFrom"));
    API_DTO_FIELD_DEFAULT(Boolean, ybMatchFlag, ZH_WORDS_GETTER("drug.field.ybMatchFlag"));
    API_DTO_FIELD_DEFAULT(String, pharmacologyCategoryCode,
                          ZH_WORDS_GETTER("drug.field.pharmacologyCategoryCode"));
    API_DTO_FIELD_DEFAULT(Boolean, skinTestFlag, ZH_WORDS_GETTER("drug.field.skinTestFlag"));
    API_DTO_FIELD_DEFAULT(Boolean, injectFlag, ZH_WORDS_GETTER("drug.field.injectFlag"));
    API_DTO_FIELD_DEFAULT(String, manufacturerId, ZH_WORDS_GETTER("drug.field.manufacturerId"));
    API_DTO_FIELD_DEFAULT(String, manufacturerText, ZH_WORDS_GETTER("drug.field.manufacturerText"));
    API_DTO_FIELD_DEFAULT(String, supplyId, ZH_WORDS_GETTER("drug.field.supplyId"));
    API_DTO_FIELD_DEFAULT(Boolean, restrictedFlag, ZH_WORDS_GETTER("drug.field.restrictedFlag"));
    API_DTO_FIELD_DEFAULT(String, restrictedScope, ZH_WORDS_GETTER("drug.field.restrictedScope"));
    API_DTO_FIELD_DEFAULT(Boolean, childrenFlag, ZH_WORDS_GETTER("drug.field.childrenFlag"));
    API_DTO_FIELD_DEFAULT(Int32, characteristic, ZH_WORDS_GETTER("drug.field.characteristic"));
    API_DTO_FIELD_DEFAULT(Float64, itemMinQuantity, ZH_WORDS_GETTER("drug.field.itemMinQuantity"));
    API_DTO_FIELD_DEFAULT(Float64, itemMaxQuantity, ZH_WORDS_GETTER("drug.field.itemMaxQuantity"));
    API_DTO_FIELD_DEFAULT(String, approvalNumber, ZH_WORDS_GETTER("drug.field.approvalNumber"));
    API_DTO_FIELD_DEFAULT(String, nationalDrugCode, ZH_WORDS_GETTER("drug.field.nationalDrugCode"));
    API_DTO_FIELD_DEFAULT(String, partAttributeEnum, ZH_WORDS_GETTER("drug.field.partAttributeEnum"));
    API_DTO_FIELD_DEFAULT(String, antibioticCode, ZH_WORDS_GETTER("drug.field.antibioticCode"));
    API_DTO_FIELD_DEFAULT(String, restrictedEnum, ZH_WORDS_GETTER("drug.field.restrictedEnum"));
    API_DTO_FIELD_DEFAULT(Boolean, selfFlag, ZH_WORDS_GETTER("drug.field.selfFlag"));
    API_DTO_FIELD_DEFAULT(Boolean, antibioticFlag, ZH_WORDS_GETTER("drug.field.antibioticFlag"));
    API_DTO_FIELD_DEFAULT(Boolean, basicFlag, ZH_WORDS_GETTER("drug.field.basicFlag"));
    API_DTO_FIELD_DEFAULT(String, thoPartAttributeEnum, ZH_WORDS_GETTER("drug.field.thoPartAttributeEnum"));
    API_DTO_FIELD_DEFAULT(String, ybNo, ZH_WORDS_GETTER("drug.field.ybNo"));
    API_DTO_FIELD_DEFAULT(Int32, chrgitmLv, ZH_WORDS_GETTER("drug.field.chrgitmLv"));
    API_DTO_FIELD_DEFAULT(String, ybType, ZH_WORDS_GETTER("drug.field.ybType"));
    API_DTO_FIELD_DEFAULT(Boolean, rxFlag, ZH_WORDS_GETTER("drug.field.rxFlag"));
    API_DTO_FIELD_DEFAULT(String, dosageInstruction, ZH_WORDS_GETTER("drug.field.dosageInstruction"));
    API_DTO_FIELD_DEFAULT(String, drug69Code, ZH_WORDS_GETTER("drug.field.drug69Code"));
    API_DTO_FIELD_DEFAULT(String, specification, ZH_WORDS_GETTER("drug.field.specification"));
    API_DTO_FIELD_DEFAULT(String, doseUnitCode, ZH_WORDS_GETTER("drug.field.doseUnitCode"));
    API_DTO_FIELD_DEFAULT(String, doseFormCode, ZH_WORDS_GETTER("drug.field.doseFormCode"));
    API_DTO_FIELD_DEFAULT(String, packageUnitCode, ZH_WORDS_GETTER("drug.field.packageUnitCode"));
    API_DTO_FIELD_DEFAULT(String, stockUnitCode, ZH_WORDS_GETTER("drug.field.stockUnitCode"));
    API_DTO_FIELD_DEFAULT(String, methodCode, ZH_WORDS_GETTER("drug.field.methodCode"));
    API_DTO_FIELD_DEFAULT(String, rateCode, ZH_WORDS_GETTER("drug.field.rateCode"));
    API_DTO_FIELD_DEFAULT(Float64, dose, ZH_WORDS_GETTER("drug.field.dose"));
    API_DTO_FIELD_DEFAULT(Float64, maxUnit, ZH_WORDS_GETTER("drug.field.maxUnit"));
    API_DTO_FIELD_DEFAULT(Float64, usageLimit, ZH_WORDS_GETTER("drug.field.usageLimit"));
    API_DTO_FIELD_DEFAULT(String, locationId, ZH_WORDS_GETTER("drug.field.locationId"));
    API_DTO_FIELD_DEFAULT(String, ingredientItem, ZH_WORDS_GETTER("drug.field.ingredientItem"));
    API_DTO_FIELD_DEFAULT(String, definition, ZH_WORDS_GETTER("drug.field.definition"));
    API_DTO_FIELD_DEFAULT(String, dddCode, ZH_WORDS_GETTER("drug.field.dddCode"));
    API_DTO_FIELD_DEFAULT(String, dddUnitCode, ZH_WORDS_GETTER("drug.field.dddUnitCode"));
    API_DTO_FIELD_DEFAULT(String, minRateCode, ZH_WORDS_GETTER("drug.field.minRateCode"));
    API_DTO_FIELD_DEFAULT(String, maxRateCode, ZH_WORDS_GETTER("drug.field.maxRateCode"));
    API_DTO_FIELD_DEFAULT(Float64, doseUnitRatio, ZH_WORDS_GETTER("drug.field.doseUnitRatio"));
    API_DTO_FIELD_DEFAULT(Float64, purchaseUnitRatio, ZH_WORDS_GETTER("drug.field.purchaseUnitRatio"));
    API_DTO_FIELD_DEFAULT(Float64, stockUnitRatio, ZH_WORDS_GETTER("drug.field.stockUnitRatio"));
    API_DTO_FIELD_DEFAULT(Float64, packageUnitRatio, ZH_WORDS_GETTER("drug.field.packageUnitRatio"));
    API_DTO_FIELD_DEFAULT(String, pyStr, ZH_WORDS_GETTER("drug.field.pyStr"));
    API_DTO_FIELD_DEFAULT(String, wbStr, ZH_WORDS_GETTER("drug.field.wbStr"));
    API_DTO_FIELD_DEFAULT(String, merchandisePyStr, ZH_WORDS_GETTER("drug.field.merchandisePyStr"));
    API_DTO_FIELD_DEFAULT(String, merchandiseWbStr, ZH_WORDS_GETTER("drug.field.merchandiseWbStr"));
    API_DTO_FIELD_DEFAULT(String, financeTypeCode, ZH_WORDS_GETTER("drug.field.typeCode"));
    API_DTO_FIELD_DEFAULT(Float64, purchasePrice, ZH_WORDS_GETTER("drug.field.purchasePrice"));
    API_DTO_FIELD_DEFAULT(Float64, retailPrice, ZH_WORDS_GETTER("drug.field.retailPrice"));
    API_DTO_FIELD_DEFAULT(Float64, maximumRetailPrice, ZH_WORDS_GETTER("drug.field.maximumRetailPrice"));

    CC_SYNTHESIZE(const PayloadDTO *, _payload, Payload);

  public:
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->version = 3;
        dto->name = ZH_WORDS_GETTER("drug.example.name");
        dto->manufacturerText = ZH_WORDS_GETTER("drug.example.manufacturerText");
        dto->purchasePrice = 15.50;
        dto->retailPrice = 20.00;
        return dto;
    }

    std::string validate() const
    {
        if (!version || version.getValue(-1) < 0 || !hasAnyChange())
        {
            return "version or patch field invalid.";
        }
        if ((name && !hasText(name)) || (manufacturerText && !hasText(manufacturerText)) ||
            (categoryCode && !hasText(categoryCode)) || (merchandiseName && !hasText(merchandiseName)) ||
            (purchaseUnitCode && !hasText(purchaseUnitCode)) || (splitUnitCode && !hasText(splitUnitCode)) ||
            (pharmacologyCategoryCode && !hasText(pharmacologyCategoryCode)) ||
            (supplyId && !hasText(supplyId)) || (partAttributeEnum && !hasText(partAttributeEnum)) ||
            (thoPartAttributeEnum && !hasText(thoPartAttributeEnum)) ||
            (specification && !hasText(specification)) || (doseUnitCode && !hasText(doseUnitCode)) ||
            (doseFormCode && !hasText(doseFormCode)) || (packageUnitCode && !hasText(packageUnitCode)) ||
            (stockUnitCode && !hasText(stockUnitCode)) ||
            (financeTypeCode && !hasText(financeTypeCode)))
        {
            return "required patch text invalid.";
        }
        if (!isValidOptionalId(manufacturerId) || !isValidOptionalId(supplyId) ||
            !isValidOptionalId(locationId) || !allTextLengthsValid() ||
            !isNonNegative(itemMinQuantity, 9999999999999999.0) ||
            !isNonNegative(itemMaxQuantity, 9999999999999999.0) ||
            !isNonNegative(dose, 9999999999999999.0) || !isNonNegative(maxUnit, 9999999999999999.0) ||
            !isNonNegative(usageLimit, 9999999999999999.0) || !isPositiveRatio(doseUnitRatio) ||
            !isPositiveRatio(purchaseUnitRatio) || !isPositiveRatio(stockUnitRatio) ||
            !isPositiveRatio(packageUnitRatio) || !isPriceValid(purchasePrice) ||
            !isPriceValid(retailPrice) || !isPriceValid(maximumRetailPrice) ||
            (itemMinQuantity && itemMaxQuantity &&
             itemMinQuantity.getValue(0.0) > itemMaxQuantity.getValue(0.0)) ||
            (retailPrice && maximumRetailPrice &&
             retailPrice.getValue(0.0) > maximumRetailPrice.getValue(0.0)))
        {
            return "patch field range invalid.";
        }
        return "";
    }

  private:
    bool hasAnyChange() const
    {
        return busNo || name || domainEnum || medVersion || nameEn || pyStr || wbStr || categoryCode ||
               merchandiseName || merchandisePyStr || merchandiseWbStr || purchaseUnitCode || splitUnitCode ||
               comprisedText || ingredient || doseFrom || approvalNumber || ybMatchFlag || ybNo ||
               pharmacologyCategoryCode || skinTestFlag || injectFlag || manufacturerId || manufacturerText ||
               supplyId || restrictedFlag || restrictedScope || childrenFlag || characteristic || itemMinQuantity ||
               itemMaxQuantity || nationalDrugCode || partAttributeEnum || antibioticCode || restrictedEnum ||
               selfFlag || antibioticFlag || basicFlag || thoPartAttributeEnum || chrgitmLv || financeTypeCode ||
               ybType || rxFlag || dosageInstruction || drug69Code || specification || doseFormCode || doseUnitCode ||
               packageUnitCode || stockUnitCode || methodCode || rateCode || dose || maxUnit || usageLimit ||
               locationId || ingredientItem || definition || dddCode || dddUnitCode || minRateCode || maxRateCode ||
               doseUnitRatio || purchaseUnitRatio || stockUnitRatio || packageUnitRatio || purchasePrice ||
               retailPrice || maximumRetailPrice;
    }

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

    static bool isValidOptionalId(const oatpp::String &value)
    {
        if (!value)
        {
            return true;
        }
        const std::string text = value.getValue("");
        return !text.empty() && text.size() <= 64 &&
               std::all_of(text.cbegin(), text.cend(), [](unsigned char character)
                           { return std::isalnum(character) != 0 || character == '-' || character == '_'; });
    }

    static bool hasValidUtf8Length(const oatpp::String &value, size_t maximumLength)
    {
        if (!value)
        {
            return true;
        }
        const std::string text = value.getValue("");
        const auto *bytes = reinterpret_cast<const unsigned char *>(text.data());
        size_t characterCount = 0;
        for (size_t index = 0; index < text.size();)
        {
            const unsigned char lead = bytes[index];
            const size_t sequenceLength = lead <= 0x7F ? 1 : (lead & 0xE0) == 0xC0 ? 2
                                                        : (lead & 0xF0) == 0xE0   ? 3
                                                        : (lead & 0xF8) == 0xF0   ? 4
                                                                                 : 0;
            if (sequenceLength == 0 || index + sequenceLength > text.size())
            {
                return false;
            }
            for (size_t offset = 1; offset < sequenceLength; ++offset)
            {
                if ((bytes[index + offset] & 0xC0) != 0x80)
                {
                    return false;
                }
            }
            index += sequenceLength;
            if (++characterCount > maximumLength)
            {
                return false;
            }
        }
        return true;
    }

    static bool isPriceValid(const oatpp::Float64 &value)
    {
        if (!value)
        {
            return true;
        }
        const double number = value.getValue(-1.0);
        return std::isfinite(number) && number >= 0.0 && number <= 9999999999999.0;
    }

    static bool isNonNegative(const oatpp::Float64 &value, double maximum)
    {
        if (!value)
        {
            return true;
        }
        const double number = value.getValue(-1.0);
        return std::isfinite(number) && number >= 0.0 && number <= maximum;
    }

    static bool isPositiveRatio(const oatpp::Float64 &value)
    {
        if (!value)
        {
            return true;
        }
        const double number = value.getValue(0.0);
        return std::isfinite(number) && number > 0.0 && number <= 999999999999.0;
    }

    bool allTextLengthsValid() const
    {
        const oatpp::String values[] = {busNo,
                                        name,
                                        domainEnum,
                                        medVersion,
                                        nameEn,
                                        categoryCode,
                                        merchandiseName,
                                        purchaseUnitCode,
                                        splitUnitCode,
                                        comprisedText,
                                        ingredient,
                                        manufacturerText,
                                        manufacturerId,
                                        supplyId,
                                        approvalNumber,
                                        nationalDrugCode,
                                        ybNo,
                                        pharmacologyCategoryCode,
                                        restrictedScope,
                                        partAttributeEnum,
                                        antibioticCode,
                                        restrictedEnum,
                                        thoPartAttributeEnum,
                                        ybType,
                                        dosageInstruction,
                                        drug69Code,
                                        specification,
                                        doseUnitCode,
                                        doseFormCode,
                                        packageUnitCode,
                                        stockUnitCode,
                                        methodCode,
                                        rateCode,
                                        locationId,
                                        ingredientItem,
                                        definition,
                                        dddCode,
                                        dddUnitCode,
                                        minRateCode,
                                        maxRateCode,
                                        pyStr,
                                        wbStr,
                                        merchandisePyStr,
                                        merchandiseWbStr,
                                        financeTypeCode};
        return std::all_of(std::begin(values), std::end(values), [](const oatpp::String &value)
                           { return hasValidUtf8Length(value, 255); });
    }
};

/** 批量启停请求。 */
class MedicineStatusUpdateDTO : public oatpp::DTO
{
    DTO_INIT(MedicineStatusUpdateDTO, DTO);

    API_DTO_FIELD_REQUIRE(List<MedicineVersionItemDTO::Wrapper>, items, ZH_WORDS_GETTER("drug.field.items"), true);
    API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("drug.field.statusEnum"), true);

    CC_SYNTHESIZE(const PayloadDTO *, _payload, Payload);

  public:
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->items = oatpp::List<MedicineVersionItemDTO::Wrapper>::createShared();
        dto->items->push_back(MedicineVersionItemDTO::createExample("10001", 3));
        dto->items->push_back(MedicineVersionItemDTO::createExample("10002", 1));
        dto->status = "INACTIVE";
        return dto;
    }

    std::string validate() const
    {
        if (!status || (status != "ACTIVE" && status != "INACTIVE"))
        {
            return "status invalid.";
        }
        return validateItems(items);
    }

    static std::string validateItems(const oatpp::List<MedicineVersionItemDTO::Wrapper> &source)
    {
        if (!source || source->empty() || source->size() > DrugConstants::MAX_BATCH_SIZE)
        {
            return "items invalid.";
        }
        std::set<std::string> ids;
        for (const auto &item : *source.get())
        {
            if (!item || !item->isValid() || !ids.emplace(item->id.getValue("")).second)
            {
                return "item invalid.";
            }
        }
        return "";
    }
};

/** 批量删除请求。 */
class MedicineBatchDeleteDTO : public oatpp::DTO
{
    DTO_INIT(MedicineBatchDeleteDTO, DTO);

    API_DTO_FIELD_REQUIRE(List<MedicineVersionItemDTO::Wrapper>, items, ZH_WORDS_GETTER("drug.field.items"), true);

    CC_SYNTHESIZE(const PayloadDTO *, _payload, Payload);

  public:
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->items = oatpp::List<MedicineVersionItemDTO::Wrapper>::createShared();
        dto->items->push_back(MedicineVersionItemDTO::createExample("10001", 3));
        dto->items->push_back(MedicineVersionItemDTO::createExample("10002", 1));
        return dto;
    }

    std::string validate() const
    {
        return MedicineStatusUpdateDTO::validateItems(items);
    }
};

/** 批量操作失败项。 */
class MedicineFailedItemDTO : public oatpp::DTO
{
    DTO_INIT(MedicineFailedItemDTO, DTO);

    DTO_FIELD(String, id, "id");
    DTO_FIELD(Int32, code, "code");
    DTO_FIELD(String, message, "message");
    DTO_FIELD(Int32, currentVersion, "currentVersion");
};

/** 批量操作结果。 */
class MedicineBatchResultDTO : public oatpp::DTO
{
    DTO_INIT(MedicineBatchResultDTO, DTO);

    DTO_FIELD(List<MedicineVersionItemDTO::Wrapper>, successItems, "successItems");
    DTO_FIELD(List<MedicineFailedItemDTO::Wrapper>, failedItems, "failedItems");

  public:
    static Wrapper createEmpty()
    {
        auto dto = createShared();
        dto->successItems = oatpp::List<MedicineVersionItemDTO::Wrapper>::createShared();
        dto->failedItems = oatpp::List<MedicineFailedItemDTO::Wrapper>::createShared();
        return dto;
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICINE_V2_DTO_H_
