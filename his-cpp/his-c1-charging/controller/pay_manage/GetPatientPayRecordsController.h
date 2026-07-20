#pragma once

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pay_manage/GetPatientPayRecordsQuery.h"
#include "domain/vo/pay_manage/GetPatientPayRecordsVO.h"
#include "domain/DTO/pay_manage/GetPatientPayRecordsDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("byj.charging.pay_manage.tag3")

class GetPatientPayRecordsController :public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(GetPatientPayRecordsController);
public:
	ENDPOINT_INFO(queryGetPatientPayRecords) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("byj.charging.pay_manage.summary3"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(GetPatientPayRecordsPageJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
	}
	ENDPOINT(API_M_GET, "/charging/pay_manage/getpatientpayrecords", queryGetPatientPayRecords, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(getpatientpayrecordsQuery, GetPatientPayRecordsQuery, queryParams);
		API_HANDLER_RESP_VO(execQueryGetPatientPayRecords(getpatientpayrecordsQuery));
	}
private:
	GetPatientPayRecordsPageJsonVO::Wrapper execQueryGetPatientPayRecords(const GetPatientPayRecordsQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)