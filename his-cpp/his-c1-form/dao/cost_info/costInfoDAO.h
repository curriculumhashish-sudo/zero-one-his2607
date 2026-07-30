#pragma once

#ifndef _COSTINFO_DAO_H
#define _COSTINFO_DAO_H
#include <sstream>
#include "BaseDAO.h"
#include "domain/do/cost_info/costInfoDO.h"
#include "domain/dto/cost_info/costInfoDTO.h"
#include "domain/query/cost_info/costInfoQuery.h"


class CostInfoDAO : public BaseDAO
{
private:
	inline std::string queryConditionBuilder(const CostInfoQuery::Wrapper& query, SqlParams& params);
public:
	// 返回有效数据条数
	uint64_t count(const CostInfoQuery::Wrapper& query);
	// 分页条件查询
	std::list<CostInfoDO> selectWithPage(const CostInfoQuery::Wrapper& query);
	// 查询全部数据
	std::list<CostInfoDO> selectAll(const CostInfoQuery::Wrapper& query);
};
#endif // !_COSTINFO_DAO_H
