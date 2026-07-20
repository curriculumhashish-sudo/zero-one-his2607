#include "stdafx.h"
#include "GetPatientDAO.h"
#include "GetPatientMapper.h"



inline std::string GetPatientDAO::queryConditionBuilder(const GetPatientQuery::Wrapper& query, SqlParams& params)
{
	return std::string();
}

uint64_t GetPatientDAO::count(const GetPatientQuery::Wrapper& query)
{
	return 0;
}

std::list<GetPatientDO> GetPatientDAO::selectWithPage(const GetPatientQuery::Wrapper& query)
{
	return std::list<GetPatientDO>();
}
