#pragma once
#ifndef _RENT_CONTROLLER_
#define _RENT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/rent/RentQuery.h"
#include "domain/vo/rent/RentVO.h"

// 0 定义API控制器使用宏
#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("rent.tags.t1")
/**
 * 获取租户列表接口
 */
class RentController : public oatpp::web::server::api::ApiController // 1 继承控制器
{
	// 2 定义控制器访问入口
	API_ACCESS_DECLARE(RentController);
	// 3 定义接口
public:
	// 3.1 租户分页查询接口描述
	ENDPOINT_INFO(RentPageQuery) {
		// 定义接口标题
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("rent.get.summary1"));
		// 定义默认授权参数（可选定义，如果定义了，下面ENDPOINT里面需要加入API_HANDLER_AUTH_PARAME）
		API_DEF_ADD_AUTH();
		// 定义标签
		API_DEF_ADD_TAG(API_TAG);
		// 定义响应参数格式
		API_DEF_ADD_RSP_JSON_WRAPPER(RentDetailPageJsonVO);
		// 定义分页查询参数描述
		API_DEF_ADD_PAGE_PARAMS();
		// 定义其他查询参数描述

		API_DEF_ADD_QUERY_PARAMS(String, "code", ZH_WORDS_GETTER("rent.field.code"), "DEFAULT", false);
		API_DEF_ADD_QUERY_PARAMS(String, "keywords", ZH_WORDS_GETTER("rent.field.keywords"), "DEFAULT_RENT", false);
		API_DEF_ADD_QUERY_PARAMS(Int32, "status", ZH_WORDS_GETTER("rent.field.status"), 1, false);
	}
	// 3.2 租户分页查询接口处理
	ENDPOINT(API_M_GET, "/rent/page-query", RentPageQuery, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		// 解析查询参数为Query领域模型
		API_HANDLER_QUERY_PARAM(rentQuery, RentQuery, queryParams);
		// 呼叫执行函数响应结果
		API_HANDLER_RESP_VO(execRentPageQuery(rentQuery));
	}

	// 3.1 租户名称查询接口描述
	ENDPOINT_INFO(RentNameQuery) {
		// 定义接口标题
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("rent.get.summary2"));
		// 定义默认授权参数（可选定义，如果定义了，下面ENDPOINT里面需要加入API_HANDLER_AUTH_PARAME）
		API_DEF_ADD_AUTH();
		// 定义标签
		API_DEF_ADD_TAG(API_TAG);
		// 定义响应参数格式
		API_DEF_ADD_RSP_JSON_WRAPPER(RentNameJsonVO);

	}
	// 3.2 租户名称查询接口处理
	ENDPOINT(API_M_GET, "/rent/name", RentNameQuery, API_HANDLER_AUTH_PARAME) {
		// 呼叫执行函数响应结果
		API_HANDLER_RESP_VO(execRentNameQuery());
	}


private:
	// 3.3 演示分页查询数据
	RentDetailPageJsonVO::Wrapper execRentPageQuery(const RentQuery::Wrapper& query);
	RentNameJsonVO::Wrapper execRentNameQuery();
};

#undef API_TAG

// 0 取消API控制器使用宏
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif // _RENT_CONTROLLER_