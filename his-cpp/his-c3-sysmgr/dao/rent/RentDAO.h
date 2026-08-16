#pragma once
#ifndef _RENT_DAO_
#define _RENT_DAO_
#include "BaseDAO.h"
#include "domain/do/rent/RentDO.h"
#include "domain/query/rent/RentQuery.h"

/**
 * 租户表数据库操作实现
 */
class RentDAO : public BaseDAO
{
private:
	inline std::string queryConditionBuilder(const RentQuery::Wrapper& query, SqlParams& params);
public:
	// 统计租户名称数据条数
	uint64_t countName();
	// 统计数据条数
	uint64_t count(const RentQuery::Wrapper& query);
	// 分页查询租户数据
	std::list<RentDO> selectWithPage(const RentQuery::Wrapper& query);
	// 查询所有租户名字
	std::list<RentDO> selectAllName();
};
#endif // !_Rent_DAO_