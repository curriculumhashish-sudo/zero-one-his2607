#include "stdafx.h"
#include "WardController.h"
#include "StringUtil.h"
#include "service/ward/WardService.h"

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
    
    if(organizationId.empty()) {
        response->init(nullptr, RS_PARAMS_INVALID);
		return response;
    }
    auto data = WardService().GetTree(query);

    if (!data)
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    response->success(data);
    return response;
}
StringJsonVO::Wrapper WardController::execAddLocation(
    const WardLocationAddDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto response = StringJsonVO::createShared();

    if (!dto)
    {
        response->init(
            "Request body is required.",
            RS_PARAMS_INVALID
        );
        return response;
    }

    const std::string id =
        WardService().createLocation(dto, payload);

    if (id.empty())
    {
        response->fail("Failed to create location.");
        return response;
    }

    response->success(id);
    return response;
}