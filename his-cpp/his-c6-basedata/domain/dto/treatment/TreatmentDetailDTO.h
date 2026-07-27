#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/19 00:00:00

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
#ifndef _TREATMENTDETAILDTO_H_
#define _TREATMENTDETAILDTO_H_

#include "../../GlobalInclude.h"
#include "TreatmentChildDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class TreatmentDetailDTO : public oatpp::DTO
{
	DTO_INIT(TreatmentDetailDTO, DTO);

	// 主键
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("treatment.field.id"), true);
	// 编码
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("treatment.field.code"), true);
	// 项目名
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("treatment.field.name"), true);
	// 状态
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("treatment.field.status"), true);
	// 分类
	API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("treatment.field.classification"), true);
	// 价格
	API_DTO_FIELD_REQUIRE(String, price, ZH_WORDS_GETTER("treatment.field.price"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif 
