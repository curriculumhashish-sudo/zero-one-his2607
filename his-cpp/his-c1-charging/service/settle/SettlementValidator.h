#pragma once

#ifndef _SETTLEMENT_VALIDATOR_H_
#define _SETTLEMENT_VALIDATOR_H_

#include <string>

#include "domain/query/settle/GetCostDetailQuery.h"
#include "domain/query/settle/GetSettleHistoryQuery.h"
#include "domain/dto/settle/SettleDTO.h"

// 住院结算接口参数校验器
class SettlementValidator {
public:
	// 校验患者费用明细查询参数
	static std::string validateCostDetailQuery(
		const GetCostDetailQuery::Wrapper &query);

	// 校验患者历史结算查询参数
	static std::string validateSettleHistoryQuery(
		const GetSettleHistoryQuery::Wrapper &query);

	// 校验住院结算请求参数
	static std::string validateSettleRequest(
		const SettleRequestDTO::Wrapper &dto);
};

#endif
