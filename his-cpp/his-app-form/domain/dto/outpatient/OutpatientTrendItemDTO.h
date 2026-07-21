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
#ifndef _OUTPATIENT_TREND_ITEM_DTO_
#define _OUTPATIENT_TREND_ITEM_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OutpatientTrendItemDTO : public oatpp::DTO
{
	DTO_INIT(OutpatientTrendItemDTO, DTO);

	// stat date
	DTO_FIELD(String, statDate);
	DTO_FIELD_INFO(statDate) {
		info->description = ZH_WORDS_GETTER("W.form.outpatient_stat.field.stat_date");
	}

	// daily visit count
	DTO_FIELD(UInt64, visitCount);
	DTO_FIELD_INFO(visitCount) {
		info->description = ZH_WORDS_GETTER("W.form.outpatient_stat.field.visit_count");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif
