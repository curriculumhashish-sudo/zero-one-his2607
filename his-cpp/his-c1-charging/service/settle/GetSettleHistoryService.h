#pragma once

#ifndef _GET_SETTLE_HISTORY_SERVICE_H_
#define _GET_SETTLE_HISTORY_SERVICE_H_

#include "domain/query/settle/GetSettleHistoryQuery.h"
#include "domain/dto/settle/GetSettleHistoryDTO.h"

// 患者历史结算记录业务服务
class GetSettleHistoryService {
public:
	// 分页获取患者历史结算记录及支付明细
	GetSettleHistoryPageDTO::Wrapper getSettleHistory(
		const GetSettleHistoryQuery::Wrapper &query);
};

#endif
