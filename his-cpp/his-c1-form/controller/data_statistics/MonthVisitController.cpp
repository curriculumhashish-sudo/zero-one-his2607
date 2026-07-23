#include "stdafx.h"
#include "MonthVisitController.h"



MonthVisitPageJsonVO::Wrapper MonthVisitController::execQueryMonthVisit(
	const MonthVisitQuery::Wrapper& query,
	const PayloadDTO& payload
)
{
	
	auto jvo = MonthVisitPageJsonVO::createShared();
	return jvo;
}