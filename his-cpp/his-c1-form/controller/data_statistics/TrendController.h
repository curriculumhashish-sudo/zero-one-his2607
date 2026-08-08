#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
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


	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("Lin.form.data_statistics.query_flow_trend.summary"),
		queryFlowTrend,
		TrafficTrendPageJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_PAGE_PARAMS();
		API_DEF_ADD_QUERY_PARAMS(String, "dateFrom", ZH_WORDS_GETTER("Lin.form.data_statistics.field.dateFrom"), "", true);
		API_DEF_ADD_QUERY_PARAMS(String, "dateTo", ZH_WORDS_GETTER("Lin.form.data_statistics.field.dateTo"), "", true);
		API_DEF_ADD_QUERY_PARAMS(String, "organizationId", ZH_WORDS_GETTER("Lin.form.data_statistics.field.organizationId"), "", true);
	);

	ENDPOINT(API_M_GET, "/data-statistics/flow-trend", queryFlowTrend, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		// 解析查询参数为Query领域模型
		API_HANDLER_QUERY_PARAM(query, TrafficTrendQuery, queryParams);
		// 呼叫执行函数响应结果
		API_HANDLER_RESP_VO(execQueryFlowTrend(query));
	}



private:


	TrafficTrendPageJsonVO::Wrapper execQueryFlowTrend(
		const TrafficTrendQuery::Wrapper& query
	);

};


#undef API_TAG


#include OATPP_CODEGEN_END(ApiController)

#endif // !_FLOW_TREND_CONTROLLER_
