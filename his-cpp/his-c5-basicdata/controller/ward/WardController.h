#pragma once
#ifndef _WARD_CONTROLLER_H_
#define _WARD_CONTROLLER_H_
#include "ApiHelper.h"
#include "domain/query/ward/WardQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/ward/WardListQuery.h"
#include "domain/vo/ward/WardItemVO.h"
#include "domain/vo/ward/WardNameVO.h"
#include "domain/dto/ward/WardLocationAddDTO.h"
#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("ward.tags.t1")
// WardController declaration goes here.
class WardController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(WardController);

public:
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("ward.add-location.summary"),
        addWardLocation,
        StringJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/ward/location",
        addWardLocation,
        BODY_DTO(WardLocationAddDTO::Wrapper, dto),
        execAddLocation(
            dto,
            authObject->getPayload()
        )
    );



    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("ward.name-list.summary"),
        queryWardNameList,
        WardQuery,
        ListJsonVO<WardNameVO::Wrapper>::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/ward/name-list",
        queryWardNameList,
        WardQuery,
        execWardNameList(query)
        );
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("ward.list.summary"),
        queryWardList,
        WardListQuery,
        ListJsonVO<WardItemVO::Wrapper>::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/ward/list",
        queryWardList,
        WardListQuery,
        execWardList(query)
    );
private:

    ListJsonVO<WardNameVO::Wrapper>::Wrapper execWardNameList(const WardQuery::Wrapper& query);
    ListJsonVO<WardItemVO::Wrapper>::Wrapper
        execWardList(const WardListQuery::Wrapper& query);
StringJsonVO::Wrapper execAddLocation(
    const WardLocationAddDTO::Wrapper& dto,
    const PayloadDTO& payload);
};






#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif 