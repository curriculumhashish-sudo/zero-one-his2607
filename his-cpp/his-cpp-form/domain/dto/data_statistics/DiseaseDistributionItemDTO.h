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
#ifndef _DISEASE_DISTRIBUTION_ITEM_DTO_
#define _DISEASE_DISTRIBUTION_ITEM_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DiseaseDistributionItemDTO : public oatpp::DTO
{
	DTO_INIT(DiseaseDistributionItemDTO, DTO);

	// disease type name (e.g. "hypertension", "respiratory infection")
	DTO_FIELD(String, diseaseName);
	DTO_FIELD_INFO(diseaseName) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.disease_name");
	}

	// total patient count for this disease type
	DTO_FIELD(Int32, patientCount);
	DTO_FIELD_INFO(patientCount) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.patient_count");
	}

	// percentage of total (redundant for ECharts, but provided as fallback)
	DTO_FIELD(Float64, percentage);
	DTO_FIELD_INFO(percentage) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.percentage");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif
