#pragma once
#ifndef _DIAGNOSIS_DAO_H_
#define _DIAGNOSIS_DAO_H_

#include "BaseDAO.h"
#include "domain/do/diagnosis/DiagnosisDO.h"

class DiagnosisDAO :public BaseDAO
{
public:
	// 通过ID查询数据
	DiagnosisDO selectById(std::string id);
};


#endif // !_DIAGNOSIS_DAO_H_
