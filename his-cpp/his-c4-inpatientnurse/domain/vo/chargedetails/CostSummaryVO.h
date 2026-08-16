#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
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
#ifndef _COSTSUMMARYVO_H_
#define _COSTSUMMARYVO_H_

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include "domain/vo/JsonVO.h"
#include "domain/dto/chargedetails/CostSummaryDTO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

// Single cost summary JSON VO
class CostSummaryJsonVO : public JsonVO<CostSummaryDTO::Wrapper> {
	DTO_INIT(CostSummaryJsonVO, JsonVO<CostSummaryDTO::Wrapper>);
};

// List cost summary JSON VO
class CostSummaryListJsonVO : public JsonVO<List<CostSummaryDTO::Wrapper>> {
	DTO_INIT(CostSummaryListJsonVO, JsonVO<List<CostSummaryDTO::Wrapper>>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_COSTSUMMARYVO_H_
