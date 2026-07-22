#include "stdafx.h"
#include "RefundGetPatientDAO.h"
#include "RefundGetPatientMapper.h"



inline std::string RefundGetPatientDAO::queryConditionBuilder(const RefundGetPatientQuery::Wrapper& query, SqlParams& params)
{
	return std::string();
}

uint64_t RefundGetPatientDAO::count(const RefundGetPatientQuery::Wrapper& query)
{
	return 0;
}

std::list<RefundGetPatientDO> RefundGetPatientDAO::selectWithPage(const RefundGetPatientQuery::Wrapper& query)
{
	return std::list<RefundGetPatientDO>();
}
