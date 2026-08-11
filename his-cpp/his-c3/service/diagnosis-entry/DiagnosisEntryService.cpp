#include "stdafx.h"
#include "DiagnosisEntryService.h"
#include "../../dao/diagnosis-entry/DiagnosisEntryDAO.h"
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY
DiagnosisEntryBriefPageDTO::Wrapper DiagnosisEntryService::listAllBrief(const DiagnosisEntryQueryNull::Wrapper& query)
{
	// 构建返回对象
	auto result = DiagnosisEntryBriefPageDTO::createShared();
	DiagnosisEntryDAO dao;
	list<DiagnosisEntryBriefDO> ldo = dao.selectBrief(query);
	// 将DO转换成DTO
	for (DiagnosisEntryBriefDO& sub : ldo)
	{
		auto dto = DiagnosisEntryBriefDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, definition_name, Definition_name);
		result->addData(dto);
	}
	return result;
}


DiagnosisEntryDetailPageDTO::Wrapper DiagnosisEntryService::listAllDetail(const DiagnosisEntryQueryById::Wrapper& query)
{
		// 构建返回对象
	auto pages = DiagnosisEntryDetailPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// 查询数据总条数
	DiagnosisEntryDAO dao;
	uint64_t count = dao.count(query);

	// 分页查询数据
	pages->total = count;
	pages->calcPages();
	list<DiagnosisEntryDetailDO> result = dao.selectDetail(query);
	// 将DO转换成DTO
	for (DiagnosisEntryDetailDO& sub : result)
	{
		auto dto = DiagnosisEntryDetailDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, condition_id, Condition_id, definition_code, Definition_code, description, Description, kind_name, Kind_name, type_name, Type_name, main_flag, Main_flag);
		pages->addData(dto);
	}
	return pages;
}



//MedicalRecordDetailDTO::Wrapper MedicalRecordService::getById(const MedicalRecordQueryById::Wrapper& query)
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
//	list<MedicalRecordByTitleDO> result = dao.selectWithPage(query);
//	// 将DO转换成DTO
//	for (MedicalRecordByTitleDO& sub : result)
//	{
//		auto dto = MedicalRecordBriefDTO::createShared();
//		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, document_type_name, Document_type_name, title, Title, status, Status, updated_by, Updated_by);
//		pages->addData(dto);
//	}
//	return pages;
//}
