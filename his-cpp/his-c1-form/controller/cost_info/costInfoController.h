#pragma once

#ifndef COSTINFO_CONTROLLER_H
#define COSTINFO_CONTROLLER_H

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/cost_info/queryCostInfoDTO.h"
#include "domain/query/cost_info/costInfoQuery.h"
#include "domain/vo/cost_info/queryCostInfoVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

class CostInfoController : public oatpp::web::server::api::ApiController
{
	ENDPOINT_INFO(queryInfo) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("rhqm.form.cost_info.query.title"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(GetSeePageJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		API_DEF_ADD_QUERY_PARAMS(String, "number", ZH_WORDS_GETTER("yxy.charging.register.number"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("yxy.charging.register.name"), "", false);
		API_DEF_ADD_QUERY_PARAMS(UInt64, "start_time", ZH_WORDS_GETTER("rhqm.form.cost_info.query.start_time"), 1, true);
		API_DEF_ADD_QUERY_PARAMS(UInt64, "end_time", ZH_WORDS_GETTER("rhqm.form.cost_info.query.end_time"), 253370736000, true);
	}
	ENDPOINT(API_M_GET, "/charging/register/getseeing", queryInfo, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(getseeQuery, GetSeeQuery, queryParams);
		API_HANDLER_RESP_VO(execQuery(getseeQuery));
	}

private:
	// 定义查询接口执行函数
	StringJsonVO::Wrapper execQuery(const PageQuery::Wrapper& query);
};

#include OATPP_CODEGEN_END(ApiController) 
#endif 