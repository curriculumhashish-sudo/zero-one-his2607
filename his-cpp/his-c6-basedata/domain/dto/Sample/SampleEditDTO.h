#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/21 10:40:00

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
#ifndef _SAMPLE_EDIT_DTO_H_
#define _SAMPLE_EDIT_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 修改样本请求DTO
 */
class SampleEditDTO : public oatpp::DTO
{
	DTO_INIT(SampleEditDTO, DTO);

	// 主键ID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("sample.field.id"), true);
	// 样本业务编码
	API_DTO_FIELD_REQUIRE(String, sampleCode, ZH_WORDS_GETTER("sample.field.sampleCode"), true);
	// 样本名称
	API_DTO_FIELD_REQUIRE(String, sampleName, ZH_WORDS_GETTER("sample.field.sampleName"), true);
	// 拼音助记码
	API_DTO_FIELD_DEFAULT(String, inputCode, ZH_WORDS_GETTER("sample.field.inputCode"));
	// 五笔助记码
	API_DTO_FIELD_DEFAULT(String, wbCode, ZH_WORDS_GETTER("sample.field.wbCode"));
	// 启用状态
	API_DTO_FIELD_DEFAULT(Int32, status, ZH_WORDS_GETTER("sample.field.status"));
	// 备注
	API_DTO_FIELD_DEFAULT(String, remark, ZH_WORDS_GETTER("sample.field.remark"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_SAMPLE_EDIT_DTO_H_
