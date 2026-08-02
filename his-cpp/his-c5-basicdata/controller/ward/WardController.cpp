#include "stdafx.h"
#include "WardController.h"
#include "StringUtil.h"
#include "service/ward/WardService.h"

namespace
{
    bool hasText(const oatpp::String& value)
    {
        if (!value)
        {
            return false;
        }

        std::string text = value->c_str();
        StringUtil::trim(&text);
        return !text.empty();
    }

    WardVO::Wrapper toWardVO(
        const PtrWardLocationDO& source)
    {
        if (!source)
        {
            return nullptr;
        }

        auto data = WardVO::createShared();
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

// ============================================================
// 根据病房ID查询病房信息
// 调用链路: Controller → WardService::queryRoomById → WardDAO::selectLocationById
// 参数校验: id非空、长度≤64，无效返回RS_PARAMS_INVALID(9995)
// 未找到对应记录或formEnum不是HOUSE，返回RS_FAIL(9999)
// 成功返回: WardRoomDTO { id, name, wardAreaId }
// ============================================================
WardRoomJsonVO::Wrapper WardController::execQueryRoomById(const String& id) {
    auto response = WardRoomJsonVO::createShared();
    if (!hasText(id) || id.getValue("").size() > 64)
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }
    auto data = WardService().queryRoomById(id.getValue(""));
    if (!data)
    {
        response->fail(nullptr);
        return response;
    }
    response->success(data);
    return response;
}

// ============================================================
// 根据病床ID查询病床信息
// 调用链路: Controller → WardService::queryBedById → WardDAO::selectLocationById
// 参数校验: id非空、长度≤64，无效返回RS_PARAMS_INVALID(9995)
// 未找到对应记录或formEnum不是BED，返回RS_FAIL(9999)
// 成功返回: WardBedDTO { id, name, status(0空闲/1占用/2停用), wardRoomId }
// ============================================================
WardBedJsonVO::Wrapper WardController::execQueryBedById(const String& id) {
    auto response = WardBedJsonVO::createShared();
    if (!hasText(id) || id.getValue("").size() > 64)
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }
    auto data = WardService().queryBedById(id.getValue(""));
    if (!data)
    {
        response->fail(nullptr);
        return response;
    }
    response->success(data);
    return response;
}

WardJsonVO::Wrapper WardController::execModifyWard(
    const WardModifyDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto response = WardJsonVO::createShared();
    if (!dto ||
        !hasText(dto->id) ||
        !hasText(dto->name) ||
        !hasText(dto->organizationId) ||
        dto->id.getValue("").size() > 64 ||
        dto->name.getValue("").size() > 255 ||
        dto->organizationId.getValue("").size() > 64 ||
        dto->pyStr.getValue("").size() > 255 ||
        dto->wbStr.getValue("").size() > 255 ||
        dto->version.getValue(-1) < 0 ||
        dto->displayOrder.getValue(0) < 0)
    {
        response->init(
            nullptr,
            ResultStatus("Invalid ward modification parameters.", 9995));
        return response;
    }

    const auto result = WardService().modifyWard(dto, payload);
    if (!result.success)
    {
        response->init(
            nullptr,
            ResultStatus(result.message, 9999));
        return response;
    }

    response->success(toWardVO(result.data));
    return response;
}

BooleanJsonVO::Wrapper WardController::execRemoveWard(
    const String& id,
    const PayloadDTO& payload)
{
    auto response = BooleanJsonVO::createShared();
    if (!hasText(id) || id.getValue("").size() > 64)
    {
        response->init(
            false,
            ResultStatus("Ward ID is required.", 9995));
        return response;
    }

    const auto result =
        WardService().removeWard(id.getValue(""), payload);
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

WardJsonVO::Wrapper WardController::execModifyWardStatus(
    const WardStatusDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto response = WardJsonVO::createShared();
    const std::string statusEnum =
        dto ? dto->statusEnum.getValue("") : "";

    if (!dto ||
        !hasText(dto->id) ||
        dto->id.getValue("").size() > 64 ||
        (statusEnum != "ACTIVE" &&
         statusEnum != "DISABLED") ||
        dto->version.getValue(-1) < 0)
    {
        response->init(
            nullptr,
            ResultStatus("Invalid ward status parameters.", 9995));
        return response;
    }

    const auto result =
        WardService().changeWardStatus(dto, payload);
    if (!result.success)
    {
        response->init(
            nullptr,
            ResultStatus(result.message, 9999));
        return response;
    }

    response->success(toWardVO(result.data));
    return response;
}

// Query ward tree list
ListJsonVO<WardTreeNodeVO::Wrapper>::Wrapper
WardController::execWardTree(
    const WardTreeQuery::Wrapper& query
) {
    if (!query || !query->organizationId) {
        return nullptr;
    }
    auto response =
        ListJsonVO<WardTreeNodeVO::Wrapper>::createShared();
    std::string organizationId = query->organizationId.getValue("");
    StringUtil::trim(&organizationId);

    if (organizationId.empty()) {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }
    auto data = WardService().GetTree(query);

    if (!data) {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    response->success(data);
    return response;
}

// Add ward location
StringJsonVO::Wrapper WardController::execAddLocation(
    const WardLocationAddDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto response = StringJsonVO::createShared();

    if (!dto) {
        response->init(
            "Request body is required.",
            RS_PARAMS_INVALID
        );
        return response;
    }

    const std::string id =
        WardService().createLocation(dto, payload);

    if (id.empty()) {
        response->fail("Failed to create location.");
        return response;
    }

    response->success(id);
    return response;
}
