#include "stdafx.h"
#include "MedicalRecordService.h"

//MedicalRecordBriefPageDTO::Wrapper MedicalRecordService::listAll(const MedicalRecordQueryByTitle::Wrapper& query)
//{
//	// 构建返回对象
//	auto pages = MedicalRecordBriefPageDTO::createShared();
//	pages->pageIndex = query->pageIndex;
//	pages->pageSize = query->pageSize;
//
//	// 查询数据总条数
//	MedicalRecordDAO dao;
//	uint64_t count = dao.count(query);
//	if (count <= 0)
//	{
//		return pages;
//	}
//
//	// 分页查询数据
//	pages->total = count;
//	pages->calcPages();
//	list<SampleDO> result = dao.selectWithPage(query);
//	// 将DO转换成DTO
//	for (SampleDO& sub : result)
//	{
//		auto dto = SampleDTO::createShared();
//		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, name, Name, sex, Sex, age, Age);
//		pages->addData(dto);
//	}
//	return pages;
//}