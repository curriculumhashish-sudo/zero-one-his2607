#pragma once
#pragma once
#ifndef _ENTERORDERSCONTROLLER_H_
#define _ENTERORDERSCONTROLLER_H_
#include "../../domain/GlobalInclude.h"
#include "../../../lib-oatpp/include/domain/vo/BaseJsonVO.h"
#include "../../domain/vo/enter-orders/EnterOrdersVO.h"
#include "../../domain/query/enter-orders/EnterOrdersQuery.h"
//EnterOrders
//enter-orders
//ENTERORDERS
#include OATPP_CODEGEN_BEGIN(ApiController)

class EnterOrdersController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(EnterOrdersController);
public:
	//定义获取医嘱列表（条件+分页）接口描述
	ENDPOINT_INFO(queryEnterOrders)
	{
		//接口归属
		info->addTag(ZH_WORDS_GETTER("enter-orders.api-name"));
		//授权描述
		API_DEF_ADD_AUTH();
		//接口标题
		info->summary = ZH_WORDS_GETTER("enter-orders.summary1");
		//响应格式
		API_DEF_ADD_RSP_JSON_WRAPPER(QueryPageJsonVO);
		//分页查询参数
		API_DEF_ADD_PAGE_PARAMS();
		//租户id查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "tenant_id", ZH_WORDS_GETTER("enter-orders.tenant_id"), "", true);
		//患者id查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "patient_id", ZH_WORDS_GETTER("enter-orders.patient_id"), "", true);
		//患者id查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "encounter_id ", ZH_WORDS_GETTER("enter-orders.encounter_id "), "", true);
	}
	//定义获取医嘱列表（条件+分页）接口处理
	ENDPOINT("GET", "/enter-orders/get", queryEnterOrders, QUERIES(QueryParams, params), API_HANDLER_AUTH_PARAME)
	{
		//解析参数
		API_HANDLER_QUERY_PARAM(query, EnterOrdersQueryById, params);
		//返回前端结果
		return createDtoResponse(Status::CODE_200, executeQueryEnterOrders(query));
	}

	////定义新增医嘱接口描述
	//ENDPOINT_INFO(addEnterOrders)
	//{
	//	//接口归属
	//	info->addTag(ZH_WORDS_GETTER("enter-orders.api-name"));
	//	//授权描述
	//	API_DEF_ADD_AUTH();
	//	//接口标题
	//	info->summary = ZH_WORDS_GETTER("enter-orders.summary2");
	//	//响应格式
	//	API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
	//}
	////定义新增医嘱接口处理
	//ENDPOINT("POST", "/enter-orders/add", addEnterOrders, BODY_DTO(EnterOrdersAddDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME)
	//{
	//	//返回前端结果
	//	return createDtoResponse(Status::CODE_200, execAddEnterOrders(request, authObject->getPayload()));
	//}

	// 3.1 定义导入接口描述
	API_DEF_ENDPOINT_INFO_FILE_AUTH(ZH_WORDS_GETTER("enter-orders.summary2"), addEnterOrders, EnterOrdersImportDTO::Wrapper, StringJsonVO::Wrapper, ZH_WORDS_GETTER("enter-orders.api-name"));
	// 3.2 定义导入接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/enter-orders/add", addEnterOrders, REQUEST(std::shared_ptr<IncomingRequest>, request), execAddEnterOrders(request, authObject->getPayload()));




	//定义修改医嘱接口描述
	ENDPOINT_INFO(modifyEnterOrders)
	{
		//接口归属
		info->addTag(ZH_WORDS_GETTER("enter-orders.api-name"));
		//授权描述
		API_DEF_ADD_AUTH();
		//接口标题
		info->summary = ZH_WORDS_GETTER("enter-orders.summary3");
		//响应格式
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
	}
	//定义修改医嘱接口处理
	ENDPOINT("PUT", "/enter-orders/modify", modifyEnterOrders, BODY_DTO(EnterOrdersModifyDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME)
	{
		//返回前端结果
		return createDtoResponse(Status::CODE_200, execModifyEnterOrders(dto, authObject->getPayload()));
	}

	//定义删除医嘱接口描述
	ENDPOINT_INFO(removeEnterOrders)
	{
		//接口归属
		info->addTag(ZH_WORDS_GETTER("enter-orders.api-name"));
		//授权描述
		API_DEF_ADD_AUTH();
		//接口标题
		info->summary = ZH_WORDS_GETTER("enter-orders.summary4");
		//响应格式
		API_DEF_ADD_RSP_JSON_WRAPPER(ListJsonVO<String>);
	}
	//定义删除医嘱接口处理
	ENDPOINT("DELETE", "/enter-orders/delete", removeEnterOrders, BODY_DTO(List<String>, ids), API_HANDLER_AUTH_PARAME)
	{
		//返回前端结果
		return createDtoResponse(Status::CODE_200, execRemoveEnterOrders(ids));
	}





private:
	QueryPageJsonVO::Wrapper executeQueryEnterOrders(const EnterOrdersQueryById::Wrapper& query);
	StringJsonVO::Wrapper EnterOrdersController::execAddEnterOrders(std::shared_ptr<IncomingRequest> request, const PayloadDTO& payload);
	//StringJsonVO::Wrapper execAddEnterOrders(const EnterOrdersAddDTO::Wrapper& dto, const PayloadDTO& payload);
	StringJsonVO::Wrapper execModifyEnterOrders(const EnterOrdersModifyDTO::Wrapper& dto, const PayloadDTO& payload);
	ListJsonVO<String>::Wrapper execRemoveEnterOrders(const List<String>& ids);
};

#include OATPP_CODEGEN_END(ApiController)
#endif