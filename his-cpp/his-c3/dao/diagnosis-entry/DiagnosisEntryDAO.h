#pragma once
#ifndef _DIAGNOSISENTRYDAO_H_
#define _DIAGNOSISENTRYDAO_H_
#include "BaseDAO.h"
#include "../../domain/do/diagnosis-entry/DiagnosisEntryDO.h"
#include "../../domain/query/diagnosis-entry/DiagnosisEntryQuery.h"
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY
/**
 * 示例表数据库操作实现
 */

class DiagnosisEntryDAO : public BaseDAO
{
private:
	inline std::string queryById(const DiagnosisEntryQueryById::Wrapper& query, SqlParams& params);
public:
	// 获取获取诊断使用历史
	std::list<DiagnosisEntryBriefDO> selectBrief(const DiagnosisEntryQueryNull::Wrapper& query);

	std::list<DiagnosisEntryDetailDO> selectDetail(const DiagnosisEntryQueryById::Wrapper& query);

	//// 统计数据条数
	//uint64_t count(const DiagnosisEntryQueryNull::Wrapper& query);
	// 统计数据条数
	uint64_t count(const DiagnosisEntryQueryById::Wrapper& query);
	//// 分页查询数据
	//std::list<MedicalRecordByTitleDO> selectWithPage(const MedicalRecordQueryByTitle::Wrapper& query);
	//// 通过姓名查询数据
	//std::list<MedicalRecordByTitleDO> selectByTitle(const string& title);
	//// 获取获取诊断使用历史
	//PtrDiagnosisEntryBriefDO selectNull(const DiagnosisEntryQueryNull::Wrapper& query);
};
#endif // !

//MedicalRecordBriefDAO
