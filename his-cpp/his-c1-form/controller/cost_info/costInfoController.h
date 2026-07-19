#pragma once

#ifndef COSTINFO_CONTROLLER_H
#define COSTINFO_CONTROLLER_H

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/cost_info/queryCostInfoDTO.h"
#include "domain/query/cost_info/costInfoQuery.h"
#include "domain/vo/cost_info/queryCostInfoVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("rhqm.form.cost_info.query.tag")

class CostInfoController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(CostInfoController)
public:
	ENDPOINT_INFO(queryInfo) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("rhqm.form.cost_info.query.title"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(CostInfoJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		API_DEF_ADD_QUERY_PARAMS(UInt64, "start_time", ZH_WORDS_GETTER("rhqm.form.cost_info.query.start_time"), 1, true);
		API_DEF_ADD_QUERY_PARAMS(UInt64, "end_time", ZH_WORDS_GETTER("rhqm.form.cost_info.query.end_time"), 253370736000, true);
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("rhqm.form.cost_info.query.name"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "room", ZH_WORDS_GETTER("rhqm.form.cost_info.query.room"), "", true);
		API_DEF_ADD_QUERY_PARAMS(String, "feeClass", ZH_WORDS_GETTER("rhqm.form.cost_info.query.feeClass"), "", true);
		API_DEF_ADD_QUERY_PARAMS(String, "detailClass", ZH_WORDS_GETTER("rhqm.form.cost_info.query.detailClass"), "", true);
		API_DEF_ADD_QUERY_PARAMS(String, "prijectName", ZH_WORDS_GETTER("rhqm.form.cost_info.query.prijectName"), "", true);
		API_DEF_ADD_QUERY_PARAMS(UInt64, "hisID", ZH_WORDS_GETTER("rhqm.form.cost_info.query.hisID"), 1, true);
		API_DEF_ADD_QUERY_PARAMS(String, "checker", ZH_WORDS_GETTER("rhqm.form.cost_info.query.checker"), "", true);
		API_DEF_ADD_QUERY_PARAMS(String, "feeDir", ZH_WORDS_GETTER("rhqm.form.cost_info.query.feeDir"), "", true);
	}
	ENDPOINT(API_M_GET, "/charging/register/getseeing", queryInfo, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(query, CostInfoQuery, queryParams);
		API_HANDLER_RESP_VO(execQuery(query));
	}

private:
	// 定义查询接口执行函数
	CostInfoJsonVO::Wrapper execQuery(const CostInfoQuery::Wrapper& query);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController) 
#endif 