#pragma once
#ifndef _INVENTORYMATERIALDAO_H_
#define _INVENTORYMATERIALDAO_H_

#include "BaseDAO.h"
#include "../../domain/do/data_statistics/InventoryMaterialDO.h"
#include "../../domain/query/data_statistics/InventoryMaterialQuery.h"
#include "../../domain/query/data_statistics/InOutboundQuery.h"
#include "../../domain/do/data_statistics/InventoryDO.h"

class InventoryMaterialDAO : public BaseDAO {
private:
	inline std::string queryConditionBuilder(const InventoryMaterialQuery::Wrapper& query, SqlParams& params);
public:
	// 分页查询数据
	std::list<InventoryDO> selectWithPage(const InventoryMaterialQuery::Wrapper& query);
	// 通过物品id查询
	InventoryDO selectById(const std::string& bus_no);
	// 统计数据条数
	uint64_t count(const InventoryMaterialQuery::Wrapper& query);
};

#endif // !_INVENTORYMATERIALDAO_H_