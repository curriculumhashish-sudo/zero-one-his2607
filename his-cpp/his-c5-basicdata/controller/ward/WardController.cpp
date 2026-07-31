#include "stdafx.h"
#include "WardController.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "StringUtil.h"
#include "../../service/ward/WardService.h"


WardRoomJsonVO::Wrapper WardController::execQueryRoomById(const String& id)
{
    // 定义返回数据对象
    auto jvo = WardRoomJsonVO::createShared();

    // 参数校验
    // 非空校验
    if (!id)
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    // 执行数据查询
    auto resDo = WardService().getRoomById(id.getValue({}));
    if (!resDo) {
        jvo->init(nullptr, RS_FAIL);
        return jvo;
    }

    // 将 DO 转为 DTO
    auto dto = WardRoomDTO::createShared();
    dto->id = resDo->getId();
    dto->name = resDo->getName();
    // 将 parentId 作为 wardAreaId
    dto->wardAreaId = resDo->getParentId();

    jvo->success(dto);

    //响应结果
    return jvo;
}

WardBedJsonVO::Wrapper WardController::execQueryBedById(const String& id)
{
    // 定义返回数据对象
    auto jvo = WardBedJsonVO::createShared();

    // 参数校验
    if (!id)
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    // 执行数据查询
    auto resDo = WardService().getBedById(id.getValue({}));
    if (!resDo) {
        jvo->init(nullptr, RS_FAIL);
        return jvo;
    }

    // 将 DO 转为 DTO
    auto dto = WardBedDTO::createShared();
    dto->id = resDo->getId();
    dto->name = resDo->getName();
    // DO 的 statusEnum 是字符串，这里用默认值 0
    dto->status = 0;
    dto->wardRoomId = resDo->getParentId();

    jvo->success(dto);

    //响应结果
    return jvo;
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
