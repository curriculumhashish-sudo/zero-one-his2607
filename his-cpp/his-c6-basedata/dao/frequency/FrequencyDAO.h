#pragma once

#ifndef _FREQUENCY_DAO_H_
#define _FREQUENCY_DAO_H_
#include"BaseDAO.h"
#include"../../domain/do/frequency/CusFrequencyDO.h"
#include"../../domain/query/frequency/FrequencyQuery.h"

class FrequencyDAO :public BaseDAO {
private:
	inline std::string queryConditionBuilder(const FrequencyQuery::Wrapper& query, SqlParams& parmas);
public:
	// 统计数据条数
	uint64_t count(const FrequencyQuery::Wrapper& query);
	// 分页查询数据
	std::list<CusFrequencyDO> selectWithPage(const FrequencyQuery::Wrapper& query);
	// 通过姓名查询数据
	std::list<CusFrequencyDO> selectByName(const string& name);
	// 通过ID查询数据
	PtrCusFrequencyDO selectById(std::string id);
};

#endif //!_FREQUENCY_DAO_H_