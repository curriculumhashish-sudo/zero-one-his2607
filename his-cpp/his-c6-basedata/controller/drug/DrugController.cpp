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
#include "stdafx.h"
#include "DrugController.h"
#include "../../service/sample/DrugService.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <exception>
#include <initializer_list>
#include <set>
#include <utility>
#include <vector>

namespace
{
constexpr size_t MAX_BATCH_SIZE = 100;

bool hasText(const oatpp::String &value)
{
    if (!value)
    {
        return false;
    }
    const std::string text = value.getValue("");
    return std::any_of(text.cbegin(), text.cend(), [](const char character)
                       { return std::isspace(static_cast<unsigned char>(character)) == 0; });
}

bool isValidId(const oatpp::String &value)
{
    if (!hasText(value))
    {
        return false;
    }
    const std::string text = value.getValue("");
    return text.size() <= 64 &&
           std::all_of(text.cbegin(), text.cend(), [](unsigned char character)
                       { return std::isalnum(character) != 0 || character == '-' || character == '_'; });
}

bool isValidOptionalId(const oatpp::String &value)
{
    return value == nullptr || isValidId(value);
}

bool hasValidUtf8Length(const oatpp::String &value, size_t maximumLength)
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

bool allHaveValidLength(size_t maximumLength, std::initializer_list<oatpp::String> values)
{
    return std::all_of(values.begin(), values.end(), [maximumLength](const oatpp::String &value)
                       { return hasValidUtf8Length(value, maximumLength); });
}

bool isValidOptionalDecimal(const oatpp::Float64 &value, double maximum)
{
    if (!value)
    {
        return true;
    }
    const double number = value.getValue(0.0);
    return std::isfinite(number) && number >= 0.0 && number <= maximum;
}

bool hasValidModifyLengths(const DrugModifyDTO::Wrapper &dto)
{
    if (!allHaveValidLength(64, {dto->id, dto->orgId, dto->manufacturerId, dto->supplyId, dto->locationId}) ||
        !allHaveValidLength(200, {dto->manufacturerText}))
    {
        return false;
    }
    return allHaveValidLength(
        255,
        {dto->busNo, dto->name, dto->domainEnum, dto->version, dto->nameEn, dto->pyStr, dto->wbStr,
         dto->categoryCode, dto->merchandiseName, dto->merchandisePyStr, dto->merchandiseWbStr,
         dto->purchaseUnitCode, dto->splitUnitCode, dto->comprisedText, dto->ingredient, dto->approvalNumber,
         dto->ybNo, dto->pharmacologyCategoryCode, dto->manufacturerText, dto->restrictedScope,
         dto->nationalDrugCode, dto->partAttributeEnum, dto->antibioticCode, dto->restrictedEnum,
         dto->thoPartAttributeEnum, dto->typeCode, dto->ybType, dto->dosageInstruction, dto->drug69Code,
         dto->totalVolume, dto->doseFormCode, dto->doseUnitCode, dto->packageUnitCode, dto->stockUnitCode,
         dto->methodCode, dto->rateCode, dto->ingredientItem, dto->definition, dto->dddCode, dto->dddUnitCode,
         dto->minRateCode, dto->maxRateCode});
}

bool hasValidUnitRatios(const DrugModifyDTO::Wrapper &dto)
{
    const std::string splitUnit = dto->splitUnitCode.getValue("");
    const std::vector<std::pair<std::string, double>> unitRatios = {
        {dto->doseUnitCode.getValue(""), dto->doseUnitRatio.getValue(0.0)},
        {dto->purchaseUnitCode.getValue(""), dto->purchaseUnitRatio.getValue(0.0)},
        {dto->stockUnitCode.getValue(""), dto->stockUnitRatio.getValue(0.0)},
        {dto->packageUnitCode.getValue(""), dto->packageUnitRatio.getValue(0.0)}};

    for (size_t left = 0; left < unitRatios.size(); ++left)
    {
        if (unitRatios[left].first == splitUnit && std::abs(unitRatios[left].second - 1.0) > 0.000001)
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

bool hasRequiredModifyFields(const DrugModifyDTO::Wrapper &dto)
{
    const char *stage = "identity";
    try
    {
        if (!dto || !isValidId(dto->id))
        {
            return false;
        }
        stage = "required-text";
        if (!hasText(dto->name) || !hasText(dto->merchandiseName) || !hasText(dto->categoryCode) ||
            !hasText(dto->manufacturerText) || !hasText(dto->supplyId) || !hasText(dto->typeCode) ||
            !hasText(dto->pharmacologyCategoryCode) || !hasText(dto->totalVolume) ||
            !hasText(dto->purchaseUnitCode) || !hasText(dto->splitUnitCode) || !hasText(dto->doseUnitCode) ||
            !hasText(dto->doseFormCode) || !hasText(dto->packageUnitCode) || !hasText(dto->stockUnitCode) ||
            !hasText(dto->partAttributeEnum) || !hasText(dto->thoPartAttributeEnum))
        {
            return false;
        }
        stage = "required-number";
        if (dto->doseUnitRatio.getValue(0.0) <= 0.0 || dto->purchaseUnitRatio.getValue(0.0) <= 0.0 ||
            dto->stockUnitRatio.getValue(0.0) <= 0.0 || dto->packageUnitRatio.getValue(0.0) <= 0.0 ||
            !dto->purchasePrice || !dto->retailPrice)
        {
            return false;
        }
        stage = "length";
        if (!hasValidModifyLengths(dto))
        {
            return false;
        }
        stage = "optional-id";
        if (!isValidOptionalId(dto->orgId) || !isValidOptionalId(dto->manufacturerId) ||
            !isValidOptionalId(dto->supplyId) || !isValidOptionalId(dto->locationId))
        {
            return false;
        }
        stage = "decimal";
        if (!isValidOptionalDecimal(dto->itemMinQuantity, 9999999999999999.0) ||
            !isValidOptionalDecimal(dto->itemMaxQuantity, 9999999999999999.0) ||
            !isValidOptionalDecimal(dto->dose, 9999999999999999.0) ||
            !isValidOptionalDecimal(dto->maxUnit, 9999999999999999.0) ||
            !isValidOptionalDecimal(dto->usageLimit, 9999999999999999.0) ||
            !isValidOptionalDecimal(dto->doseUnitRatio, 999999999999.0) ||
            !isValidOptionalDecimal(dto->purchaseUnitRatio, 999999999999.0) ||
            !isValidOptionalDecimal(dto->stockUnitRatio, 999999999999.0) ||
            !isValidOptionalDecimal(dto->packageUnitRatio, 999999999999.0) ||
            !isValidOptionalDecimal(dto->purchasePrice, 9999999999999.0) ||
            !isValidOptionalDecimal(dto->retailPrice, 9999999999999.0) ||
            !isValidOptionalDecimal(dto->maximumRetailPrice, 9999999999999.0))
        {
            return false;
        }
        stage = "business-rule";
        if ((dto->itemMinQuantity && dto->itemMinQuantity.getValue(-1.0) < 0.0) ||
            (dto->itemMaxQuantity && dto->itemMaxQuantity.getValue(-1.0) < 0.0) ||
            (dto->itemMinQuantity && dto->itemMaxQuantity &&
             dto->itemMinQuantity.getValue(0.0) > dto->itemMaxQuantity.getValue(0.0)) ||
            (dto->maximumRetailPrice &&
             dto->maximumRetailPrice.getValue(0.0) < dto->retailPrice.getValue(0.0)) ||
            !hasValidUnitRatios(dto))
        {
            return false;
        }
        return true;
    }
    catch (const std::exception &error)
    {
        std::cerr << "DrugController validation failed at " << stage << ": " << error.what() << std::endl;
        throw;
    }
}

bool hasValidBatchIds(const oatpp::List<oatpp::String> &ids)
{
    if (!ids || ids->empty() || ids->size() > MAX_BATCH_SIZE)
    {
        return false;
    }
    std::set<std::string> unique;
    for (const auto &id : *ids.get())
    {
        if (!isValidId(id))
        {
            return false;
        }
        unique.emplace(id.getValue(""));
    }
    return !unique.empty();
}

ResultStatus serviceErrorStatus(DrugServiceError error)
{
    switch (error)
    {
    case DrugServiceError::InvalidInput:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.invalidInput"), 9995);
    case DrugServiceError::NotFound:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.notFound"), 9999);
    case DrugServiceError::InvalidStatus:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.invalidStatus"), 9999);
    case DrugServiceError::Referenced:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.referenced"), 9999);
    case DrugServiceError::CrossTenant:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.crossTenant"), 9999);
    case DrugServiceError::ConcurrentModification:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.concurrentModification"), 9999);
    case DrugServiceError::DatabaseFailure:
    default:
        return ResultStatus(ZH_WORDS_GETTER("drug.error.databaseFailure"), 9994);
    }
}

ResultStatus unexpectedErrorStatus()
{
    return ResultStatus(ZH_WORDS_GETTER("drug.error.databaseFailure"), 9994);
}
} // namespace

void DrugSwagger::addModifyExamples(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info> &info)
{
    info->body.description = ZH_WORDS_GETTER("drug.body.modify");
    info->body.addExample("default", sampleModify());
    info->responses[oatpp::web::protocol::http::Status::CODE_200].addExample("success", successModify());
}

void DrugSwagger::addRemoveExamples(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info> &info)
{
    info->body.description = ZH_WORDS_GETTER("drug.body.remove");
    info->body.addExample("default", sampleRemove());
    info->responses[oatpp::web::protocol::http::Status::CODE_200].addExample("success", successNoData());
}

void DrugSwagger::addEnableDisableExamples(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info> &info)
{
    info->body.description = ZH_WORDS_GETTER("drug.body.enableDisable");
    info->body.addExample("enable", sampleEnableDisable(true));
    info->body.addExample("disable", sampleEnableDisable(false));
    info->responses[oatpp::web::protocol::http::Status::CODE_200].addExample("success", successNoData());
}

oatpp::List<oatpp::String> DrugSwagger::sampleIds()
{
    auto ids = oatpp::List<oatpp::String>::createShared();
    ids->push_back("322994171216400384");
    ids->push_back("322994478289784832");
    return ids;
}

DrugModifyDTO::Wrapper DrugSwagger::sampleModify()
{
    auto dto = DrugModifyDTO::createShared();
    fillModifyExample(dto);
    return dto;
}

DrugRemoveDTO::Wrapper DrugSwagger::sampleRemove()
{
    auto dto = DrugRemoveDTO::createShared();
    dto->ids = sampleIds();
    return dto;
}

DrugEnableDisableDTO::Wrapper DrugSwagger::sampleEnableDisable(bool activeFlag)
{
    auto dto = DrugEnableDisableDTO::createShared();
    dto->ids = sampleIds();
    dto->activeFlag = activeFlag;
    return dto;
}

DrugModifyJsonVO::Wrapper DrugSwagger::successModify()
{
    auto result = DrugModifyJsonVO::createShared();
    result->success(DrugModifyResultDTO::createShared());
    return result;
}

NoDataJsonVO::Wrapper DrugSwagger::successNoData()
{
    auto result = NoDataJsonVO::createShared();
    result->init(RS_SUCCESS);
    return result;
}

void DrugSwagger::fillModifyExample(const DrugModifyDTO::Wrapper &dto)
{
    dto->id = "322994171216400384";
    dto->orgId = "30001";
    dto->busNo = "YP20250409001";
    dto->name = ZH_WORDS_GETTER("drug.example.name");
    dto->domainEnum = "3";
    dto->version = "V1.0";
    dto->nameEn = "Amoxicillin Capsules";
    dto->pyStr = "AMXLJN";
    dto->wbStr = "SGRM";
    dto->categoryCode = "FL001";
    dto->merchandiseName = ZH_WORDS_GETTER("drug.example.merchandiseName");
    dto->merchandisePyStr = "AMX";
    dto->merchandiseWbStr = "SGR";
    dto->purchaseUnitCode = "DW001";
    dto->splitUnitCode = "DW002";
    dto->comprisedText = ZH_WORDS_GETTER("drug.example.comprisedText");
    dto->ingredient = ZH_WORDS_GETTER("drug.example.ingredient");
    dto->doseFrom = 1;
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
    dto->restrictedEnum = "1";
    dto->selfFlag = false;
    dto->antibioticFlag = true;
    dto->basicFlag = true;
    dto->thoPartAttributeEnum = "1";
    dto->chrgitmLv = 2;
    dto->typeCode = "CW001";
    dto->ybType = "YB001";
    dto->rxFlag = true;
    dto->dosageInstruction = ZH_WORDS_GETTER("drug.example.dosageInstruction");
    dto->drug69Code = "6901234567890";
    dto->totalVolume = "5ml:0.5g";
    dto->doseFormCode = "JX001";
    dto->doseUnitCode = "DW001";
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
    dto->purchasePrice = 10.5;
    dto->retailPrice = 15.8;
    dto->maximumRetailPrice = 18.0;
}

DrugListPageJsonVO::Wrapper DrugController::execQueryDrug(const DrugQuery::Wrapper &query)
{
    return {};
}

DrugSelectPageJsonVO::Wrapper DrugController::execQuerySelectDrug(const DrugSelectQuery::Wrapper &query)
{
    return {};
}

StringJsonVO::Wrapper DrugController::execAddDrug(const DrugAddDTO::Wrapper &dto, const PayloadDTO &payload)
{
    auto jvo = StringJsonVO::createShared();

    std::string errmsg = dto->validate();
    if (errmsg != "")
    {
        jvo->init(errmsg, RS_PARAMS_INVALID);
        return jvo;
    }
    dto->setPayload(&payload);
    std::string id = DrugService().saveData(dto);
    if (id != "") {
        jvo->success(id);
    }
    else
    {
        jvo->fail("");
    }
    return jvo;
    return {};
}

DrugDetailJsonVO::Wrapper DrugController::execQueryById(const String &id)
{
    // 定义返回数据对象
    auto jvo = DrugDetailJsonVO::createShared();
    // 参数校验
    // 非空校验
    if (!id)
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    // 执行数据新增
    auto res = DrugService().getById(id.getValue({}));
    jvo->success(res);

    //响应结果
    return jvo;

}

DrugModifyJsonVO::Wrapper DrugController::execModifyDrug(const DrugModifyDTO::Wrapper &dto, const PayloadDTO &payload)
{
    auto result = DrugModifyJsonVO::createShared();
    if (!hasRequiredModifyFields(dto))
    {
        result->init(nullptr, RS_PARAMS_INVALID);
        return result;
    }

    try
    {
        result->success(DrugService().updateDrug(dto, payload));
    }
    catch (const DrugServiceException &error)
    {
        result->init(nullptr, serviceErrorStatus(error.getError()));
    }
    catch (const std::exception &)
    {
        result->init(nullptr, unexpectedErrorStatus());
    }

    return result;
}

NoDataJsonVO::Wrapper DrugController::execRemoveDrug(const DrugRemoveDTO::Wrapper &dto, const PayloadDTO &payload)
{
    auto result = NoDataJsonVO::createShared();
    if (!dto || !hasValidBatchIds(dto->ids))
    {
        result->init(RS_PARAMS_INVALID);
        return result;
    }
    try
    {
        DrugService().removeDrugs(dto, payload);
        result->init(RS_SUCCESS);
    }
    catch (const DrugServiceException &error)
    {
        result->init(serviceErrorStatus(error.getError()));
    }
    catch (const std::exception &)
    {
        result->init(unexpectedErrorStatus());
    }
    return result;
}

NoDataJsonVO::Wrapper DrugController::execEnableDisableDrug(const DrugEnableDisableDTO::Wrapper &dto,
                                                            const PayloadDTO &payload)
{
    auto result = NoDataJsonVO::createShared();
    if (!dto || dto->activeFlag == nullptr || !hasValidBatchIds(dto->ids))
    {
        result->init(RS_PARAMS_INVALID);
        return result;
    }
    try
    {
        DrugService().enableDisableDrugs(dto, payload);
        result->init(RS_SUCCESS);
    }
    catch (const DrugServiceException &error)
    {
        result->init(serviceErrorStatus(error.getError()));
    }
    catch (const std::exception &)
    {
        result->init(unexpectedErrorStatus());
    }
    return result;
}
