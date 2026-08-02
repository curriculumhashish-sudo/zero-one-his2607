#pragma once

#ifndef _COSTINFO_SERVICE_
#define _COSTINFO_SERVICE_

#include "domain/vo/cost_info/costInfoVO.h"
#include "domain/query/cost_info/costInfoQuery.h"
#include "domain/dto/cost_info/costInfoDTO.h"

/**
 * 示例服务实现，演示基础的示例服务实现
 */
class CostInfoService
{
public:
	// 分页查询所有数据
	CostInfoDataDTO::Wrapper listAll(const CostInfoQuery::Wrapper& query);
	// 导出报表(返回全部数据)
	CostInfoDataDTO::Wrapper exportAll(const CostInfoQuery::Wrapper& query);
};

#endif // !_COSTINFO_SERVICE_


