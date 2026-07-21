#pragma once
#ifndef _HISTORY_ORDER_LIST_CONTROLLER_H_
#define _HISTORY_ORDER_LIST_CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/historyOrder/historyOrderListQuery.h"
#include "domain/vo/historyOrder/historyOrderListVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("historyOrder.tags.tag")

class HistoryOrderListController : public oatpp::web::server::api::ApiController {
	API_ACCESS_DECLARE(HistoryOrderListController);
public:
	// 定义分页查询历史就诊记录列表接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("historyOrder.list.summary"), queryHistoryOrderList, HistoryOrderListQuery, HistoryOrderListPageJsonVO::Wrapper, API_TAG);
	// 定义分页查询历史就诊记录列表接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/historyOrder/list", queryHistoryOrderList, HistoryOrderListQuery, execQueryHistoryOrderList(query));

private:
	// 定义分页查询历史就诊记录列表接口执行函数
	HistoryOrderListPageJsonVO::Wrapper execQueryHistoryOrderList(const HistoryOrderListQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif
