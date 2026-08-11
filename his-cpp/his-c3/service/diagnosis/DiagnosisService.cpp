#include "stdafx.h"
#include "DiagnosisService.h"
#include "../../dao/diagnosis/DiagnosisDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "domain/do/diagnosis/DiagnosisDO.h"
#include "domain/do/diagnosis/ClnConditionDO.h"
#include "dao/diagnosis/ClnConditionDAO.h"



std::string DiagnosisService::saveData(const DiagnosisAddDTO::Wrapper& dto)
{
	// 组装DiagnosisDO数据
	DiagnosisDO diagnosis_data;
	ZO_STAR_DOMAIN_DTO_TO_DO(diagnosis_data, dto, TenantId, tenantId, EncounterId, encounterId, PrimaryFlag, primaryFlag,Description, description, TcmFlag, tcmFlag,
		ConditionCode, conditionCode,Rank,rank);
	
	// 生成ID
	UuidFacade diagnosis_uf;

	
	diagnosis_data.setId(diagnosis_uf.genUuid());
	// 设置创建人
	diagnosis_data.setCreatedBy(dto->getPayload()->getUsername());
	// 设置创建时间
	diagnosis_data.setCreatedAt(SimpleDateTimeFormat::format());

	// 组装ClnConditionDO数据
	ClnConditionDO cln_data;
	ZO_STAR_DOMAIN_DTO_TO_DO(cln_data, dto, TenantId, tenantId, EncounterId, encounterId, PatientId, patientId, Description, description, TcmFlag, tcmFlag);
	UuidFacade cln_uf;
	cln_data.setId(cln_uf.genUuid());
	diagnosis_data.setConditionId(cln_uf.genUuid());
	// 设置创建人
	cln_data.setCreatedBy(dto->getPayload()->getUsername());
	// 设置创建时间
	cln_data.setCreatedAt(SimpleDateTimeFormat::format());


	// 执行数据添加
	DiagnosisDAO diagnosis_dao;
	ClnConditionDAO cln_dao;
	
	// 执行插入操作
	bool diagnosis_ok = (diagnosis_dao.insert(diagnosis_data) == 1);
	bool cln_ok = (cln_dao.insert(cln_data) == 1);

	// 组装返回结果
	std::string result = "diagnosis_id: ";
	result += diagnosis_ok ? diagnosis_data.getId() : "insert failed";
	result += ", cln_id: ";
	result += cln_ok ? cln_data.getId() : "insert failed";

	return result;


}
bool DiagnosisService::updateData(const DiagnosisModifyDTO::Wrapper& dto)
{
	
	// 组装DO数据
	DiagnosisDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Id,id, DiagTypeCode, diagTypeCode);
	// 设置修改人
	data.setUpdatedBy(dto->getPayload()->getUsername());
	// 设置修改时间
	data.setUpdatedAt(SimpleDateTimeFormat::format());
	// 执行数据修改
	DiagnosisDAO dao;
	return dao.update(data) == 1;

}

bool DiagnosisService::removeData(const DiagnosisRemoveDTO::Wrapper& dto)
{
	std::string diagnosisId = dto->id;
	std::string conditionId = dto->conditionId;

	if (diagnosisId.empty() || conditionId.empty()) { 
		return false;
	}

	DiagnosisDAO diagnosis_dao;
	ClnConditionDAO cln_dao;

	// 先删除条件，再删除诊断
	bool clnDeleted = (cln_dao.deleteById<ClnConditionDO>(conditionId) == 1);
	bool diagnosisDeleted = (diagnosis_dao.deleteById< DiagnosisDO>(diagnosisId) == 1);

	return clnDeleted && diagnosisDeleted;
}