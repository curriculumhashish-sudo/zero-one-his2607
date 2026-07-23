#pragma once
#ifndef _WARD_CONTROLLER_H_
#define _WARD_CONTROLLER_H_
#include "ApiHelper.h"
#include "domain/query/ward/WardQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/ward/WardNameVO.h"
#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("ward.tags.t1")
// WardController declaration goes here.
class WardController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(WardController);

public:
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

private:

    ListJsonVO<WardNameVO::Wrapper>::Wrapper execWardNameList(const WardQuery::Wrapper& query);
    

};






#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif 