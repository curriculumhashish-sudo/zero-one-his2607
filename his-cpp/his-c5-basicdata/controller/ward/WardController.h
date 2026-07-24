//#pragma once
//#ifndef _WARD_CONTROLLER_H_
//#define _WARD_CONTROLLER_H_
//
//#include "ApiHelper.h"
//#include "domain/query/ward/WardQuery.h"
//#include "domain/vo/BaseJsonVO.h"
//#include "domain/vo/ward/WardNameVO.h"
//
//#include OATPP_CODEGEN_BEGIN(ApiController)
//
//#define API_TAG ZH_WORDS_GETTER("ward.tags.t1")
//
//class WardController
//    : public oatpp::web::server::api::ApiController
//{
//    API_ACCESS_DECLARE(WardController);
//
//public:
//    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
//        ZH_WORDS_GETTER("ward.name-list.summary"),
//        queryWardNameList,
//        WardQuery,
//        ListJsonVO<WardNameVO::Wrapper>::Wrapper,
//        API_TAG
//    );
//
//    API_HANDLER_ENDPOINT_QUERY_AUTH(
//        API_M_GET,
//        "/ward/name-list",
//        queryWardNameList,
//        WardQuery,
//        execWardNameList(query)
//    );
//
//private:
//    ListJsonVO<WardNameVO::Wrapper>::Wrapper
//    execWardNameList(
//        const WardQuery::Wrapper& query
//    );
//};
//
//#undef API_TAG
//
//#include OATPP_CODEGEN_END(ApiController)
//
//#endif


#pragma once
#ifndef _WARD_CONTROLLER_H_
#define _WARD_CONTROLLER_H_

#include "ApiHelper.h"
#include "domain/query/ward/WardTreeQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/ward/WardTreeNodeVO.h"
#include "domain/dto/ward/WardLocationAddDTO.h"
#include OATPP_CODEGEN_BEGIN(ApiController)


#define API_TAG ZH_WORDS_GETTER("ward.tags.t1")


class WardController
    : public oatpp::web::server::api::ApiController
{

    API_ACCESS_DECLARE(WardController);


public:

    //接口描述
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("ward.tree-list.summary"),
        queryWardTree,
        WardTreeQuery,
        ListJsonVO<WardTreeNodeVO::Wrapper>::Wrapper,
        API_TAG
    );


    //接口路径
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/ward/tree-list",
        queryWardTree,
        WardTreeQuery,
        execWardTree(query)
    );

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
        execAddLocation(dto, authObject->getPayload())
    );
private:


    ListJsonVO<WardTreeNodeVO::Wrapper>::Wrapper
        WardController ::execWardTree(
            const WardTreeQuery::Wrapper& query
        );

    StringJsonVO::Wrapper    WardController::execAddLocation(
        const WardLocationAddDTO::Wrapper& dto,
        const PayloadDTO& payload);
};


#undef API_TAG


#include OATPP_CODEGEN_END(ApiController)

#endif