#ifndef _MONTH_VISIT_SERVICER_
#define _MONTH_VISIT_SERVICER_
#include"domain/query/data_statistics/MonthVisitQuery.h"
#include"domain/dto/data_statistics/MonthVisitDTO.h"
#include"../../dao/data_statistics/MonthVisitDAO.h"

class MonthVisitService
{
public:
	MonthVisitDTO::Wrapper findflow(const MonthVisitQuery::Wrapper& query)
	{
		MonthVisitDAO dao;
		auto dto = MonthVisitDTO::createShared();
		string result = dao.findMonthVisit(query);
			dto->totalVisit= result;
		return dto;
	}

};


#endif