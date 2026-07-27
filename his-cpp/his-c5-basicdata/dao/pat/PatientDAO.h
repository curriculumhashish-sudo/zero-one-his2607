#pragma once
#ifndef _PATIENT_DAO_H_
#define _PATIENT_DAO_H_

#include "BaseDAO.h"
#include "../../domain/do/pat/PatientDO.h"
#include "domain/query/pat/PatSelectPageQuery.h"
#include "PatientMapper.h"

class PatientDAO : public BaseDAO
{
public:
	// 弹窗选择患者分页列表
	std::vector<PtrPatientDO> selectPatList(const PatSelectPageQuery::Wrapper& query, int offset, int limit);
	// 弹窗选择患者总条数统计
	int countPatList(const PatSelectPageQuery::Wrapper& query);
};
#endif
