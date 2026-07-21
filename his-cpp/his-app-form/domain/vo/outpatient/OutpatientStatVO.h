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
#ifndef _OUTPATIENT_STAT_VO_
#define _OUTPATIENT_STAT_VO_

#include "../../GlobalInclude.h"
#include "../../dto/outpatient/OutpatientTrendItemDTO.h"
#include "../../dto/outpatient/DiseaseDistributionItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Paginated wrapper for trend items
class OutpatientTrendPageDTO : public PageDTO<OutpatientTrendItemDTO::Wrapper>
{
	DTO_INIT(OutpatientTrendPageDTO, PageDTO<OutpatientTrendItemDTO::Wrapper>);
};

// Paginated wrapper for disease distribution items
class DiseaseDistributionPageDTO : public PageDTO<DiseaseDistributionItemDTO::Wrapper>
{
	DTO_INIT(DiseaseDistributionPageDTO, PageDTO<DiseaseDistributionItemDTO::Wrapper>);
};

// Response VO for weekly/monthly outpatient visit trend
class OutpatientTrendJsonVO : public JsonVO<OutpatientTrendPageDTO::Wrapper>
{
	DTO_INIT(OutpatientTrendJsonVO, JsonVO<OutpatientTrendPageDTO::Wrapper>);
};

// Response VO for disease type distribution
class DiseaseDistributionJsonVO : public JsonVO<DiseaseDistributionPageDTO::Wrapper>
{
	DTO_INIT(DiseaseDistributionJsonVO, JsonVO<DiseaseDistributionPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif
