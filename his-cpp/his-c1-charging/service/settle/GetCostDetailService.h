#pragma once

#ifndef _GET_COST_DETAIL_SERVICE_H_
#define _GET_COST_DETAIL_SERVICE_H_

#include "domain/query/settle/GetCostDetailQuery.h"
#include "domain/dto/settle/GetCostDetailDTO.h"

// 患者住院费用明细业务服务
class GetCostDetailService {
public:
	// 获取患者本次住院的费用汇总及明细
	GetCostDetailDTO::Wrapper getCostDetail(
		const GetCostDetailQuery::Wrapper &query);
};

#endif
