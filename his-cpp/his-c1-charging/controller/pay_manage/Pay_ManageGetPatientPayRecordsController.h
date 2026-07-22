#pragma once

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pay_manage/Pay_ManageGetPatientPayRecordsQuery.h"
#include "domain/vo/pay_manage/Pay_ManageGetPatientPayRecordsVO.h"
#include "domain/DTO/pay_manage/Pay_ManageGetPatientPayRecordsDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.pay_manage.GetPatientPayRecords.summary")

class Pay_ManageGetPatientPayRecordsController :public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(Pay_ManageGetPatientPayRecordsController);
public:
	ENDPOINT_INFO(queryPay_ManageGetPatientPayRecords) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("charging.pay_manage.GetPatientPayRecords.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(Pay_ManageGetPatientPayRecordsPageJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
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