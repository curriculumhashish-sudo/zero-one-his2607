#pragma once

#ifndef COSTINFO_CONTROLLER_H
#define COSTINFO_CONTROLLER_H

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/cost_info/costInfoDTO.h"
#include "domain/query/cost_info/costInfoQuery.h"
#include "domain/vo/cost_info/costInfoVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("form.cost_info.tag")

class CostInfoController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(CostInfoController)
public:

	// 获取报表明细
	ENDPOINT_INFO(queryCostInfo) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("rhqm.form.cost_info.query.title"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(CostInfoJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		API_DEF_ADD_QUERY_PARAMS(String, "start_time", ZH_WORDS_GETTER("rhqm.form.cost_info.query.start_time"), "", true);
		API_DEF_ADD_QUERY_PARAMS(String, "end_time", ZH_WORDS_GETTER("rhqm.form.cost_info.query.end_time"), "", true);
		API_DEF_ADD_QUERY_PARAMS(String, "room", ZH_WORDS_GETTER("rhqm.form.cost_info.query.room"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "feeClass", ZH_WORDS_GETTER("rhqm.form.cost_info.query.feeClass"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "detailClass", ZH_WORDS_GETTER("rhqm.form.cost_info.query.detailClass"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "projectName", ZH_WORDS_GETTER("rhqm.form.cost_info.query.projectName"), "", false);
		API_DEF_ADD_QUERY_PARAMS(UInt64, "hisID", ZH_WORDS_GETTER("rhqm.form.cost_info.query.hisID"), 1, false);
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("rhqm.form.cost_info.query.name"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "checker", ZH_WORDS_GETTER("rhqm.form.cost_info.query.checker"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "feeDir", ZH_WORDS_GETTER("rhqm.form.cost_info.query.feeDir"), "", false);
	}
	ENDPOINT(API_M_GET, "/form/cost_info/query", queryCostInfo, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(costInfoQuery, CostInfoQuery, queryParams);
		API_HANDLER_RESP_VO(execQuery(costInfoQuery));
	}

	
	// 导出报表
	ENDPOINT_INFO(exportCostInfo) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("rhqm.form.cost_info.export.title"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(ExportVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		API_DEF_ADD_QUERY_PARAMS(String, "start_time", ZH_WORDS_GETTER("rhqm.form.cost_info.query.start_time"), "", true);
		API_DEF_ADD_QUERY_PARAMS(String, "end_time", ZH_WORDS_GETTER("rhqm.form.cost_info.query.end_time"), "", true);
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("rhqm.form.cost_info.query.name"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "room", ZH_WORDS_GETTER("rhqm.form.cost_info.query.room"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "feeClass", ZH_WORDS_GETTER("rhqm.form.cost_info.query.feeClass"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "detailClass", ZH_WORDS_GETTER("rhqm.form.cost_info.query.detailClass"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "projectName", ZH_WORDS_GETTER("rhqm.form.cost_info.query.projectName"), "", false);
		API_DEF_ADD_QUERY_PARAMS(UInt64, "hisID", ZH_WORDS_GETTER("rhqm.form.cost_info.query.hisID"), 1, false);
		API_DEF_ADD_QUERY_PARAMS(String, "checker", ZH_WORDS_GETTER("rhqm.form.cost_info.query.checker"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "feeDir", ZH_WORDS_GETTER("rhqm.form.cost_info.query.feeDir"), "", false);
	}
	ENDPOINT(API_M_GET, "/form/cost_info/export", exportCostInfo, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(costInfoExport, CostInfoQuery, queryParams);
		return execExport(costInfoExport);
	}

private:
	CostInfoJsonVO::Wrapper execQuery(const CostInfoQuery::Wrapper& query);
	std::shared_ptr<OutgoingResponse> execExport(const CostInfoQuery::Wrapper& query);

};

#undef API_TAG


#include OATPP_CODEGEN_END(ApiController) 
#endif 