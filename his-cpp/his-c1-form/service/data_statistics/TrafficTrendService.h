#ifndef _FLOW_TREND_SERVICE_
#define _FLOW_TREND_SERVICE_
#include"domain/query/data_statistics/TrafficTrendQuery.h"
#include"domain/dto/data_statistics/TrafficTrendDTO.h"
#include"../../dao/data_statistics/TrafficTrendDAO.h"

class TrafficTrendService
{
public:
	TrafficTrendPageDTO::Wrapper findflow(const TrafficTrendQuery::Wrapper& query)
	{
		TrafficTrendDAO dao;
		auto pages = TrafficTrendPageDTO::createShared();
		
		uint64_t count = dao.count(query);
		if (count <= 0)
		{
			cout << 2 << endl;
			return pages;
		}
		pages->total = count;
		pages->calcPages();
		std::list<TrafficTrendDTO::Wrapper> result = dao.findFlowTreand(query);
		for(auto sub:result)
		{
			pages->addData(sub);
		}
		return pages;
	}

};


#endif