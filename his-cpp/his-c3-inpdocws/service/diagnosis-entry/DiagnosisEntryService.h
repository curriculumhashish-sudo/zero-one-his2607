#pragma once
#ifndef _DIAGNOSISENTRYSERVICE_H_
#define _DIAGNOSISENTRYSERVICE_H_
//#include "domain/vo/sample/SampleVO.h"
//#include "domain/query/sample/SampleQuery.h"
#include "../../domain/query/diagnosis-entry/DiagnosisEntryQuery.h"
//#include "domain/dto/sample/SampleDTO.h"
#include "../../domain/dto/diagnosis-entry/DiagnosisEntryDTO.h"
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY
/**
 * 示例服务实现，演示基础的示例服务实现
 */
class DiagnosisEntryService
{
public:
	// 查询所有数据
	DiagnosisEntryBriefPageDTO::Wrapper listAllBrief(const DiagnosisEntryQueryNull::Wrapper& query);
	// 通过ID分页查询所有数据
	DiagnosisEntryDetailPageDTO::Wrapper listAllDetail(const DiagnosisEntryQueryById::Wrapper& query);
};

#endif // !_SAMPLE_SERVICE_