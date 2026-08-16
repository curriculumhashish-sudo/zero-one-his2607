#pragma once

#ifndef _GET_TODAY_APPOINT_CONTROLLER_
#define _GET_TODAY_APPOINT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/register/GetTodayAppointQuery.h"
#include "domain/vo/register/GetTodayAppointVO.h"
#include "domain/dto/register/GetTodayAppointDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)


#define API_TAG ZH_WORDS_GETTER("charging.register.tag")

class GetTodayAppointController :public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(GetTodayAppointController);
public:
	ENDPOINT_INFO(queryGetTodayAppoint) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("yxy.charging.register.summary2"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(GetTodayAppointJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		API_DEF_ADD_QUERY_PARAMS(String, "doctor", ZH_WORDS_GETTER("yxy.charging.register.doctor"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "register_type", ZH_WORDS_GETTER("yxy.charging.register.register_type"), "", false);
	}
	ENDPOINT(API_M_GET, "/charging/register/get_today_appoint", queryGetTodayAppoint, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(getodayappointQuery, GetTodayAppointQuery, queryParams);
		API_HANDLER_RESP_VO(execQueryGetTodayAppoint(getodayappointQuery));
	}
private:
	GetTodayAppointJsonVO::Wrapper execQueryGetTodayAppoint(const GetTodayAppointQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif
