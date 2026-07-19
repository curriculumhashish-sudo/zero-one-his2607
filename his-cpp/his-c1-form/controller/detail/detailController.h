#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/12 21:09:46

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
#ifndef DETAIL_CONTROLLER_H
#define DETAIL_CONTROLLER_H

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/detail/queryDetailDTO.h"
#include "domain/query/detail/detailQuery.h"
#include "domain/vo/detail/queryDetailVO.h"



#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin ApiController codegen section

class DetailController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(DetailController);
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