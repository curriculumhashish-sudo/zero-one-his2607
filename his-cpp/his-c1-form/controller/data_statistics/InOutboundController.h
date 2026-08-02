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
#ifndef _INOUTBOUNDCONTROLLER_H_
#define _INOUTBOUNDCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/data_statistics/InOutboundQuery.h"
#include "domain/vo/data_statistics/InOutboundVo.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("form.data_statistics.tag")

class InOutboundController : public oatpp::web::server::api::ApiController {
	// Declare auth access handler
	API_ACCESS_DECLARE(InOutboundController);

public:
	ENDPOINT_INFO(queryObject) {
		// Set endpoint title
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("Litguidyo.form.inoutbound.summary"));
		// Enable default auth
		API_DEF_ADD_AUTH();
		// Set response JSON wrapper type
		API_DEF_ADD_RSP_JSON_WRAPPER(InOutboundPageJsonVo);
		// Set tag
		API_DEF_ADD_TAG(API_TAG);
		// Set paged query parameters
		API_DEF_ADD_PAGE_PARAMS();
		// 定义查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.name"), "", false);
		API_DEF_ADD_QUERY_PARAMS(Float64, "onemoney", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.onemoney"), 0.0, false);
		API_DEF_ADD_QUERY_PARAMS(Float64, "allmoney", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.allmoney"), 0.0, false);
		API_DEF_ADD_QUERY_PARAMS(Float64, "outmoney", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.outmoney"), 0.0, false);
		API_DEF_ADD_QUERY_PARAMS(Float64, "alloutmoney", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.alloutmoney"), 0.0, false);
		API_DEF_ADD_QUERY_PARAMS(String, "batch_number", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.batch_number"), "", false);
	}

	// Query endpoint handler
	ENDPOINT(API_M_GET, "/form/data_statistics/InOutbound", queryObject, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		// Bind query parameters to query DTO
		API_HANDLER_QUERY_PARAM(objectQuery, InOutboundQuery, queryParams);
		// Execute and respond
		API_HANDLER_RESP_VO(execQueryObject(objectQuery));
	}

private:
	// Paged query handler
	InOutboundPageJsonVo::Wrapper execQueryObject(const InOutboundQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // !_INOUTBOUNDCONTROLLER_H_