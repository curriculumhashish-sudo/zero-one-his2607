#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 11:00:34

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
#ifndef _DIAGNOSISPAGEJSONVO_H_
#define _DIAGNOSISPAGEJSONVO_H_

#include "../../GlobalInclude.h"
#include "../vo/DiagnosisPageVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 诊断管理分页响应VO
 * 对应页面：诊断后台管理主列表（支持批量启停）
 * 接口：/diagnosis/page
 */
class DiagnosisPageJsonVO : public JsonVO<DiagnosisPageVO::Wrapper>
{
	DTO_INIT(DiagnosisPageJsonVO, JsonVO<DiagnosisPageVO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _DIAGNOSISPAGEJSONVO_H_#pragma once
