#ifndef _MONTH_VISIT_SERVICER_
#define _MONTH_VISIT_SERVICER_
#include"domain/query/data_statistics/MonthVisitQuery.h"
#include"domain/dto/data_statistics/MonthVisitDTO.h"
#include"../../dao/data_statistics/MonthVisitDAO.h"

class MonthVisitService
{
public:
	MonthVisitPageDTO::Wrapper findflow(const MonthVisitQuery::Wrapper& query)
	{
		MonthVisitDAO dao;
		auto pages = MonthVisitPageDTO::createShared();
		uint64_t count = dao.count(query);
		if (count <= 0)
		{
			cout << 2 << endl;
			return pages;
		}
		pages->total = count;
		pages->calcPages();
		auto result = dao.findMonthVisit(query);
		for(auto dto:result)
		{
			pages->addData(dto);
		}
		return pages;
	}

};


#endif