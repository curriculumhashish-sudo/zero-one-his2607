#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/21
*/
#ifndef _PRINT_LOG_CONTROLLER_
#define _PRINT_LOG_CONTROLLER_

#include "ApiHelper.h"
#include "domain/vo/opnurse3/PrintLogVO.h"
#include "domain/query/opnurse3/PrintLogQuery.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("print.tag.text")


class PrintLogController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PrintLogController);
public:
	ENDPOINT_INFO(queryRecords) {
		API_DEF_ADD_AUTH();
		info->summary = ZH_WORDS_GETTER("print.records.summary");
		API_DEF_ADD_RSP_JSON_WRAPPER(PrintLogPageJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		API_DEF_ADD_QUERY_PARAMS(String, "docType", ZH_WORDS_GETTER("print.field.docType"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "pname", ZH_WORDS_GETTER("print.field.patientName"), "", false);
		API_DEF_ADD_QUERY_PARAMS(Int32, "isReprint", ZH_WORDS_GETTER("print.field.isReprint"), 0, false);
		API_DEF_ADD_QUERY_PARAMS(String, "timefrom", ZH_WORDS_GETTER("print.field.timefrom"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "timeto", ZH_WORDS_GETTER("print.field.timeto"), "", false);
	}
	// GET /print/records — execution records
	ENDPOINT(API_M_GET, "/print/records", queryRecords, QUERIES(QueryParams, params), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(query, PrintLogQuery, params);
		return createDtoResponse(Status::CODE_200, executeQueryRecords(query));
	}

private:
	PrintLogPageJsonVO::Wrapper executeQueryRecords(const PrintLogQuery::Wrapper& query);
};

#include OATPP_CODEGEN_END(ApiController)
#endif // !_PRINT_LOG_CONTROLLER_
