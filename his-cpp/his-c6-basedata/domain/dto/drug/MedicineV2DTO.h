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
#include <cerrno>
#include <cctype>
#include <cmath>
#include <cstdlib>
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

ENUM(MedicineStatusEnum, v_int32,
     VALUE(ACTIVE, 0, "ACTIVE"),
     VALUE(INACTIVE, 1, "INACTIVE"));

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

    /** 校验药品编号格式和乐观锁版本。 */
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

/** 修改药品请求，仅修改显式传入的字段。 */
class MedicineUpdateDTO : public oatpp::DTO
{
    DTO_INIT(MedicineUpdateDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("drug.field.id"), true);
    API_DTO_FIELD_REQUIRE(Int32, version, ZH_WORDS_GETTER("drug.field.version"), true);
    API_DTO_FIELD_DEFAULT(String, busNo, ZH_WORDS_GETTER("drug.field.busNo"));
    API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("drug.field.name"));
    API_DTO_FIELD_DEFAULT(String, domainEnum, ZH_WORDS_GETTER("drug.field.domainEnum"));
    API_DTO_FIELD_DEFAULT(String, categoryCode, ZH_WORDS_GETTER("drug.field.categoryCode"));
    API_DTO_FIELD_DEFAULT(String, merchandiseName, ZH_WORDS_GETTER("drug.field.merchandiseName"));
    API_DTO_FIELD_DEFAULT(String, purchaseUnitCode, ZH_WORDS_GETTER("drug.field.purchaseUnitCode"));
    API_DTO_FIELD_DEFAULT(String, splitUnitCode, ZH_WORDS_GETTER("drug.field.splitUnitCode"));
    API_DTO_FIELD_DEFAULT(Boolean, ybMatchFlag, ZH_WORDS_GETTER("drug.field.ybMatchFlag"));
    API_DTO_FIELD_DEFAULT(String, pharmacologyCategoryCode,
                          ZH_WORDS_GETTER("drug.field.pharmacologyCategoryCode"));
    API_DTO_FIELD_DEFAULT(Boolean, skinTestFlag, ZH_WORDS_GETTER("drug.field.skinTestFlag"));
    API_DTO_FIELD_DEFAULT(Boolean, injectFlag, ZH_WORDS_GETTER("drug.field.injectFlag"));
    API_DTO_FIELD_DEFAULT(String, manufacturerText, ZH_WORDS_GETTER("drug.field.manufacturerText"));
    API_DTO_FIELD_DEFAULT(String, supplyId, ZH_WORDS_GETTER("drug.field.supplyId"));
    API_DTO_FIELD_DEFAULT(Boolean, restrictedFlag, ZH_WORDS_GETTER("drug.field.restrictedFlag"));
    API_DTO_FIELD_DEFAULT(String, restrictedScope, ZH_WORDS_GETTER("drug.field.restrictedScope"));
    API_DTO_FIELD_DEFAULT(Boolean, childrenFlag, ZH_WORDS_GETTER("drug.field.childrenFlag"));
    API_DTO_FIELD_DEFAULT(Float64, itemMinQuantity, ZH_WORDS_GETTER("drug.field.itemMinQuantity"));
    API_DTO_FIELD_DEFAULT(Float64, itemMaxQuantity, ZH_WORDS_GETTER("drug.field.itemMaxQuantity"));
    API_DTO_FIELD_DEFAULT(String, approvalNumber, ZH_WORDS_GETTER("drug.field.approvalNumber"));
    API_DTO_FIELD_DEFAULT(String, nationalDrugCode, ZH_WORDS_GETTER("drug.field.nationalDrugCode"));
    API_DTO_FIELD_DEFAULT(String, partAttributeEnum, ZH_WORDS_GETTER("drug.field.partAttributeEnum"));
    API_DTO_FIELD_DEFAULT(Boolean, selfFlag, ZH_WORDS_GETTER("drug.field.selfFlag"));
    API_DTO_FIELD_DEFAULT(Boolean, antibioticFlag, ZH_WORDS_GETTER("drug.field.antibioticFlag"));
    API_DTO_FIELD_DEFAULT(Boolean, basicFlag, ZH_WORDS_GETTER("drug.field.basicFlag"));
    API_DTO_FIELD_DEFAULT(String, thoPartAttributeEnum, ZH_WORDS_GETTER("drug.field.thoPartAttributeEnum"));
    API_DTO_FIELD_DEFAULT(String, ybNo, ZH_WORDS_GETTER("drug.field.ybNo"));
    API_DTO_FIELD_DEFAULT(String, chrgitmLv, ZH_WORDS_GETTER("drug.field.chrgitmLv"));
    API_DTO_FIELD_DEFAULT(String, ybType, ZH_WORDS_GETTER("drug.field.ybType"));
    API_DTO_FIELD_DEFAULT(Boolean, rxFlag, ZH_WORDS_GETTER("drug.field.rxFlag"));
    API_DTO_FIELD_DEFAULT(String, dosageInstruction, ZH_WORDS_GETTER("drug.field.dosageInstruction"));
    API_DTO_FIELD_DEFAULT(String, drug69Code, ZH_WORDS_GETTER("drug.field.drug69Code"));
    API_DTO_FIELD_DEFAULT(String, totalVolume, ZH_WORDS_GETTER("drug.field.totalVolume"));
    API_DTO_FIELD_DEFAULT(String, doseUnitCode, ZH_WORDS_GETTER("drug.field.doseUnitCode"));
    API_DTO_FIELD_DEFAULT(String, doseFormCode, ZH_WORDS_GETTER("drug.field.doseFormCode"));
    API_DTO_FIELD_DEFAULT(String, packageUnitCode, ZH_WORDS_GETTER("drug.field.packageUnitCode"));
    API_DTO_FIELD_DEFAULT(String, stockUnitCode, ZH_WORDS_GETTER("drug.field.stockUnitCode"));
    API_DTO_FIELD_DEFAULT(String, methodCode, ZH_WORDS_GETTER("drug.field.methodCode"));
    API_DTO_FIELD_DEFAULT(String, rateCode, ZH_WORDS_GETTER("drug.field.rateCode"));
    API_DTO_FIELD_DEFAULT(Float64, dose, ZH_WORDS_GETTER("drug.field.dose"));
    API_DTO_FIELD_DEFAULT(String, maxUnit, ZH_WORDS_GETTER("drug.field.maxUnit"));
    API_DTO_FIELD_DEFAULT(String, usageLimit, ZH_WORDS_GETTER("drug.field.usageLimit"));
    API_DTO_FIELD_DEFAULT(String, locationId, ZH_WORDS_GETTER("drug.field.locationId"));
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
    API_DTO_FIELD_DEFAULT(String, typeCode, ZH_WORDS_GETTER("drug.field.typeCode"));
    API_DTO_FIELD_DEFAULT(Float64, purchasePrice, ZH_WORDS_GETTER("drug.field.purchasePrice"));
    API_DTO_FIELD_DEFAULT(Float64, retailPrice, ZH_WORDS_GETTER("drug.field.retailPrice"));
    API_DTO_FIELD_DEFAULT(Float64, maximumRetailPrice, ZH_WORDS_GETTER("drug.field.maximumRetailPrice"));

    CC_SYNTHESIZE(const PayloadDTO *, _payload, Payload);

  public:
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->id = "322994171216400384";
        dto->version = 0;
        dto->name = ZH_WORDS_GETTER("drug.example.name");
        dto->merchandiseName = ZH_WORDS_GETTER("drug.example.merchandiseName");
        dto->categoryCode = "2";
        dto->manufacturerText = ZH_WORDS_GETTER("drug.example.manufacturerText");
        dto->supplyId = "4100000000000000004";
        dto->pharmacologyCategoryCode = "1";
        dto->totalVolume = "0.1g";
        dto->doseFormCode = "5";
        dto->doseUnitCode = "10";
        dto->purchaseUnitCode = "2";
        dto->splitUnitCode = "5";
        dto->packageUnitCode = "2";
        dto->stockUnitCode = "2";
        dto->partAttributeEnum = "2";
        dto->thoPartAttributeEnum = "2";
        dto->typeCode = "1001";
        dto->purchasePrice = 6.99;
        dto->retailPrice = 6.99;
        return dto;
    }

    /** 校验修改字段、长度、数值范围和必填组合。 */
    std::string validate() const
    {
        if (!isValidRequiredId(id) || !version || version.getValue(-1) < 0 || !hasAnyChange())
        {
            return "id, version or update field invalid.";
        }
        if ((name && !hasText(name)) || (manufacturerText && !hasText(manufacturerText)) ||
            (categoryCode && !hasText(categoryCode)) || (merchandiseName && !hasText(merchandiseName)) ||
            (purchaseUnitCode && !hasText(purchaseUnitCode)) || (splitUnitCode && !hasText(splitUnitCode)) ||
            (pharmacologyCategoryCode && !hasText(pharmacologyCategoryCode)) ||
            (supplyId && !hasText(supplyId)) || (partAttributeEnum && !hasText(partAttributeEnum)) ||
            (thoPartAttributeEnum && !hasText(thoPartAttributeEnum)) ||
            (totalVolume && !hasText(totalVolume)) || (doseUnitCode && !hasText(doseUnitCode)) ||
            (doseFormCode && !hasText(doseFormCode)) || (packageUnitCode && !hasText(packageUnitCode)) ||
            (stockUnitCode && !hasText(stockUnitCode)) ||
            (typeCode && !hasText(typeCode)))
        {
            return "required update text invalid.";
        }
        if (!isValidOptionalId(supplyId) || !isValidOptionalId(locationId) || !allTextLengthsValid() ||
            !isNonNegative(itemMinQuantity, 9999999999999999.0) ||
            !isNonNegative(itemMaxQuantity, 9999999999999999.0) ||
            !isNonNegative(dose, 9999999999999999.0) || !isNonNegativeText(maxUnit, 9999999999999999.0) ||
            !isNonNegativeText(usageLimit, 9999999999999999.0) || !isNonNegativeIntegerText(chrgitmLv) ||
            !isPositiveRatio(doseUnitRatio) ||
            !isPositiveRatio(purchaseUnitRatio) || !isPositiveRatio(stockUnitRatio) ||
            !isPositiveRatio(packageUnitRatio) || !isPriceValid(purchasePrice) ||
            !isPriceValid(retailPrice) || !isPriceValid(maximumRetailPrice) ||
            (itemMinQuantity && itemMaxQuantity &&
             itemMinQuantity.getValue(0.0) > itemMaxQuantity.getValue(0.0)) ||
            (retailPrice && maximumRetailPrice &&
             retailPrice.getValue(0.0) > maximumRetailPrice.getValue(0.0)))
        {
            return "update field range invalid.";
        }
        return "";
    }

  private:
    /** 判断请求中是否至少包含一个待修改字段。 */
    bool hasAnyChange() const
    {
        return busNo || name || domainEnum || pyStr || wbStr || categoryCode ||
               merchandiseName || merchandisePyStr || merchandiseWbStr || purchaseUnitCode || splitUnitCode ||
               approvalNumber || ybMatchFlag || ybNo || pharmacologyCategoryCode || skinTestFlag || injectFlag ||
               manufacturerText || supplyId || restrictedFlag || restrictedScope || childrenFlag || itemMinQuantity ||
               itemMaxQuantity || nationalDrugCode || partAttributeEnum ||
               selfFlag || antibioticFlag || basicFlag || thoPartAttributeEnum || chrgitmLv || typeCode ||
               ybType || rxFlag || dosageInstruction || drug69Code || totalVolume || doseFormCode || doseUnitCode ||
               packageUnitCode || stockUnitCode || methodCode || rateCode || dose || maxUnit || usageLimit ||
               locationId || definition || dddCode || dddUnitCode || minRateCode || maxRateCode ||
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

    static bool isValidRequiredId(const oatpp::String &value)
    {
        if (!value)
        {
            return false;
        }
        const std::string text = value.getValue("");
        return !text.empty() && text.size() <= 64 &&
               std::all_of(text.cbegin(), text.cend(), [](unsigned char character)
                           { return std::isalnum(character) != 0 || character == '-' || character == '_'; });
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

    /** 校验前端文本输入的非负小数。 */
    static bool isNonNegativeText(const oatpp::String &value, double maximum)
    {
        if (!value)
        {
            return true;
        }
        const std::string text = value.getValue("");
        if (text.empty())
        {
            return false;
        }
        char *end = nullptr;
        errno = 0;
        const double number = std::strtod(text.c_str(), &end);
        return errno != ERANGE && end == text.c_str() + text.size() && std::isfinite(number) &&
               number >= 0.0 && number <= maximum;
    }

    /** 校验下拉框提交的非负整数字符串。 */
    static bool isNonNegativeIntegerText(const oatpp::String &value)
    {
        if (!value)
        {
            return true;
        }
        const std::string text = value.getValue("");
        return !text.empty() && std::all_of(text.cbegin(), text.cend(), [](unsigned char character)
                                            { return std::isdigit(character) != 0; });
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
                                        categoryCode,
                                        merchandiseName,
                                        purchaseUnitCode,
                                        splitUnitCode,
                                        manufacturerText,
                                        supplyId,
                                        approvalNumber,
                                        nationalDrugCode,
                                        ybNo,
                                        pharmacologyCategoryCode,
                                        restrictedScope,
                                        partAttributeEnum,
                                        thoPartAttributeEnum,
                                        chrgitmLv,
                                        ybType,
                                        dosageInstruction,
                                        drug69Code,
                                        totalVolume,
                                        doseUnitCode,
                                        doseFormCode,
                                        packageUnitCode,
                                        stockUnitCode,
                                        methodCode,
                                        rateCode,
                                        maxUnit,
                                        usageLimit,
                                        locationId,
                                        definition,
                                        dddCode,
                                        dddUnitCode,
                                        minRateCode,
                                        maxRateCode,
                                        pyStr,
                                        wbStr,
                                        merchandisePyStr,
                                        merchandiseWbStr,
                                        typeCode};
        return std::all_of(std::begin(values), std::end(values), [](const oatpp::String &value)
                           { return hasValidUtf8Length(value, 255); });
    }
};

