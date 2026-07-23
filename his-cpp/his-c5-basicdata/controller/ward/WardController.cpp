#include "stdafx.h"
#include "WardController.h"
#include "service/ward/WardService.h"



ListJsonVO<WardNameVO::Wrapper>::Wrapper
WardController::execWardNameList(const WardQuery::Wrapper& query)
{

     auto ljv=ListJsonVO<WardNameVO::Wrapper>::createShared();

     if (!query || !query->organizationId || query->organizationId.getValue("").empty()) {
         ljv->init(nullptr, RS_PARAMS_INVALID);
         return ljv;
     }

     auto data = WardService().listNames(
         query->organizationId.getValue("")
         );

     ljv->success(data);

     return ljv;




}

ListJsonVO<WardItemVO::Wrapper>::Wrapper
WardController::execWardList(
    const WardListQuery::Wrapper& query)
{
    auto result =
        ListJsonVO<WardItemVO::Wrapper>::createShared();

    if (!query ||
        !query->organizationId ||
        query->organizationId.getValue("").empty())
    {
        result->init(nullptr, RS_PARAMS_INVALID);
        return result;
    }

    result->success(WardService().listWards(query));
    return result;
}




StringJsonVO::Wrapper execAddLocation(
    const WardLocationAddDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto result = StringJsonVO::createShared();

    if (!dto)
    {
        result->init(
            "Request body is required.",
            RS_PARAMS_INVALID
        );
        return result;
    }

    const std::string errorMessage = dto->validate();

    if (!errorMessage.empty())
    {
        result->init(
            errorMessage,
            RS_PARAMS_INVALID
        );
        return result;
    }

const std::string id =
    WardService().addLocation(dto, payload);

    if (id.empty())
    {
        result->fail("Failed to add location.");
        return result;
    }

    result->success(id);
    return result;
}