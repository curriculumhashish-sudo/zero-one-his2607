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
#ifndef _OUTPATIENT_TREND_DATA_DTO_
#define _OUTPATIENT_TREND_DATA_DTO_

#include "../../GlobalInclude.h"
#include "OutpatientTrendItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OutpatientTrendDataDTO : public oatpp::DTO
{
	DTO_INIT(OutpatientTrendDataDTO, DTO);

	// trend data point list
	DTO_FIELD(List<Object<OutpatientTrendItemDTO>>, trendList);
	DTO_FIELD_INFO(trendList) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.trend_list");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif
