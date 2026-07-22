#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: ovo
 @Date: 2026/07/20

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
