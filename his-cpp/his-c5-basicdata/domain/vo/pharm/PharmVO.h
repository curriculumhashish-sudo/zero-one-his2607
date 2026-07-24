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

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * PharmJsonVO - Single pharmacy/warehouse record response.
 *
 * Used when querying a single record by ID or returning
 * the result of an add operation.
 */
class PharmJsonVO : public JsonVO<PharmDTO::Wrapper> {
    DTO_INIT(PharmJsonVO, JsonVO<PharmDTO::Wrapper>);
};

/**
 * PharmPageJsonVO - Paginated pharmacy/warehouse list response.
 *
 * Used for the main table display with pagination support.
 */
class PharmPageJsonVO : public JsonVO<PharmPageDTO::Wrapper> {
    DTO_INIT(PharmPageJsonVO, JsonVO<PharmPageDTO::Wrapper>);
};

/**
 * PharmSelectListJsonVO - Dropdown selection list response.
 *
 * Returns a lightweight list with only id and name
 * for frontend dropdown/select components.
 */
class PharmSelectListJsonVO : public JsonVO<List<PharmSelectDTO::Wrapper>> {
    DTO_INIT(PharmSelectListJsonVO, JsonVO<List<PharmSelectDTO::Wrapper>>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARM_VO_
