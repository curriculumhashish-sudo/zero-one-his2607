#ifndef _GET_TODAY_DAO_
#define _GET_TODAY_DAO_
#include "BaseDAO.h"
#include "../../domain/do/register/GetTodayAppointDO.h"
#include "../../domain/query/register/GetTodayAppointQuery.h"

class GetTodayAppointDAO : public BaseDAO
{
private:
	inline std::string queryConditionBuilder(const GetTodayAppointQuery::Wrapper& query, SqlParams& params);

public:

	uint64_t count(const GetTodayAppointQuery::Wrapper& query);

	std::list<GetTodayAppointDO>selectTodayWithPage(const GetTodayAppointQuery::Wrapper& query);

};

#endif