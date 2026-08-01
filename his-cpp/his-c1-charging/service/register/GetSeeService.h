#ifndef __GET_SEE_SERVICE__
#define __GET_SEE_SERVICE__
#include "domain/vo/register/GetSeeVO.h"
#include "domain/query/register/GetSeeQuery.h"
#include "domain/dto/register/GetSeeDTO.h"
#include "domain/do/register/GetSeeDO.h"
#include"../../dao/register/GetSeeDAO.h"

class GetSeeService
{
public:
	GetSeePageDTO::Wrapper selectSee(const GetSeeQuery::Wrapper &query)
	{
		auto pages = GetSeePageDTO::createShared();
		pages->pageIndex = query->pageIndex;
		pages->pageSize = query->pageSize;

		GetSeeDAO dao;
		uint64_t count = dao.count(query);
		if (count <= 0)
		{
			cout << 2 << endl;
			return pages;
		}
		pages->total = count;
		pages->calcPages();
		list<GetSeeDO> result = dao.selectSeeWithPage(query);
		// ½«DO×ª»»³ÉDTO
		for (GetSeeDO& sub : result)
		{
			auto dto = GetSeeDTO::createShared();
			ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, number, Bus_no, name, Name, sex, Gender_enum, phone, Phone);
			ZO_STAR_DOMAIN_DO_TO_DTO(dto,sub, organization,Org_name, doctor, Doctor, cost_type, Cost_type, addr, Addr, register_type, Register_type, cost,Cost,time,Time);
			pages->addData(dto);
		}
		cout << 1 << endl;
		return pages;
	}
};



#endif