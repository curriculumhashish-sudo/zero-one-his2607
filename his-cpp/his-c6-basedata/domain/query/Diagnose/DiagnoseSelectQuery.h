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
#ifndef _diagnose_SELECT_DTO_H_
#define _diagnose_SELECT_DTO_H_
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)
/**
 * 诊断下拉选择数据DTO
 * 对应页面：西医/中医诊断选择弹窗
 */
class diagnoseSelectDTO : public oatpp::DTO
{
	DTO_INIT(diagnoseSelectDTO, DTO);
	// 唯一主键标识
	API_DTO_FIELD_REQUIRE(Int32, id, ZH_WORDS_GETTER("diagnose.field.id"), true);
	// 诊断编码
	API_DTO_FIELD_REQUIRE(String, diagnoseCode, ZH_WORDS_GETTER("diagnose.field.code"), true);
	// 诊断名称
	API_DTO_FIELD_REQUIRE(String, diagnoseName, ZH_WORDS_GETTER("diagnose.field.name"), true);
	// 诊断分类 1-西医 2-中医
	API_DTO_FIELD_REQUIRE(Int32, diagnoseType, ZH_WORDS_GETTER("diagnose.field.type"), true);
};
#include OATPP_CODEGEN_END(DTO)
#endif // !_diagnose_SELECT_DTO_H_