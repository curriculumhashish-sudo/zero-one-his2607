#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Lfff
 @Date: 2026/07/23 10:00:00

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
#ifndef _PHARM_VO_
#define _PHARM_VO_

#include "../../GlobalInclude.h"
#include "../../dto/pharm/PharmDTO.h"
#include "../../dto/pharm/PharmPageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * PharmSelectListJsonVO - Dropdown selection list response (c5 mengsuibizhi).
 *
 * Note: PharmJsonVO and PharmPageJsonVO are defined in
 * PharmDetailVO.h and PharmPageVO.h (c5-pharm) respectively.
 */
class PharmSelectListJsonVO : public JsonVO<List<PharmSelectDTO::Wrapper>> {
    DTO_INIT(PharmSelectListJsonVO, JsonVO<List<PharmSelectDTO::Wrapper>>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARM_VO_
