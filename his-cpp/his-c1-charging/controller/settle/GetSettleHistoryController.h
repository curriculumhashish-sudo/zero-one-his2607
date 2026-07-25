#pragma once

#ifndef _GET_SETTLE_HISTORY_CONTROLLER_H_
#define _GET_SETTLE_HISTORY_CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/settle/GetSettleHistoryQuery.h"
#include "domain/vo/settle/GetSettleHistoryVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.settle.tag")

class GetSettleHistoryController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(GetSettleHistoryController);

public:
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("ovo.charging.settle.GetSettleHistory.summary"),
		getSettleHistory,
		GetSettleHistoryQuery,
		GetSettleHistoryJsonVO::Wrapper,
		API_TAG);

	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/charging/settle/gethistory",
		getSettleHistory,
		GetSettleHistoryQuery,
		execGetSettleHistory(query));

private:
	GetSettleHistoryJsonVO::Wrapper execGetSettleHistory(const GetSettleHistoryQuery::Wrapper& query);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_GET_SETTLE_HISTORY_CONTROLLER_H_
