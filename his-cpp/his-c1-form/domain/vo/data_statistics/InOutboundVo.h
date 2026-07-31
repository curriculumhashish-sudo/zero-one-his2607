#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
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
#ifndef _INOUTBOUNDVO_H_
#define _INOUTBOUNDVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/data_statistics/InOutboundDto.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Single record JsonVO
class InOutboundVo : public JsonVO<InOutboundDto::Wrapper> {
	DTO_INIT(InOutboundVo, JsonVO<InOutboundDto::Wrapper>);
};

// Paged record JsonVO
class InOutboundPageJsonVo : public JsonVO<InOutboundPageDto::Wrapper> {
	DTO_INIT(InOutboundPageJsonVo, JsonVO<InOutboundPageDto::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INOUTBOUNDVO_H_
