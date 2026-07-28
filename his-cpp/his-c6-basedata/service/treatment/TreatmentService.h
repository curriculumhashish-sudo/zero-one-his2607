#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 11:08:56

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _TREATMENT_SERVICE_
#define _TREATMENT_SERVICE_
#include "domain/vo/treatment/TreatmentVO.h"
#include "domain/query/treatment/TreatmentQuery.h"
#include "domain/dto/treatment/TreatmentAddDTO.h"
#include "domain/dto/treatment/TreatmentChildDTO.h"
#include "domain/dto/treatment/TreatmentDetailDTO.h"
#include "domain/dto/treatment/TreatmentModifyDTO.h"
#include "domain/dto/treatment/TreatmentPageDTO.h"
#include "domain/dto/treatment/TreatmentSelectDTO.h"
#include "domain/dto/treatment/TreatmentStatusDTO.h"

/**
 * 诊疗服务实现（对齐 SampleService）
 */
class TreatmentService
{
public:
	// 通过ID查询诊疗详情（含基础信息/医保信息/子项）
	TreatmentDetailDTO::Wrapper getById(std::string id);
	// 新增诊疗（主表 + 子项）
	std::string saveData(const TreatmentAddDTO::Wrapper& dto);
	// 批量新增诊疗
	bool saveDataBatch(const oatpp::List<TreatmentAddDTO::Wrapper>& datas);
	// 修改诊疗（主表 + 子项先软删再插入）
	bool updateData(const TreatmentModifyDTO::Wrapper& dto);


	// 分页查询列表数据
	TreatmentPageDTO::Wrapper listAll(const TreatmentQuery::Wrapper& query);
	// 分页查询下拉/选择列表（id + name）
	TreatmentSelectPageDTO::Wrapper listSelect(const TreatmentQuery::Wrapper& query);
	// 通过ID批量删除数据
	bool removeData(const oatpp::List<oatpp::String>& ids);
};

#endif // !_TREATMENT_SERVICE_