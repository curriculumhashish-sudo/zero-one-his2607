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
	TreatmentPageDTO::Wrapper listAll(const TreatmentQuery::Wrapper& query);
	// 分页查询下拉/选择列表（id + name）
	TreatmentSelectPageDTO::Wrapper listSelect(const TreatmentQuery::Wrapper& query);
	// 通过ID查询诊疗详情（空实现，后续补 DAO）
	TreatmentDetailDTO::Wrapper getById(std::string id);
	// 新增诊疗（空实现，后续补 DAO）
	std::string saveData(const TreatmentAddDTO::Wrapper& dto);
	// 修改诊疗（空实现，后续补 DAO）
	bool updateData(const TreatmentModifyDTO::Wrapper& dto);
	// 通过ID删除数据
	bool removeData(const oatpp::List<oatpp::String>& ids);
};

#endif // !_TREATMENT_SERVICE_
