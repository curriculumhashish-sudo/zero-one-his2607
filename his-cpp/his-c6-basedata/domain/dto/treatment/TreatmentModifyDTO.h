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
#ifndef _TREATMENTMODIFYDTO_H_
#define _TREATMENTMODIFYDTO_H_

#include "TreatmentAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 修改诊疗DTO，在新增诊疗全部可编辑字段基础上增加主键
 */
class TreatmentModifyDTO : public TreatmentAddDTO
{
	DTO_INIT(TreatmentModifyDTO, TreatmentAddDTO);
	// 诊疗项目主键
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("treatment.field.id"), true);

public:
	// 修改参数最小校验：主键 + 新增必填字段
	std::string validate()
	{
		if (!id || id->empty())
			return "id invalidate.";
		return TreatmentAddDTO::validate();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENTMODIFYDTO_H_
