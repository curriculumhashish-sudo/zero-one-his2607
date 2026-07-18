#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/18 19:16:52

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
#ifndef _RENTADDDTO_H_
#define _RENTADDDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 租户添加DTO类
 */
class RentAddDTO : public oatpp::DTO
{
	DTO_INIT(RentAddDTO, DTO);
	// 租户编码
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("rent.field.code"), true);
	// 租户名称
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("rent.field.name"), true);
	// 租户状态
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("rent.field.status"), true);
	// 租户描述
	API_DTO_FIELD_REQUIRE(String, desc, ZH_WORDS_GETTER("rent.field.desc"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_RENTADDDTO_H_