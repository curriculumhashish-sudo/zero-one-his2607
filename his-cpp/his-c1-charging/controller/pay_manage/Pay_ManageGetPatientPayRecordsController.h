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
#include "domain/query/pay_manage/Pay_ManageGetPatientPayRecordsQuery.h"
#include "domain/vo/pay_manage/Pay_ManageGetPatientPayRecordsVO.h"
#include "domain/dto/pay_manage/Pay_ManageGetPatientPayRecordsDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.pay_manage.tag")

class Pay_ManageGetPatientPayRecordsController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(Pay_ManageGetPatientPayRecordsController);
public:
	ENDPOINT_INFO(queryPay_ManageGetPatientPayRecords) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("charging.pay_manage.GetPatientPayRecords.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(Pay_ManageGetPatientPayRecordsPageJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		// Query parameter: inpatient number to identify the selected patient
		API_DEF_ADD_QUERY_PARAMS(String, "hospitalized_number", ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_number"), "", false);
	}
	ENDPOINT(API_M_GET, "/charging/pay_manage/getpatientpayrecords", queryPay_ManageGetPatientPayRecords, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(Pay_ManageGetPatientPayRecordsQuery, Pay_ManageGetPatientPayRecordsQuery, queryParams);
		API_HANDLER_RESP_VO(execQueryPay_ManageGetPatientPayRecords(Pay_ManageGetPatientPayRecordsQuery));
	}
private:
	Pay_ManageGetPatientPayRecordsPageJsonVO::Wrapper execQueryPay_ManageGetPatientPayRecords(const Pay_ManageGetPatientPayRecordsQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
