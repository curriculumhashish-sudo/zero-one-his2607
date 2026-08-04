#ifndef _FLOW_TREND_SERVICE_
#define _FLOW_TREND_SERVICE_
#include"domain/query/data_statistics/TrafficTrendQuery.h"
#include"domain/dto/data_statistics/TrafficTrendDTO.h"
#include"../../dao/data_statistics/TrafficTrendDAO.h"

class TrafficTrendService
{
public:
	TrafficTrendDTO::Wrapper findflow(const TrafficTrendQuery::Wrapper& query)
	{
		TrafficTrendDAO dao;
		auto dto= TrafficTrendDTO::createShared();
		string result = dao.findFlowTreand(query);
		dto->visited=result;
		return dto;
	}

};


#endif