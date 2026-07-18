#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/18 19:22:14

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
#ifndef _RENTDETAILDTO_H_
#define _RENTDETAILDTO_H_

#include "RentAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 租户详情DTO
 */
class RentDetailDTO : public RentAddDTO
{
	DTO_INIT(RentDetailDTO, RentAddDTO);
	// 租户ID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("rent.field.id"), true);
	// 创建人
	API_DTO_FIELD_REQUIRE(String, createBy, ZH_WORDS_GETTER("rent.field.createBy"), true);
	// 更新人
	API_DTO_FIELD_REQUIRE(String, updateBy, ZH_WORDS_GETTER("rent.field.updateBy"), true);
	// 其他字段自己补充完整
};

/**
 * 定义一个租户信息分页传输对象
 */
class RentDetailPageDTO : public PageDTO<RentDetailDTO::Wrapper>
{
	DTO_INIT(RentDetailPageDTO, PageDTO<RentDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_RENTDETAILDTO_H_