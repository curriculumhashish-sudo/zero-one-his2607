#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c4
 @Date: 2026/07/18 22:00:00

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
#ifndef _DISPENSE_VO_
#define _DISPENSE_VO_

#include "../../GlobalInclude.h"
#include "../../dto/medicationcollection/DispenseDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Page JsonVO of summary drug-collection bills (response of the list API).
 */
class DispenseSummaryPageJsonVO : public JsonVO<DispenseSummaryPageDTO::Wrapper> {
	DTO_INIT(DispenseSummaryPageJsonVO, JsonVO<DispenseSummaryPageDTO::Wrapper>);
};

/**
 * List JsonVO of dispense details (response of the detail API).
 */
class DispenseDetailListJsonVO : public ListJsonVO<DispenseDetailDTO::Wrapper> {
	DTO_INIT(DispenseDetailListJsonVO, ListJsonVO<DispenseDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DISPENSE_VO_
