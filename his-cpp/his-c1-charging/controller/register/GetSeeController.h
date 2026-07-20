#pragma once

#ifndef _GET_SEE_CONTROLLER_
#define _GET_SEE_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/register/GetSeeQuery.h"
#include "domain/vo/register/GetSeeVO.h"
#include "domain/DTO/register/GetSeeDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)


#define API_TAG ZH_WORDS_GETTER("charging.register.tag")

class GetSeeController:public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(GetSeeController);
public:
	ENDPOINT_INFO(queryGetSee) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("yxy.charging.register.summary1"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(GetSeePageJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		API_DEF_ADD_QUERY_PARAMS(String, "number", ZH_WORDS_GETTER("yxy.charging.register.number"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("yxy.charging.register.name"), "", false);
		API_DEF_ADD_QUERY_PARAMS(UInt64, "start_time", ZH_WORDS_GETTER("yxy.charging.register.start_time"), 1, true);
		API_DEF_ADD_QUERY_PARAMS(UInt64, "end_time", ZH_WORDS_GETTER("yxy.charging.register.end_time"), 253370736000, true);
	}
	ENDPOINT(API_M_GET,"/charging/register/getseeing",queryGetSee, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME){
		API_HANDLER_QUERY_PARAM(getseeQuery, GetSeeQuery, queryParams);
		API_HANDLER_RESP_VO(execQueryGetSee(getseeQuery));
	}
private:
	GetSeePageJsonVO::Wrapper execQueryGetSee(const GetSeeQuery::Wrapper& query);
};
		
#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif
