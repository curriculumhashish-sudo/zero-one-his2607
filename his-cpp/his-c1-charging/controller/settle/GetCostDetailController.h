#pragma once

#ifndef _GET_COST_DETAIL_CONTROLLER_H_
#define _GET_COST_DETAIL_CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/settle/GetCostDetailQuery.h"
#include "domain/vo/settle/GetCostDetailVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.settle.tag")

class GetCostDetailController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(GetCostDetailController);

public:
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("ovo.charging.settle.GetCostDetail.summary"),
		getCostDetail,
		GetCostDetailQuery,
		GetCostDetailJsonVO::Wrapper,
		API_TAG);

	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/charging/settle/getcostdetail",
		getCostDetail,
		GetCostDetailQuery,
		execGetCostDetail(query));

private:
	GetCostDetailJsonVO::Wrapper execGetCostDetail(const GetCostDetailQuery::Wrapper& query);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_GET_COST_DETAIL_CONTROLLER_H_
