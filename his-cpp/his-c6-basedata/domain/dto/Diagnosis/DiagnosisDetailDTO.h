#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 11:00:34

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
#ifndef _DIAGNOSISPAGEVO_H_
#define _DIAGNOSISPAGEVO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief 诊断管理列表VO
 * 对应页面：诊断后台管理主列表
 * 说明：列表仅返回关键标识数据，完整详情通过详情接口获取
 */
class DiagnosisDetailDTO : public oatpp::DTO
{
	DTO_INIT(DiagnosisDetailDTO, DTO);

	// 唯一主键标识
	API_DTO_FIELD_REQUIRE(Int32, id, ZH_WORDS_GETTER("diagnosis.field.id"), true);
	// 诊断编码
	API_DTO_FIELD_REQUIRE(String, diagnosisCode, ZH_WORDS_GETTER("diagnosis.field.code"), true);
	// 诊断名称
	API_DTO_FIELD_REQUIRE(String, diagnosisName, ZH_WORDS_GETTER("diagnosis.field.name"), true);
	// 诊断目录分类
	API_DTO_FIELD_DEFAULT(String, catalogName, ZH_WORDS_GETTER("diagnosis.field.catalogName"));
	// 启用状态
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("diagnosis.field.status"), true);
	
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSISPAGEVO_H_#pragma once
