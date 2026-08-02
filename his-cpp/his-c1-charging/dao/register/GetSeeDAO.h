#ifndef _GET_SEE_DAO_
#define _GET_SEE_DAO_
#include "BaseDAO.h"
#include "../../domain/do/register/GetSeeDO.h"
#include "../../domain/query/register/GetSeeQuery.h"

class GetSeeDAO : public BaseDAO
{
private:
	inline std::string queryConditionBuilder(const GetSeeQuery::Wrapper& query, SqlParams& params);

public:
	uint64_t count(const GetSeeQuery::Wrapper& query);

	std::list<GetSeeDO>selectSeeWithPage(const GetSeeQuery::Wrapper& query);

};
#endif