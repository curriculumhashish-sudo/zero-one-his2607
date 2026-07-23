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

#include <algorithm>
#include <cctype>
#include <exception>

namespace
{
bool hasText(const oatpp::String &value)
{
    return value && std::any_of(value->cbegin(), value->cend(), [](const char character)
                                { return std::isspace(static_cast<unsigned char>(character)) == 0; });
}

bool isUnsignedIntegerId(const oatpp::String &value)
{
    return hasText(value) && value->find_first_not_of("0123456789") == std::string::npos;
}

bool hasRequiredModifyFields(const DrugModifyDTO::Wrapper &dto)
{
    return dto && isUnsignedIntegerId(dto->id) && hasText(dto->name) && hasText(dto->merchandiseName) &&
           hasText(dto->categoryCode) && hasText(dto->manufacturerText) && hasText(dto->supplyId) &&
           hasText(dto->typeCode) && hasText(dto->pharmacologyCategoryCode) && hasText(dto->totalVolume) &&
           hasText(dto->purchaseUnitCode) && hasText(dto->splitUnitCode) && hasText(dto->doseUnitCode) &&
           hasText(dto->doseFormCode) && hasText(dto->packageUnitCode) && hasText(dto->stockUnitCode) &&
           hasText(dto->partAttributeEnum) && hasText(dto->thoPartAttributeEnum) &&
           dto->doseUnitRatio.getValue(0.0) > 0.0 && dto->purchaseUnitRatio.getValue(0.0) > 0.0 &&
           dto->purchasePrice != nullptr && dto->stockUnitRatio.getValue(0.0) > 0.0 && dto->retailPrice != nullptr &&
           dto->packageUnitRatio.getValue(0.0) > 0.0;
}

NoDataJsonVO::Wrapper createBatchResult(const oatpp::List<oatpp::String> &ids)
{
    auto result = NoDataJsonVO::createShared();
    if (!ids || ids->empty())
    {
        result->init(RS_PARAMS_INVALID);
        return result;
    }

    for (const auto &id : *ids.get())
    {
        if (!isUnsignedIntegerId(id))
        {
            result->init(RS_PARAMS_INVALID);
            return result;
        }
    }

    result->init(RS_SUCCESS);
    return result;
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
    ids->push_back("10001");
    ids->push_back("10002");
    return ids;
}

DrugModifyDTO::Wrapper DrugSwagger::sampleModify()
{
    auto dto = DrugModifyDTO::createShared();
    fillRequiredModifyFields(dto);
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

void DrugSwagger::fillRequiredModifyFields(const DrugModifyDTO::Wrapper &dto)
{
    dto->id = "10001";
    dto->name = ZH_WORDS_GETTER("drug.example.name");
    dto->categoryCode = "FL001";
    dto->merchandiseName = ZH_WORDS_GETTER("drug.example.merchandiseName");
    dto->purchaseUnitCode = "DW001";
    dto->splitUnitCode = "DW002";
    dto->pharmacologyCategoryCode = "YL001";
    dto->manufacturerText = ZH_WORDS_GETTER("drug.example.manufacturerText");
    dto->supplyId = "60001";
    dto->partAttributeEnum = "1";
    dto->thoPartAttributeEnum = "1";
    dto->typeCode = "CW001";
    dto->totalVolume = "5ml:0.5g";
    dto->doseFormCode = "JX001";
    dto->doseUnitCode = "DW001";
    dto->packageUnitCode = "DW003";
    dto->stockUnitCode = "DW004";
    dto->doseUnitRatio = 10.0;
    dto->purchaseUnitRatio = 10.0;
    dto->stockUnitRatio = 10.0;
    dto->packageUnitRatio = 10.0;
    dto->purchasePrice = 10.5;
    dto->retailPrice = 15.8;
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
    return {};
}

DrugDetailJsonVO::Wrapper DrugController::execQueryOneDrug(const String &id)
{
    return {};
}

DrugModifyJsonVO::Wrapper DrugController::execModifyDrug(const DrugModifyDTO::Wrapper &dto)
{
    auto result = DrugModifyJsonVO::createShared();
    if (!hasRequiredModifyFields(dto))
    {
        result->init(nullptr, RS_PARAMS_INVALID);
        return result;
    }

    try
    {
        const auto mapper = getDefaultObjectMapper();
        if (!mapper)
        {
            result->init(nullptr, RS_PARAMS_INVALID);
            return result;
        }

        auto detail = mapper->readFromString<DrugModifyResultDTO::Wrapper>(mapper->writeToString(dto));
        if (!detail)
        {
            result->init(nullptr, RS_PARAMS_INVALID);
            return result;
        }

        detail->specification = dto->totalVolume;
        if (!hasText(detail->drugDefId))
        {
            detail->drugDefId = dto->id;
        }
        result->success(detail);
    }
    catch (const std::exception &)
    {
        result->init(nullptr, RS_PARAMS_INVALID);
    }

    return result;
}

NoDataJsonVO::Wrapper DrugController::execRemoveDrug(const DrugRemoveDTO::Wrapper &dto)
{
    return createBatchResult(dto ? dto->ids : nullptr);
}

NoDataJsonVO::Wrapper DrugController::execEnableDisableDrug(const DrugEnableDisableDTO::Wrapper &dto)
{
    if (!dto || dto->activeFlag == nullptr)
    {
        auto result = NoDataJsonVO::createShared();
        result->init(RS_PARAMS_INVALID);
        return result;
    }
    return createBatchResult(dto->ids);
}
