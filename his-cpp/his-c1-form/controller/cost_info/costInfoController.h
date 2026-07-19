#pragma once

#ifndef COSTINFO_CONTROLLER_H
#define COSTINFO_CONTROLLER_H

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/cost_info/queryCostInfoDTO.h"
#include "domain/query/cost_info/costInfoQuery.h"
#include "domain/vo/cost_info/queryCostInfoVO.h"



#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin ApiController codegen section

class CostInfoController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(CostInfoController);
public:
	// 定义查询接口描述
	ENDPOINT_INFO(queryTest) {
		// 定义授权描述
		API_DEF_ADD_AUTH();
		// 定义接口标题
		info->summary = "title";
		// 定义响应格式
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		// 定义请求参数
		API_DEF_ADD_PAGE_PARAMS();
	}
	// 定义查询接口端点
	ENDPOINT(API_M_GET, "/test", queryTest, QUERIES(QueryParams, params), API_HANDLER_AUTH_PARAME) {
		// 解析参数
		API_HANDLER_QUERY_PARAM(query, PageQuery, params);
		// 执行查询并返回结果
		return createDtoResponse(Status::CODE_200, executeQueryTest(query));
	}
private:
	// 定义查询接口执行函数
	StringJsonVO::Wrapper executeQueryTest(const PageQuery::Wrapper& query);
};

#include OATPP_CODEGEN_END(ApiController) ///< End ApiController codegen section
#endif // !_TESTCONTORLLER_H_