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

	// X-axis label text (e.g. "Monday", "07-01")
	DTO_FIELD(String, xLabel);
	DTO_FIELD_INFO(xLabel) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.x_label");
	}

	// total patient count for this period
	DTO_FIELD(Int32, patientCount);
	DTO_FIELD_INFO(patientCount) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.patient_count");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif
