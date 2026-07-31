#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/29
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
#ifndef _DIAGNOSE_SELECT_DTO_H_
#define _DIAGNOSE_SELECT_DTO_H_
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)


class DSelectDTO : public oatpp::DTO
{
	DTO_INIT(DSelectDTO, DTO);
	// 主键ID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("diagnose.field.id"), true);
	// 诊断编码
	API_DTO_FIELD_REQUIRE(String, diagnoseCode, ZH_WORDS_GETTER("diagnose.field.code"), true);
	// 诊断名称
	API_DTO_FIELD_REQUIRE(String, diagnoseName, ZH_WORDS_GETTER("diagnose.field.name"), true);
	// dictType:0,1,2
	API_DTO_FIELD_REQUIRE(String, dictType, ZH_WORDS_GETTER("diagnose.field.dictType"), true);
	//// 是否主诊:0,1
	//API_DTO_FIELD_DEFAULT(Int32, isMain, ZH_WORDS_GETTER("diagnose.field.isMain"));
	//// 是否存为常用:0,1
	//API_DTO_FIELD_DEFAULT(Int32, saveToCommon, ZH_WORDS_GETTER("diagnose.field.saveToCommon"));
	//

};


class DiagnoseSelectDTO : public PageDTO<DSelectDTO::Wrapper>
{
	DTO_INIT(DiagnoseSelectDTO, PageDTO<DSelectDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DIAGNOSE_SELECT_DTO_H_