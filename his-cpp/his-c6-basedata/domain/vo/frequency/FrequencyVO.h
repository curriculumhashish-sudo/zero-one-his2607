#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
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
#ifndef _FREQUENCYVO_H_
#define _FREQUENCYVO_H_

#include "../../dto/frequency/FrequencyDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 频次分页列表响应包装（code + message + data）
 */
class FrequencyPageJsonVO : public JsonVO<FrequencyPageDTO::Wrapper>
{
	DTO_INIT(FrequencyPageJsonVO, JsonVO<FrequencyPageDTO::Wrapper>);
};

// 频次详情响应包装（code + message + data）
class FrequencyDetailJsonVO : public JsonVO<FrequencyDTO::Wrapper>
{
    DTO_INIT(FrequencyDetailJsonVO, JsonVO<FrequencyDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_FREQUENCYVO_H_
