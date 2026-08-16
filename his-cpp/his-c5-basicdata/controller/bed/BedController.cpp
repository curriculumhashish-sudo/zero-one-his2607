/*
 Copyright Zero One Star. All rights reserved.

 @Author: Codex
 @Date: 2026/07/28

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
#include "BedController.h"
#include "StringUtil.h"
#include "service/ward/WardService.h"
#include "picojson/picojson.h"

namespace
{
    bool hasBedText(const oatpp::String& value)
    {
        if (!value)
        {
            return false;
        }

        std::string text = value->c_str();
        StringUtil::trim(&text);
        return !text.empty();
    }

    BedVO::Wrapper toBedVO(
        const PtrWardLocationDO& source)
    {
        if (!source)
        {
            return nullptr;
        }

        auto data = BedVO::createShared();
        data->id = source->getId();
        data->name = source->getName();
        data->busNo = source->getBusNo();
        data->statusEnum = source->getStatusEnum();
        data->operationalEnum = source->getOperationalEnum();
        data->formEnum = source->getFormEnum();
        data->modeEnum = source->getModeEnum();
        data->typeJson = source->getTypeJson();
        data->organizationId = source->getOrganizationId();
        data->displayOrder = source->getDisplayOrder();
        data->pyStr = source->getPyStr();
        data->wbStr = source->getWbStr();
        data->parentId = source->getParentId();
        data->version = source->getVersion();
        return data;
    }

    bool isBedTypeJsonValid(const oatpp::String& value)
    {
        const std::string text =
            value ? value->c_str() : "{}";
        picojson::value parsed;
        const std::string error =
            picojson::parse(parsed, text);
        return error.empty() &&
            parsed.is<picojson::object>();
    }
}

BedJsonVO::Wrapper BedController::execModifyBed(
    const BedModifyDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto response = BedJsonVO::createShared();
    if (!dto ||
        !hasBedText(dto->id) ||
        !hasBedText(dto->name) ||
        !hasBedText(dto->parentId) ||
        !hasBedText(dto->organizationId) ||
        dto->id.getValue("").size() > 64 ||
        dto->name.getValue("").size() > 255 ||
        dto->parentId.getValue("").size() > 64 ||
        dto->organizationId.getValue("").size() > 64 ||
        dto->typeJson.getValue("{}").size() > 255 ||
        !isBedTypeJsonValid(dto->typeJson) ||
        dto->version.getValue(-1) < 0 ||
        dto->displayOrder.getValue(0) < 0)
    {
        response->init(
            nullptr,
            ResultStatus("Invalid bed modification parameters.", 9995));
        return response;
    }

    const auto result = WardService().modifyBed(dto, payload);
    if (!result.success)
    {
        response->init(
            nullptr,
            ResultStatus(result.message, 9999));
        return response;
    }

    response->success(toBedVO(result.data));
    return response;
}

BooleanJsonVO::Wrapper BedController::execRemoveBed(
    const String& id,
    const PayloadDTO& payload)
{
    auto response = BooleanJsonVO::createShared();
    if (!hasBedText(id) || id.getValue("").size() > 64)
    {
        response->init(
            false,
            ResultStatus("Bed ID is required.", 9995));
        return response;
    }

    const auto result =
        WardService().removeBed(id.getValue(""), payload);
    if (!result.success)
    {
        response->init(
            false,
            ResultStatus(result.message, 9999));
        return response;
    }

    response->success(true);
    return response;
}

BedJsonVO::Wrapper BedController::execModifyBedStatus(
    const BedStatusDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto response = BedJsonVO::createShared();
    const std::string statusEnum =
        dto ? dto->statusEnum.getValue("") : "";

    if (!dto ||
        !hasBedText(dto->id) ||
        dto->id.getValue("").size() > 64 ||
        (statusEnum != "ACTIVE" &&
         statusEnum != "DISABLED") ||
        dto->version.getValue(-1) < 0)
    {
        response->init(
            nullptr,
            ResultStatus("Invalid bed status parameters.", 9995));
        return response;
    }

    const auto result =
        WardService().changeBedStatus(dto, payload);
    if (!result.success)
    {
        response->init(
            nullptr,
            ResultStatus(result.message, 9999));
        return response;
    }

    response->success(toBedVO(result.data));
    return response;
}
