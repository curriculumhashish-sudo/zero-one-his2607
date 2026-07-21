#pragma once

#ifndef COSTINFO_CONTROLLER_H
#define COSTINFO_CONTROLLER_H

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/cost_info/queryCostInfoDTO.h"
#include "domain/query/cost_info/costInfoQuery.h"
#include "domain/vo/cost_info/queryCostInfoVO.h"
#include "domain/vo/cost_info/exportVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("form.cost_info.tag")

class CostInfoController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(CostInfoController)
public:
	ENDPOINT_INFO(queryTest) {
		API_DEF_ADD_AUTH();
		info->summary = API_TAG;
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		API_DEF_ADD_PAGE_PARAMS();
	}
	// 定义查询接口端点
	ENDPOINT(API_M_GET, "/test", queryTest, QUERIES(QueryParams, params), API_HANDLER_AUTH_PARAME) {
		// 解析参数
		API_HANDLER_QUERY_PARAM(query, PageQuery, params);
		// 执行查询并返回结果
		return createDtoResponse(Status::CODE_200, executeQueryTest(query));
	}


	// 获取报表明细接口
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("rhqm.form.cost_info.query.title"), queryCostInfo, CostInfoQuery, CostInfoJsonVO::Wrapper, API_TAG);

	API_HANDLER_ENDPOINT_OPTION_AUTH(API_M_GET, "/form/cost_info/query", queryCostInfo, QUERIES(QueryParams, queryParams),
		API_HANDLER_QUERY_PARAM(query, CostInfoQuery, queryParams); API_HANDLER_RESP_VO(execQuery(query)););

	// 导出接口
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("rhqm.form.cost_info.export.title"), exportCostInfo, CostInfoQuery, StringJsonVO::Wrapper, API_TAG);

	API_HANDLER_ENDPOINT_OPTION_AUTH(API_M_GET, "/form/cost_info/export", exportCostInfo, QUERIES(QueryParams, queryParams),
		API_HANDLER_QUERY_PARAM(query, ExportVO, queryParams); API_HANDLER_RESP_VO(execExport(query)););

private:
	CostInfoJsonVO::Wrapper execQuery(const CostInfoQuery::Wrapper& query);
	StringJsonVO::Wrapper execExport(const ExportVO::Wrapper& query);
};

#undef API_TAG


#include OATPP_CODEGEN_END(ApiController) 
#endif 