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
	uint64_t count(const CostInfoQuery::Wrapper& query);
	std::list<CostInfoDO> selectWithPage(const CostInfoQuery::Wrapper& query);
	std::list<CostInfoDO> selectAll(const CostInfoQuery::Wrapper& query);
};
#endif // !_COSTINFO_DAO_H
