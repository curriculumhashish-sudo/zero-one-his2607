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
#ifndef _MEDICAL_ADVICE_VO_
#define _MEDICAL_ADVICE_VO_

#include "../../GlobalInclude.h"
#include "../../dto/prescription/PrintApplicationFormDTO.h"
#include "../../dto/prescription/SignMedicalAdviceDTO.h"
#include "../../dto/prescription/WithdrawMedicalAdviceDTO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 批量签发医嘱响应对象，返回已签发的医嘱ID列表
 */
class SignMedicalAdviceJsonVO : public JsonVO<List<String>>
{
    DTO_INIT(SignMedicalAdviceJsonVO, JsonVO<List<String>>);
};

/**
 * 批量撤回医嘱响应对象，返回已撤回的医嘱ID列表
 */
class WithdrawMedicalAdviceJsonVO : public JsonVO<List<String>>
{
    DTO_INIT(WithdrawMedicalAdviceJsonVO, JsonVO<List<String>>);
};

/**
 * 打印申请单响应对象，返回生成的申请单地址或内容
 */
class PrintApplicationFormJsonVO : public JsonVO<String>
{
    DTO_INIT(PrintApplicationFormJsonVO, JsonVO<String>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ADVICE_VO_
