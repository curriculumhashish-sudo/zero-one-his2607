#pragma once

#ifndef _GET_SETTLE_HISTORY_DAO_H_
#define _GET_SETTLE_HISTORY_DAO_H_

#include <cstdint>
#include <list>
#include <string>
#include <vector>
#include "BaseDAO.h"
#include "domain/do/settle/GetSettleHistoryDO.h"
#include "domain/query/settle/GetSettleHistoryQuery.h"

// 获取患者历史结算记录的数据访问对象
class GetSettleHistoryDAO : public BaseDAO {
private:
	// 构建历史结算记录的动态查询条件
	inline std::string queryConditionBuilder(const GetSettleHistoryQuery::Wrapper &query, SqlParams &params);

public:
	// 统计符合条件的历史结算记录数量
	uint64_t count(const GetSettleHistoryQuery::Wrapper &query);

	// 分页查询历史结算记录
	std::list<PtrGetSettleHistoryDO> selectWithPage(const GetSettleHistoryQuery::Wrapper &query);

	// 批量查询多条结算记录的支付明细，供业务层按结算主键组装
	std::list<PtrSettleHistoryPaymentDetailDO> selectPaymentDetails(const std::vector<std::string> &settlementIds);
};

#endif
