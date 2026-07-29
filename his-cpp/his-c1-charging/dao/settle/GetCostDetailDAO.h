#pragma once

#ifndef _GET_COST_DETAIL_DAO_H_
#define _GET_COST_DETAIL_DAO_H_

#include <list>
#include <string>

#include "BaseDAO.h"
#include "domain/do/settle/GetCostDetailDO.h"
#include "domain/query/settle/GetCostDetailQuery.h"

// 获取患者住院费用明细的数据访问对象
class GetCostDetailDAO : public BaseDAO {
private:
	// 构建收费明细的动态查询条件
	inline std::string queryConditionBuilder(const GetCostDetailQuery::Wrapper &query, SqlParams &params);

public:
	// 查询患者、住院记录及费用汇总信息
	PtrGetCostDetailDO selectSummary(const GetCostDetailQuery::Wrapper &query);

	// 查询患者住院费用明细
	std::list<PtrCostDetailItemDO> selectItems(const GetCostDetailQuery::Wrapper &query);
};

#endif
