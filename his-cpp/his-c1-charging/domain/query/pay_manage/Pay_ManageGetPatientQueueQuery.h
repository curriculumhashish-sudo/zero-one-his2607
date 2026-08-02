#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: byj
 @Date: 2026/08/02

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

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientQueueQuery : public PageQuery
{
	DTO_INIT(Pay_ManageGetPatientQueueQuery, PageQuery);

	// Inpatient number (optional)
	DTO_FIELD(String, hospitalized_number);
	DTO_FIELD_INFO(hospitalized_number) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_number");
	}

	// Patient name (optional)
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.name");
	}

	// Bed number (optional)
	DTO_FIELD(String, bed_number);
	DTO_FIELD_INFO(bed_number) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.bed_number");
	}

	// Admission date, format: yyyy-MM-dd (optional)
	DTO_FIELD(String, hospitalized_time);
	DTO_FIELD_INFO(hospitalized_time) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_time");
	}
};

#include OATPP_CODEGEN_END(DTO)
