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

	// disease name
	DTO_FIELD(String, diseaseName);
	DTO_FIELD_INFO(diseaseName) {
		info->description = ZH_WORDS_GETTER("W.form.outpatient_stat.field.disease_name");
	}

	// disease code
	DTO_FIELD(String, diseaseCode);
	DTO_FIELD_INFO(diseaseCode) {
		info->description = ZH_WORDS_GETTER("W.form.outpatient_stat.field.disease_code");
	}

	// case count
	DTO_FIELD(UInt64, caseCount);
	DTO_FIELD_INFO(caseCount) {
		info->description = ZH_WORDS_GETTER("W.form.outpatient_stat.field.case_count");
	}

	// percentage
	DTO_FIELD(Float64, percentage);
	DTO_FIELD_INFO(percentage) {
		info->description = ZH_WORDS_GETTER("W.form.outpatient_stat.field.percentage");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif
