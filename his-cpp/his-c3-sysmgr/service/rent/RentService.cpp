#include "stdafx.h"
#include "RentService.h"
#include "dao/rent/RentDAO.h"
#include "domain/dto/rent/RentDetailDTO.h"
#include "domain/dto/rent/RentNameDTO.h"
#include "domain/do/rent/RentDO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"


RentDetailPageDTO::Wrapper RentService::rentPage(const RentQuery::Wrapper& query)
{
	// 构建返回对象
	auto pages = RentDetailPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// 查询数据总条数
	RentDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}

	// 分页查询数据
	pages->total = count;
	pages->calcPages();
	list<RentDO> result = dao.selectWithPage(query);
	// 将DO转换成DTO
	for (RentDO& sub : result)
	{
		auto dto = RentDetailDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, name, Name, code, Code, description, Description, 
			is_active, IsActive, extraProperties, ExtraProperties, created_at, CreatedAt, updated_at, UpdatedAt, created_by, CreatedBy,
			updated_by, UpdatedBy, is_deleted, IsDeleted, deleted_at, DeletedAt, deleted_by, DeletedBy);
		pages->addData(dto);
	}
	return pages;
}

RentNamePageDTO::Wrapper RentService::findAllRent()
{
	// 构建返回对象
	auto pages = RentNamePageDTO::createShared();
	pages->pageIndex = 1;

	// 查询数据总条数
	RentDAO dao;
	uint64_t count = dao.countName();
	pages->pageSize = count;

	// 分页查询数据
	pages->total = count;
	pages->calcPages();
	list<RentDO> result = dao.selectAllName();
	// 将DO转换成DTO
	for (RentDO& sub : result)
	{
		auto dto = RentNameDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, name, Name, code, Code);
		pages->addData(dto);
	}
	return pages;
}
