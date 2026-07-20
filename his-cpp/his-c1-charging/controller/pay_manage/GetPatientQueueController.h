#pragma once

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pay_manage/GetPatientQueueQuery.h"
#include "domain/vo/pay_manage/GetPatientQueueVO.h"
#include "domain/DTO/pay_manage/GetPatientQueueDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("byj.charging.pay_manage.tag1")

class GetPatientQueueController :public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(GetPatientQueueController);
public:
	ENDPOINT_INFO(queryGetPatientQueue) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("byj.charging.pay_manage.summary1"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(GetPatientQueuePageJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		//查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "hospitalized_number", ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_number"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("byj.charging.pay_manage.name"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "bed_number", ZH_WORDS_GETTER("yxy.charging.register.bed_number"), "", false);
		API_DEF_ADD_QUERY_PARAMS(UInt64, "hospitalized_time", ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_time"), 20260708, true);
	}
	ENDPOINT(API_M_GET, "/charging/pay_manage/getpatientqueue", queryGetPatientQueue, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(getpatientqueueQuery, GetPatientQueueQuery, queryParams);
		API_HANDLER_RESP_VO(execQueryGetPatientQueue(getpatientqueueQuery));
	}
private:
	GetPatientQueuePageJsonVO::Wrapper execQueryGetPatientQueue(const GetPatientQueueQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)