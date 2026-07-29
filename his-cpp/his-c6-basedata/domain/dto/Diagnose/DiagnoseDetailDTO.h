#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/22 11:00:34

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
#ifndef _DIAGNOSE_DETAIL_DTO_H_
#define _DIAGNOSE_DETAIL_DTO_H_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class DiagnoseDetailDTO : public oatpp::DTO
{
	DTO_INIT(DiagnoseDetailDTO, DTO);
	// 主键ID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("diagnose.field.id"), true);
	// 诊断编码
	API_DTO_FIELD_REQUIRE(String, diagnoseCode, ZH_WORDS_GETTER("diagnose.field.code"), true);
	// 诊断名称
	API_DTO_FIELD_REQUIRE(String, diagnoseName, ZH_WORDS_GETTER("diagnose.field.name"), true);
	// 诊断类型
	API_DTO_FIELD_REQUIRE(String, diagnoseType, ZH_WORDS_GETTER("diagnose.field.type"), true);
	// 医保编码
	API_DTO_FIELD_DEFAULT(String, medicalInsuranceCode, ZH_WORDS_GETTER("diagnose.field.medicalInsuranceCode"));
	// 状态:0,1
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("diagnose.field.status"), true);
	// 说明
	API_DTO_FIELD_DEFAULT(String, remark, ZH_WORDS_GETTER("diagnose.field.remark"));
public:
	DiagnoseDetailDTO()
	{
		
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_diagnose_DETAIL_DTO_H_