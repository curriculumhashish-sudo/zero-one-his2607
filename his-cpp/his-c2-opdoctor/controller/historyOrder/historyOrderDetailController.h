#pragma once
#ifndef _HISTORY_ORDER_DETAIL_CONTROLLER_H_
#define _HISTORY_ORDER_DETAIL_CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/historyOrder/historyOrderDetailQuery.h"
#include "domain/vo/historyOrder/historyOrderDetailVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("historyOrder.tags.tag")

class HistoryOrderDetailController : public oatpp::web::server::api::ApiController {
	API_ACCESS_DECLARE(HistoryOrderDetailController);
public:
	// 定义分页查询历史医嘱明细列表接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("historyOrder.detail.summary"), queryHistoryOrderDetail, HistoryOrderDetailQuery, HistoryOrderDetailPageJsonVO::Wrapper, API_TAG);
	// 定义分页查询历史医嘱明细列表接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/historyOrder/detail", queryHistoryOrderDetail, HistoryOrderDetailQuery, execQueryHistoryOrderDetail(query));

private:
	// 定义分页查询历史医嘱明细列表接口执行函数
	HistoryOrderDetailPageJsonVO::Wrapper execQueryHistoryOrderDetail(const HistoryOrderDetailQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif
