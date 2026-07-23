#include "stdafx.h"
#include "TrendController.h"

TrafficTrendPageJsonVO::Wrapper FlowTrendController::execQueryFlowTrend(
	const TrafficTrendQuery::Wrapper& query,
	const PayloadDTO& payload
)
{

	
	auto jvo = TrafficTrendPageJsonVO::createShared();

	return jvo;
}