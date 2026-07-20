#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Codex
 @Date: 2026/07/20 00:00:00

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
#ifndef _WITHDRAWMEDICALADVICEDTO_H_
#define _WITHDRAWMEDICALADVICEDTO_H_

#include "../../GlobalInclude.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 批量撤回医嘱请求DTO
 */
class WithdrawMedicalAdviceDTO : public oatpp::DTO
{
	DTO_INIT(WithdrawMedicalAdviceDTO, DTO);
	// 医嘱ID列表
	API_DTO_FIELD_REQUIRE(List<String>, medicalAdviceIds,
		ZH_WORDS_GETTER("medical-advice.field.medical-advice-ids"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_WITHDRAWMEDICALADVICEDTO_H_
