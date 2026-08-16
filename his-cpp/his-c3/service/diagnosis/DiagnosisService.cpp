#include "stdafx.h"
#include "DiagnosisService.h"
#include "../../dao/diagnosis/DiagnosisDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "domain/do/diagnosis/DiagnosisDO.h"
#include "domain/do/diagnosis/ClnConditionDO.h"
#include "dao/diagnosis/ClnConditionDAO.h"



DiagnosisDTO::Wrapper DiagnosisService::getById(std::string id)
{
	DiagnosisDAO dao;
	auto res = dao.selectById(id);

	if (res.getId().empty())  // 或者根据实际判断
		return nullptr;

	auto dto = DiagnosisDTO::createShared();
	ZO_STAR_DOMAIN_DO_TO_DTO(dto, res,
		id, Id,
		encounterId, EncounterId,
		conditionId, ConditionId,
		primaryFlag, PrimaryFlag,
		rank, Rank,
		description, Description,
		diagTypeCode, DiagTypeCode,
		conditionCode, ConditionCode,
		tcmSyndromeGroup, TcmSyndromeGroup,
		tcmFlag, TcmFlag);
	return dto;
}
DiagnosisAddResultDTO::Wrapper DiagnosisService::saveData(const DiagnosisAddDTO::Wrapper& dto)
{
	// 生成ID
	UuidFacade uf;
	const std::string conditionId = uf.genUuid();

	// 组装ClnConditionDO数据
	ClnConditionDO cln_data;
	ZO_STAR_DOMAIN_DTO_TO_DO(cln_data, dto, TenantId, tenantId, EncounterId, encounterId, PatientId, patientId, Description, description, TcmFlag, tcmFlag);

	cln_data.setId(conditionId);
	// 设置创建人
	cln_data.setCreatedBy(dto->getPayload()->getUsername());
	// 设置创建时间
	cln_data.setCreatedAt(SimpleDateTimeFormat::format());
	// 组装DiagnosisDO数据
	DiagnosisDO diagnosis_data;
	ZO_STAR_DOMAIN_DTO_TO_DO(diagnosis_data, dto, TenantId, tenantId, EncounterId, encounterId, PrimaryFlag, primaryFlag,Description, description, TcmFlag, tcmFlag,
		ConditionCode, conditionCode,Rank,rank);
	diagnosis_data.setId(uf.genUuid());
	//设置conditionId
	diagnosis_data.setConditionId(conditionId);
	// 设置创建人
	diagnosis_data.setCreatedBy(dto->getPayload()->getUsername());
	// 设置创建时间
	diagnosis_data.setCreatedAt(SimpleDateTimeFormat::format());

	


	// 执行数据添加
	DiagnosisDAO diagnosis_dao;
	ClnConditionDAO cln_dao;
	
	
	// 创建SqlSession并开启事务
	SqlSession sqlSession;
	sqlSession.beginTransaction();

	// 设置两个DAO使用同一个SqlSession
	diagnosis_dao.setSqlSession(&sqlSession);
	cln_dao.setSqlSession(&sqlSession);

	diagnosis_dao.setAutoRelease(false);
	cln_dao.setAutoRelease(false);

	try {
		// 1. 先插入cln_condition
		int clnResult = cln_dao.insert(cln_data);
		if (clnResult != 1) {
			sqlSession.rollbackTransaction();
			return nullptr;
		}

		// 2. 再插入adm_encounter_diagnosis
		int diagnosisResult = diagnosis_dao.insert(diagnosis_data);
		if (diagnosisResult != 1) {
			sqlSession.rollbackTransaction();
			return nullptr;
		}

		// 提交事务
		sqlSession.commitTransaction();

		// 创建返回结果DTO
		auto result = DiagnosisAddResultDTO::createShared();
		result->diagnosisId = diagnosis_data.getId();
		result->conditionId = conditionId;
		return result;

	}
	catch (const std::exception& e) {
		// 发生异常，回滚事务
		sqlSession.rollbackTransaction();
		std::cerr << "Save diagnosis failed: " << e.what() << std::endl;
		return nullptr;
	}
	catch (...) {
		// 捕获其他异常
		sqlSession.rollbackTransaction();
		std::cerr << "Save diagnosis failed with unknown exception" << std::endl;
		return nullptr;
	}
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

	// 创建SqlSession并开启事务
	SqlSession sqlSession;
	sqlSession.beginTransaction();

	diagnosis_dao.setSqlSession(&sqlSession);
	diagnosis_dao.setAutoRelease(false);
	cln_dao.setSqlSession(&sqlSession);
	cln_dao.setAutoRelease(false);

	try {
		// 1. 删除 adm_encounter_diagnosis
		int diagnosisDeleted = diagnosis_dao.deleteById<DiagnosisDO>(diagnosisId);
		if (diagnosisDeleted != 1) {
			sqlSession.rollbackTransaction();
			return false;
		}

		// 2. 删除 cln_condition
		int clnDeleted = cln_dao.deleteById<ClnConditionDO>(conditionId);
		if (clnDeleted != 1) {
			sqlSession.rollbackTransaction();
			return false;
		}

		sqlSession.commitTransaction();
		return true;

	}
	catch (const std::exception& e) {
		sqlSession.rollbackTransaction();
		std::cerr << "Remove diagnosis failed: " << e.what() << std::endl;
		return false;
	}
	catch (...) {
		sqlSession.rollbackTransaction();
		std::cerr << "Remove diagnosis failed with unknown exception" << std::endl;
		return false;
	}
}
