#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xinpingqihe
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
#ifndef _COSTDETAILVO_H_
#define _COSTDETAILVO_H_

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include "domain/vo/JsonVO.h"
#include "domain/dto/PageDTO.h"
#include "domain/dto/chargedetails/CostDetailDTO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

// Single cost detail JSON VO
class CostDetailJsonVO : public JsonVO<CostDetailDTO::Wrapper> {
	DTO_INIT(CostDetailJsonVO, JsonVO<CostDetailDTO::Wrapper>);
};

// Paginated cost detail JSON VO
class CostDetailPageJsonVO : public JsonVO<PageDTO<CostDetailDTO::Wrapper>::Wrapper> {
	DTO_INIT(CostDetailPageJsonVO, JsonVO<PageDTO<CostDetailDTO::Wrapper>::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_COSTDETAILVO_H_
