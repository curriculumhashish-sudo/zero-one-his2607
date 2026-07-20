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
#ifndef _DIAGNOSISDETAILJSONVO_H_
#define _DIAGNOSISDETAILJSONVO_H_

#include "../../GlobalInclude.h"
#include "../vo/DiagnosisDetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 诊断详情单条响应VO
 * 对应页面：诊断详情查看、编辑表单回填弹窗
 * 接口：/diagnosis/detail
 */
class DiagnosisDetailJsonVO : public JsonVO<DiagnosisDetailDTO::Wrapper>
{
	DTO_INIT(DiagnosisDetailJsonVO, JsonVO<DiagnosisDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _DIAGNOSISDETAILJSONVO_H_#pragma once
