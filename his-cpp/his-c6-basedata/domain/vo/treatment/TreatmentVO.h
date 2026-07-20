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
#ifndef _TREATMENTVO_H_
#define _TREATMENTVO_H_

#include "../../dto/treatment/TreatmentDetailDTO.h"
#include "../../dto/treatment/TreatmentPageDTO.h"
#include "../../dto/treatment/TreatmentSelectDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class TreatmentDetailJsonVO : public JsonVO<TreatmentDetailDTO::Wrapper>
{
	DTO_INIT(TreatmentDetailJsonVO, JsonVO<TreatmentDetailDTO::Wrapper>);
};

// 诊疗分页（详细）响应包装
class TreatmentPageJsonVO : public JsonVO<TreatmentPageDTO::Wrapper>
{
    DTO_INIT(TreatmentPageJsonVO, JsonVO<TreatmentPageDTO::Wrapper>);
};

// 诊疗选择列表分页响应包装（简化项）
class TreatmentSelectPageJsonVO : public JsonVO<TreatmentSelectPageDTO::Wrapper>
{
    DTO_INIT(TreatmentSelectPageJsonVO, JsonVO<TreatmentSelectPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENTVO_H_
