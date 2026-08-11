#include "stdafx.h"
#include "MedicalRecordService.h"
#include "../../dao/inpatient-record/MedicalRecordDAO.h"

MedicalRecordBriefPageDTO::Wrapper MedicalRecordService::listAll(const MedicalRecordQueryByTitle::Wrapper& query)
{
	// 构建返回对象
	auto pages = MedicalRecordBriefPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// 查询数据总条数
	MedicalRecordDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}

	// 分页查询数据
	pages->total = count;
	pages->calcPages();
	list<MedicalRecordByTitleDO> result = dao.selectWithPage(query);
	// 将DO转换成DTO
	for (MedicalRecordByTitleDO& sub : result)
	{
		auto dto = MedicalRecordBriefDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, document_type_name, Document_type_name, title, Title, status, Status, updated_by, Updated_by);
		pages->addData(dto);
	}
	return pages;
}


MedicalRecordDetailDTO::Wrapper MedicalRecordService::getById(const MedicalRecordQueryById::Wrapper& query)
{
	// 构建返回对象
	auto result = MedicalRecordDetailDTO::createShared();
	MedicalRecordDAO dao;
	ZO_STAR_DOMAIN_DO_TO_DTO_1(result, dao.selectById(query), id, Id, content, Content);

	return result;
}


class SimpleDateTimeFormat final
{
public:
	static std::string format(const std::string& fmt = "%Y-%m-%d %H:%M:%S");
	static std::string formatWithMilli(const std::string& fmt = "%Y-%m-%d %H:%M:%S", const std::string msDelim = " ");
};

std::string  MedicalRecordService::saveData(const MedicalRecordAddDTO::Wrapper& dto)
{
	// 组装DO数据
	MedicalRecordAddDO data;
	//ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Id, id, Patient_id, patient_id, Encounter_id, encounter_id,Document_type_code, document_type_code, Document_type_name, document_type_name, Title, title, Status, status, Content, content, Create_at, create_at, Update_at, update_at, Create_by, create_by, Update_by, update_by);
	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,Patient_id, patient_id, Encounter_id, encounter_id,Document_type_code, document_type_code, Title, title, Status, status, Content, content);
	// 生成ID
	UuidFacade uf;
	data.setId(uf.genUuid());
	// 设置创建人
	data.setCreated_by(dto->getPayload()->getUsername());
	// 设置创建时间
	data.setCreated_at(SimpleDateTimeFormat::format());
	// 执行数据添加
	MedicalRecordDAO dao;
	return dao.insert(data) == 1 ? data.getId() : "";
}


//std::string SampleService::saveData(const SampleAddDTO::Wrapper& dto)
//{
//	// 组装DO数据
//	SampleDO data;
//	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Name, name, Sex, sex, Age, age);
//	// 生成ID
//	UuidFacade uf;
//	data.setId(uf.genUuid());
//	// 设置创建人
//	data.setCreateBy(dto->getPayload()->getUsername());
//	// 设置创建时间
//	data.setCreateTime(SimpleDateTimeFormat::format());
//	// 执行数据添加
//	SampleDAO dao;
//	return dao.insert(data) == 1 ? data.getId() : "";
//}