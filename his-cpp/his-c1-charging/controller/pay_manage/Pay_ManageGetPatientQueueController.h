#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: byj
 @Date: 2026/08/02

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

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pay_manage/Pay_ManageGetPatientQueueQuery.h"
#include "domain/vo/pay_manage/Pay_ManageGetPatientQueueVO.h"
#include "domain/dto/pay_manage/Pay_ManageGetPatientQueueDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.pay_manage.tag")

class Pay_ManageGetPatientQueueController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(Pay_ManageGetPatientQueueController);
public:
	ENDPOINT_INFO(queryPay_ManageGetPatientQueue) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("charging.pay_manage.GetPatientQueue.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(Pay_ManageGetPatientQueuePageJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		// Query parameters (all optional to support empty search)
		API_DEF_ADD_QUERY_PARAMS(String, "hospitalized_number", ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_number"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("byj.charging.pay_manage.name"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "bed_number", ZH_WORDS_GETTER("byj.charging.pay_manage.bed_number"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "hospitalized_time", ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_time"), "2026-08-01", false);
	}
	ENDPOINT(API_M_GET, "/charging/pay_manage/getpatientqueue", queryPay_ManageGetPatientQueue, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(Pay_ManageGetPatientQueueQuery, Pay_ManageGetPatientQueueQuery, queryParams);
		API_HANDLER_RESP_VO(execQueryPay_ManageGetPatientQueue(Pay_ManageGetPatientQueueQuery));
	}
private:
	Pay_ManageGetPatientQueuePageJsonVO::Wrapper execQueryPay_ManageGetPatientQueue(const Pay_ManageGetPatientQueueQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