/** 批量启停请求。 */
class MedicineStatusUpdateDTO : public oatpp::DTO
{
    DTO_INIT(MedicineStatusUpdateDTO, DTO);

    API_DTO_FIELD_REQUIRE(List<MedicineVersionItemDTO::Wrapper>, items, ZH_WORDS_GETTER("drug.field.items"), true);
    API_DTO_FIELD_REQUIRE(Enum<MedicineStatusEnum>::AsString, status,
                          ZH_WORDS_GETTER("drug.field.targetStatus"), true);

    CC_SYNTHESIZE(const PayloadDTO *, _payload, Payload);

  public:
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->items = oatpp::List<MedicineVersionItemDTO::Wrapper>::createShared();
        dto->items->push_back(MedicineVersionItemDTO::createExample("323999010000000015", 3));
        dto->status = MedicineStatusEnum::INACTIVE;
        return dto;
    }

    /** 校验目标状态和批量药品版本列表。 */
    std::string validate() const
    {
        if (!status)
        {
            return "status invalid.";
        }
        return validateItems(items);
    }

    /** 校验批量数量、编号、版本和重复项。 */
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
        dto->items->push_back(MedicineVersionItemDTO::createExample("323999010000000015", 3));
        return dto;
    }

    /** 复用批量药品版本列表校验。 */
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
