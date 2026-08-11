#pragma once
#ifndef _DIAGNOSIS_SERVICE_H_
#define _DIAGNOSIS_SERVICE_H_


#include "domain/dto/diagnosis/DiagnosisAddDTO.h"
#include "domain/dto/diagnosis/DiagnosisModifyDTO .h"
#include "domain/dto/diagnosis/DiagnosisRemoveDTO .h"
#include "domain/vo/BaseJsonVO.h"


class DiagnosisService
{
public:
	// 保存数据
	std::string saveData(const DiagnosisAddDTO::Wrapper& dto);
	// 修改数据
	bool updateData(const DiagnosisModifyDTO::Wrapper& dto);
	// 通过ID删除数据
	bool removeData(const DiagnosisRemoveDTO::Wrapper& dto);
};


#endif // !_DIAGNOSIS_SERVICE_H_
