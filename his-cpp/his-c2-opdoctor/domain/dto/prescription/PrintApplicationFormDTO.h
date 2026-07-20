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
#ifndef _PRINTAPPLICATIONFORMDTO_H_
#define _PRINTAPPLICATIONFORMDTO_H_

#include "../../GlobalInclude.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 打印申请单请求DTO
 */
class PrintApplicationFormDTO : public oatpp::DTO
{
	DTO_INIT(PrintApplicationFormDTO, DTO);
	// 服务申请单ID
	API_DTO_FIELD_REQUIRE(String, serviceRequestId,
		ZH_WORDS_GETTER("medical-advice.field.service-request-id"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRINTAPPLICATIONFORMDTO_H_
