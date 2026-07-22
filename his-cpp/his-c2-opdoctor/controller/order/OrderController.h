#pragma once
#ifndef _ORDER_CONTROLLER_H_
#define _ORDER_CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/order/OrderQuery.h"
#include "domain/vo/order/OrderVO.h"
#include "domain/dto/order/OrderAddDTO.h"
#include "domain/dto/order/OrderDetailDTO.h"
#include "domain/dto/order/OrderModifyDTO.h"
#include "domain/dto/order/CatalogItemDTO.h"
#include "domain/query/order/CatalogQuery.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("order.tags.tag")

class OrderController : public oatpp::web::server::api::ApiController {
	// 定义控制器访问入口
	API_ACCESS_DECLARE(OrderController);
public: // 定义接口
	// 定义分页查询医嘱接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("order.query.summary"), queryOrder, OrderQuery, OrderDetailPageJsonVO::Wrapper, API_TAG);
	// 定义分页查询医嘱接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/order/query", queryOrder, OrderQuery, execQueryOrder(query));

	// 定义新增接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("order.add.summary"), addOrder, StringJsonVO::Wrapper, API_TAG);
	// 定义新增接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/order/add", addOrder, BODY_DTO(OrderAddDTO::Wrapper, dto), execAddOrder(dto, authObject->getPayload()));

	// 定义修改接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("order.modify.summary"), modifyOrder, StringJsonVO::Wrapper, API_TAG);
	// 定义修改接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/order/modify", modifyOrder, BODY_DTO(OrderModifyDTO::Wrapper, dto), execModifySample(dto, authObject->getPayload()));


	// 定义分页查询搜索框内药品的接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("order.catalog.summary"), queryCatalog, CatalogQuery, CatalogItemPageJsonVO::Wrapper, API_TAG);
	// 定义分页查询搜索框内药品的接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/order/catalog", queryCatalog, CatalogQuery, execQueryCatalog(query));
private: // 定义接口执行函数
	// 定义分页查询医嘱接口执行函数
	OrderDetailPageJsonVO::Wrapper execQueryOrder(const OrderQuery::Wrapper& query);
	// 定义新增接口执行函数
	StringJsonVO::Wrapper execAddOrder(const OrderAddDTO::Wrapper& dto, const PayloadDTO& patload);
	// 定义修改接口执行函数
	StringJsonVO::Wrapper execModifySample(const OrderModifyDTO::Wrapper& dto, const PayloadDTO& patload);
	// 定义分页查询搜索框内药品的接口执行函数
	CatalogItemPageJsonVO::Wrapper execQueryCatalog(const CatalogQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif