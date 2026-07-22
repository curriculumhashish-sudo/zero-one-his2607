#pragma once
#ifndef _MEDICALRECORDVOSERVICE_H
#define _MEDICALRECORDVOSERVICE_H
//#include "domain/vo/sample/SampleVO.h"
//#include "domain/query/sample/SampleQuery.h"
#include "../../domain/query/inpatient-record/MedicalRecordQuery.h"
//#include "domain/dto/sample/SampleDTO.h"
#include "../../domain/dto/inpatient-record/MedicalRecordDTO.h"

/**
 * 示例服务实现，演示基础的示例服务实现
 */
class MedicalRecordService
{
public:
	// 分页查询所有数据
	MedicalRecordBriefPageDTO::Wrapper listAll(const MedicalRecordQueryByTitle::Wrapper& query);
	//// 通过ID查询单个数据
	//SampleDTO::Wrapper getById(std::string id);
	//// 保存数据
	//std::string saveData(const SampleAddDTO::Wrapper& dto);
	//// 批量保存数据
	//bool saveDataBatch(const oatpp::List<SampleAddDTO::Wrapper>& datas);
	//// 修改数据
	//bool updateData(const SampleDTO::Wrapper& dto);
	//// 通过ID删除数据
	//bool removeData(const oatpp::List<oatpp::String>& ids);
};

#endif // !_SAMPLE_SERVICE_