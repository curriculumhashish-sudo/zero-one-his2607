#pragma once

#ifndef _TREATMENT_SERVICE_
#define _TREATMENT_SERVICE_
#include "domain/vo/treatment/TreatmentVO.h"
#include "domain/query/treatment/TreatmentQuery.h"
#include "domain/dto/treatment/TreatmentAddDTO.h"
#include "domain/dto/treatment/TreatmentChildDTO.h"
#include "domain/dto/treatment/TreatmentDetailDTO.h"
#include "domain/dto/treatment/TreatmentModifyDTO.h"
#include "domain/dto/treatment/TreatmentPageDTO.h"
#include "domain/dto/treatment/TreatmentRemoveDTO.h"
#include "domain/dto/treatment/TreatmentSelectDTO.h"
#include "domain/dto/treatment/TreatmentStatusDTO.h"
#include "domain/dto/treatment/TreatmentDTO.h"
/**
 * 示例服务实现，演示基础的示例服务实现
 */
class TreatmentService
{
public:
	// 分页查询所有数据
	TreatmentDTO::Wrapper listAll(const TreatmentQuery::Wrapper& query);
	// 通过ID查询单个数据
	// SampleDTO::Wrapper getById(std::string id);
	// 保存数据
	//std::string saveData(const SampleAddDTO::Wrapper& dto);
	// 批量保存数据
	//bool saveDataBatch(const oatpp::List<SampleAddDTO::Wrapper>& datas);
	// 修改数据
	//bool updateData(const SampleDTO::Wrapper& dto);
	// 通过ID删除数据
	bool removeData(const oatpp::List<oatpp::String>& ids);
};

#endif // !_TREATMENT_SERVICE_

