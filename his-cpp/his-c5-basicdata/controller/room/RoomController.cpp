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
#include "RoomController.h"
#include "StringUtil.h"
#include "service/ward/WardService.h"

namespace
{
    bool hasRoomText(const oatpp::String& value)
    {
        if (!value)
        {
            return false;
        }

        std::string text = value->c_str();
        StringUtil::trim(&text);
        return !text.empty();
    }

    RoomVO::Wrapper toRoomVO(
        const PtrWardLocationDO& source)
    {
        if (!source)
        {
            return nullptr;
        }

        auto data = RoomVO::createShared();
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
}

RoomJsonVO::Wrapper RoomController::execModifyRoom(
    const RoomModifyDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto response = RoomJsonVO::createShared();
    if (!dto ||
        !hasRoomText(dto->id) ||
        !hasRoomText(dto->name) ||
        !hasRoomText(dto->parentId) ||
        !hasRoomText(dto->organizationId) ||
        dto->id.getValue("").size() > 64 ||
        dto->name.getValue("").size() > 255 ||
        dto->parentId.getValue("").size() > 64 ||
        dto->organizationId.getValue("").size() > 64 ||
        dto->pyStr.getValue("").size() > 255 ||
        dto->wbStr.getValue("").size() > 255 ||
        dto->version.getValue(-1) < 0 ||
        dto->displayOrder.getValue(0) < 0)
    {
        response->init(
            nullptr,
            ResultStatus("Invalid room modification parameters.", 9995));
        return response;
    }

    const auto result = WardService().modifyRoom(dto, payload);
    if (!result.success)
    {
        response->init(
            nullptr,
            ResultStatus(result.message, 9999));
        return response;
    }

    response->success(toRoomVO(result.data));
    return response;
}

BooleanJsonVO::Wrapper RoomController::execRemoveRoom(
    const String& id,
    const PayloadDTO& payload)
{
    auto response = BooleanJsonVO::createShared();
    if (!hasRoomText(id) || id.getValue("").size() > 64)
    {
        response->init(
            false,
            ResultStatus("Room ID is required.", 9995));
        return response;
    }

    const auto result =
        WardService().removeRoom(id.getValue(""), payload);
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

RoomJsonVO::Wrapper RoomController::execModifyRoomStatus(
    const RoomStatusDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto response = RoomJsonVO::createShared();
    const std::string statusEnum =
        dto ? dto->statusEnum.getValue("") : "";

    if (!dto ||
        !hasRoomText(dto->id) ||
        dto->id.getValue("").size() > 64 ||
        (statusEnum != "ACTIVE" &&
         statusEnum != "DISABLED") ||
        dto->version.getValue(-1) < 0)
    {
        response->init(
            nullptr,
            ResultStatus("Invalid room status parameters.", 9995));
        return response;
    }

    const auto result =
        WardService().changeRoomStatus(dto, payload);
    if (!result.success)
    {
        response->init(
            nullptr,
            ResultStatus(result.message, 9999));
        return response;
    }

    response->success(toRoomVO(result.data));
    return response;
}
