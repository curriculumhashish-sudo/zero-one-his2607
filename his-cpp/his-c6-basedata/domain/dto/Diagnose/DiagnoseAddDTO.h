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
#ifndef _DIAGNOSE_RECORD_ADD_DTO_H_
#define _DIAGNOSE_RECORD_ADD_DTO_H_
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)


class DiagnoseRecordAddDTO : public oatpp::DTO
{
	DTO_INIT(DiagnoseRecordAddDTO, DTO);
	// Õï¶Ï×ÖµäID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("diagnose.field.id"), true);
	// Õï¶Ï±àÂë
	API_DTO_FIELD_REQUIRE(String, diagnoseCode, ZH_WORDS_GETTER("diagnose.field.code"), true);
	// Õï¶ÏÃû³Æ
	API_DTO_FIELD_REQUIRE(String, diagnoseName, ZH_WORDS_GETTER("diagnose.field.name"), true);
	// dictType:0,1,2
	API_DTO_FIELD_REQUIRE(String, dictType, ZH_WORDS_GETTER("diagnose.field.dictType"), true);
	// ÊÇ·ñÖ÷Õï:0,1
	API_DTO_FIELD_DEFAULT(Int32, isMain, ZH_WORDS_GETTER("diagnose.field.isMain"));
	// ÊÇ·ñ´æÎª³£ÓÃ:0,1
	API_DTO_FIELD_DEFAULT(Int32, saveToCommon, ZH_WORDS_GETTER("diagnose.field.saveToCommon"));


CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);//ÇëÇóÍ·tokenÖ¸Õë

public:
	std::string validate()
	{

		if (!id || id->empty())
		{
			return "The  id cannot be empty";
		}
		if (!diagnoseCode || diagnoseCode->empty())
		{
			return "The diagnose code cannot be empty";
		}
		if (!diagnoseName || diagnoseName->empty())
		{
			return "The diagnose name cannot be empty";
		}
		if (!dictType)
		{
			return "The dictType  cannot be empty";
		}


		if (*dictType != 0 && *dictType != 1 && *dictType != 2)
		{
			return "The dictType only allows 0, 1 or 2";
		}
		if (isMain && *isMain != 0 && *isMain != 1)
		{
			return "The main diagnose flag only allows 0 or 1";
		}
		if (saveToCommon && *saveToCommon != 0 && *saveToCommon != 1)
		{
			return "The save to common flag only allows 0 or 1";
		}

		return "";
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DIAGNOSE_RECORD_ADD_DTO_H_