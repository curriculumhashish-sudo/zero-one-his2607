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
#include "../../dto/data_statistics/OutpatientTrendDataDTO.h"
#include "../../dto/data_statistics/DiseaseDistributionDataDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Response VO for outpatient visit trend
class OutpatientTrendJsonVO : public JsonVO<OutpatientTrendDataDTO::Wrapper>
{
	DTO_INIT(OutpatientTrendJsonVO, JsonVO<OutpatientTrendDataDTO::Wrapper>);
};

// Response VO for disease type distribution
class DiseaseDistributionJsonVO : public JsonVO<DiseaseDistributionDataDTO::Wrapper>
{
	DTO_INIT(DiseaseDistributionJsonVO, JsonVO<DiseaseDistributionDataDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif
