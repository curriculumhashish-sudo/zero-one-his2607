#ifndef _GET_TODAY_SERVICE_
#define _GET_TODAY_SERVICE_
#include "domain/vo/register/GetTodayAppointVO.h"
#include "domain/query/register/GetTodayAppointQuery.h"
#include "domain/dto/register/GetTodayAppointDTO.h"
#include "domain/do/register/GetTodayAppointDO.h"
#include"../../dao/register/GetTodayAppointDAO.h"

class GetTodayAppointService
{
public:
	GetTodayAppointPageDTO::Wrapper selectToday(const GetTodayAppointQuery::Wrapper& query)
	{
		auto pages = GetTodayAppointPageDTO::createShared();
		pages->pageIndex = query->pageIndex;
		pages->pageSize = query->pageSize;

		GetTodayAppointDAO dao;
		uint64_t count = dao.count(query);
		if (count <= 0)
		{
			return pages;
		}

		pages->total = count;
		pages->calcPages();
		list<GetTodayAppointDO> result = dao.selectTodayWithPage(query);

		for (GetTodayAppointDO& sub : result)
		{
			auto dto = GetTodayAppointPageDTO::createShared();
			ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, status, Status, capacity, Capacity, booked_count, Booked_count,
				register_type, Register_type, organization, Organization,start_time, Start_time, end_time, End_time);
			pages->addData(dto);
		}
		return pages;
	}
};
#endif
