#pragma once
#ifndef _ORDER_CONTROLLER_H_
#define _ORDER_CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/order/OrderQuery1.h"
#include "domain/vo/order/OrderVO1.h"
#include "domain/dto/order/OrderAddDTO1.h"
#include "domain/dto/order/OrderDetailDTO1.h"
#include "domain/dto/order/OrderModifyDTO1.h"
#include "domain/dto/order/CatalogItemDTO1.h"
#include "domain/query/order/CatalogQuery1.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("prescription1.order.tags.tag")

class OrderController1 : public oatpp::web::server::api::ApiController {
	// 定义控制器访问入口
	API_ACCESS_DECLARE(OrderController1);
public: // 定义接口
	// 定义分页查询医嘱接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("prescription1.order.query.summary"), queryOrder, OrderQuery1, OrderDetailPageJsonVO1::Wrapper, API_TAG);
	// 定义分页查询医嘱接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/order/query", queryOrder, OrderQuery1, execQueryOrder(query));

	// 定义新增接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("prescription1.order.add.summary"), addOrder, StringJsonVO::Wrapper, API_TAG);
	// 定义新增接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/order/add", addOrder, BODY_DTO(OrderAddDTO1::Wrapper, dto), execAddOrder(dto, authObject->getPayload()));

	// 定义修改接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("prescription1.order.modify.summary"), modifyOrder, StringJsonVO::Wrapper, API_TAG);
	// 定义修改接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/order/modify", modifyOrder, BODY_DTO(OrderModifyDTO1::Wrapper, dto), execModifySample(dto, authObject->getPayload()));


	// 定义分页查询搜索框内药品的接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("prescription1.order.catalog.summary"), queryCatalog, CatalogQuery1, CatalogItemPageJsonVO1::Wrapper, API_TAG);
	// 定义分页查询搜索框内药品的接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/order/catalog", queryCatalog, CatalogQuery1, execQueryCatalog(query));
private: // 定义接口执行函数
	// 定义分页查询医嘱接口执行函数
	OrderDetailPageJsonVO1::Wrapper execQueryOrder(const OrderQuery1::Wrapper& query);
	// 定义新增接口执行函数
	StringJsonVO::Wrapper execAddOrder(const OrderAddDTO1::Wrapper& dto, const PayloadDTO& patload);
	// 定义修改接口执行函数
	StringJsonVO::Wrapper execModifySample(const OrderModifyDTO1::Wrapper& dto, const PayloadDTO& patload);
	// 定义分页查询搜索框内药品的接口执行函数
	CatalogItemPageJsonVO1::Wrapper execQueryCatalog(const CatalogQuery1::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif