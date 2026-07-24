#ifndef _FLOW_TREND_CONTROLLER_
#define _FLOW_TREND_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/data_statistics/TrafficTrendQuery.h"
#include "domain/vo/data_statistics/TrafficTrendVO.h"


#include OATPP_CODEGEN_BEGIN(ApiController)


#define API_TAG ZH_WORDS_GETTER("form.data_statistics.tag")



class FlowTrendController : public oatpp::web::server::api::ApiController
{

	
	API_ACCESS_DECLARE(FlowTrendController);


public:

	
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("Lin.form.data_statistics.query_flow_trend.summary"),
		queryFlowTrend,
		TrafficTrendQuery,
		TrafficTrendPageJsonVO::Wrapper,
		API_TAG
	);


	
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/data-statistics/flow-trend",
		queryFlowTrend,
		TrafficTrendQuery,
		execQueryFlowTrend(query, authObject->getPayload())
	);



private:

	
	TrafficTrendPageJsonVO::Wrapper execQueryFlowTrend(
		const TrafficTrendQuery::Wrapper& query,
		const PayloadDTO& payload
	);

};


#undef API_TAG


#include OATPP_CODEGEN_END(ApiController)

#endif // !_FLOW_TREND_CONTROLLER_