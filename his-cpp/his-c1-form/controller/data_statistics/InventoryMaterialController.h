#pragma once
#ifndef _INVENTORYCONTROLLER_H_
#define _INVENTORYCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/data_statistics/InventoryMaterialQuery.h"
#include "domain/vo/data_statistics/InventoryMaterialVo.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("Litguidyo.form.inoutbound.tag2")

class InventoryMaterialController : public oatpp::web::server::api::ApiController {
	// 定义授权处理器入口
	API_ACCESS_DECLARE(InventoryMaterialController);

public:
	ENDPOINT_INFO(queryObject1) {
		// 设置接口标题
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("Litguidyo.form.inoutbound.summary2"));
		// 定义默认授权参数
		API_DEF_ADD_AUTH();
		// 定义相应参数格式
		API_DEF_ADD_RSP_JSON_WRAPPER(InventoryMaterialPageJsonVo);
		// 定义标签
		API_DEF_ADD_TAG(API_TAG);
		// 定义分页查询参数描述
		API_DEF_ADD_PAGE_PARAMS();
		// 定义查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.name"), "medicine", false);
		API_DEF_ADD_QUERY_PARAMS(Int32, "time", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.time"), 0, false);
	}

	// 定义查询接口处理点
	ENDPOINT(API_M_GET, "/form/data_statistics/InventoryMaterial", queryObject1, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		// 解析查询参数到查询对象
		API_HANDLER_QUERY_PARAM(objectQuery1, InventoryMaterialQuery, queryParams);
		// 调用执行函数相应结果
		API_HANDLER_RESP_VO(execQueryObject1(objectQuery1));
	}

private:
	// 分页查询数据
	InventoryMaterialPageDto::Wrapper execQueryObject1(const InventoryMaterialQuery::Wrapper& query);
};

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // !_INVENTORYCONTROLLER_H_
