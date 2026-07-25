#pragma once

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pay_manage/Pay_ManageGetPatientQueueQuery.h"
#include "domain/vo/pay_manage/Pay_ManageGetPatientQueueVO.h"
#include "domain/DTO/pay_manage/Pay_ManageGetPatientQueueDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.pay_manage.tag")

class Pay_ManageGetPatientQueueController :public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(Pay_ManageGetPatientQueueController);
public:
	ENDPOINT_INFO(queryPay_ManageGetPatientQueue) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("charging.pay_manage.GetPatientQueue.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(Pay_ManageGetPatientQueuePageJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		//查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "hospitalized_number", ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_number"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("byj.charging.pay_manage.name"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "bed_number", ZH_WORDS_GETTER("yxy.charging.register.bed_number"), "", false);
		API_DEF_ADD_QUERY_PARAMS(UInt64, "hospitalized_time", ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_time"), 20260708, true);
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