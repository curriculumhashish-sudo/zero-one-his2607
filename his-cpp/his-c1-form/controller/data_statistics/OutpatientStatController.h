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
#ifndef _OUTPATIENT_STAT_CONTROLLER_
#define _OUTPATIENT_STAT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/data_statistics/OutpatientStatVO.h"
#include "domain/query/data_statistics/OutpatientTrendQuery.h"
#include "domain/query/data_statistics/DiseaseDistributionQuery.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("form.data_statistics.tag")

class OutpatientStatController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(OutpatientStatController);
public:
	// 1. Outpatient visit trend (weekly / monthly, controlled by timeType)
	ENDPOINT_INFO(queryTrend) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("W.form.data_statistics.query_trend.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(OutpatientTrendJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_QUERY_PARAMS(Int32, "timeType", ZH_WORDS_GETTER("W.form.data_statistics.field.time_type"), 1, true);
		API_DEF_ADD_QUERY_PARAMS(String, "startDate", ZH_WORDS_GETTER("W.form.data_statistics.field.start_date"), "2026-07-14", true);
		API_DEF_ADD_QUERY_PARAMS(String, "endDate", ZH_WORDS_GETTER("W.form.data_statistics.field.end_date"), "2026-07-20", true);
		API_DEF_ADD_QUERY_PARAMS(String, "orgId", ZH_WORDS_GETTER("W.form.data_statistics.field.org_id"), "1001", false);
	}
	ENDPOINT(API_M_GET, "/form/data_statistics/query_trend", queryTrend, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(query, OutpatientTrendQuery, queryParams);
		API_HANDLER_RESP_VO(execQueryTrend(query));
	}

	// 2. Disease type distribution
	ENDPOINT_INFO(queryDiseaseDistribution) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("W.form.data_statistics.query_disease_distribution.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(DiseaseDistributionJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_QUERY_PARAMS(String, "startDate", ZH_WORDS_GETTER("W.form.data_statistics.field.start_date"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "endDate", ZH_WORDS_GETTER("W.form.data_statistics.field.end_date"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "orgId", ZH_WORDS_GETTER("W.form.data_statistics.field.org_id"), "1001", false);
	}
	ENDPOINT(API_M_GET, "/form/data_statistics/query_disease_distribution", queryDiseaseDistribution, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(query, DiseaseDistributionQuery, queryParams);
		API_HANDLER_RESP_VO(execQueryDiseaseDistribution(query));
	}

private:
	OutpatientTrendJsonVO::Wrapper execQueryTrend(const OutpatientTrendQuery::Wrapper& query);
	DiseaseDistributionJsonVO::Wrapper execQueryDiseaseDistribution(const DiseaseDistributionQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif
