#pragma once
#ifndef _INOUTBOUNDCONTROLLER_H_
#define _INOUTBOUNDCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/data_statistics/InOutboundQuery.h"
#include "domain/vo/data_statistics/InOutboundVo.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("form.data_statistics.tag")

class InOutboundController : public oatpp::web::server::api::ApiController {
	// 定义授权处理器入口
	API_ACCESS_DECLARE(InOutboundController);

public:
	ENDPOINT_INFO(queryObject) {
		// 设置接口标题
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("Litguidyo.form.inoutbound.summary"));
		// 定义默认授权参数
		API_DEF_ADD_AUTH();
		// 定义相应参数格式
		API_DEF_ADD_RSP_JSON_WRAPPER(InOutboundPageJsonVo);
		// 定义标签
		API_DEF_ADD_TAG(API_TAG);
		// 定义分页查询参数描述
		API_DEF_ADD_PAGE_PARAMS();
		// 定义查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.name"), "", false);
		API_DEF_ADD_QUERY_PARAMS(Float64, "onemoney", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.onemoney"), 0.0, false);
		API_DEF_ADD_QUERY_PARAMS(Float64, "allmoney", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.allmoney"), 0.0, false);
		API_DEF_ADD_QUERY_PARAMS(Float64, "outmoney", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.outmoney"), 0.0, false);
		API_DEF_ADD_QUERY_PARAMS(Float64, "alloutmoney", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.alloutmoney"), 0.0, false);
		API_DEF_ADD_QUERY_PARAMS(String, "batch_number", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.batch_number"), "", false);
	}

	// 定义查询接口处理点
	ENDPOINT(API_M_GET, "/form/data_statistics/InOutbound", queryObject, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		// 解析查询参数到查询对象
		API_HANDLER_QUERY_PARAM(objectQuery, InOutboundQuery, queryParams);
		// 调用执行函数相应结果
		API_HANDLER_RESP_VO(execQueryObject(objectQuery));
	}

private:
	// 分页查询数据
	InOutboundPageJsonVo::Wrapper execQueryObject(const InOutboundQuery::Wrapper& query);
};

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // !_INOUTBOUNDCONTROLLER_H_
