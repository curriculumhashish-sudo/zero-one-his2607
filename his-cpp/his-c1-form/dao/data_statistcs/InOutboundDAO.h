#pragma once
#ifndef _INOUTBOUNDDAO_H_
#define _INOUTBOUNDDAO_H_

#include "BaseDAO.h"
#include "../../domain/do/data_statistics/InOutboundDo.h"
#include "../../domain/query/data_statistics/InOutboundQuery.h"

class InOutboundDAO : public BaseDAO {
private:
	std::string queryConditionBuilder(const InOutboundQuery::Wrapper& query, SqlParams& params);
public:
	std::list<InOutboundDO> selectWithPage(const InOutboundQuery::Wrapper& query);
	InOutboundDO selectById(const std::string& id);
	uint64_t count(const InOutboundQuery::Wrapper& query);
};
#endif // !_INOUTBOUNDDAO_H_
