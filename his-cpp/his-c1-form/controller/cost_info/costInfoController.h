#pragma once

#ifndef COSTINFO_CONTROLLER_H
#define COSTINFO_CONTROLLER_H

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/cost_info/queryCostInfoDTO.h"
#include "domain/query/cost_info/costInfoQuery.h"
#include "domain/vo/cost_info/queryCostInfoVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

class CostInfoController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(CostInfoController);
public:
	API_DEF_ENDPOINT_INFO(
		ZH_WORDS_GETTER("file.download.summary"), queryInfo, StringJsonVO::Wrapper, ZH_WORDS_GETTER("rhqm.form.cost_info.query.title"),
		API_DEF_ADD_QUERY_PARAMS(String, "filename", ZH_WORDS_GETTER("rhqm.form.cost_info.export.filename"), "file/test.jpg", true);
	);
	// 定义端点
	API_HANDLER_ENDPOINT_OPTION(API_M_GET, "/form/cost_info/query", queryInfo, QUERY(QueryParams, query), return execQuery(query););

private:
	// 定义查询接口执行函数
	StringJsonVO::Wrapper execQuery(const PageQuery::Wrapper& query);
};

#include OATPP_CODEGEN_END(ApiController) 
#endif 