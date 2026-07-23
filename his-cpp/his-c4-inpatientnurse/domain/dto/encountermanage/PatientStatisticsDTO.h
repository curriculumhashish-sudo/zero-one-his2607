#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/21 00:00:00

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
#ifndef _PATIENT_STATISTICS_DTO_
#define _PATIENT_STATISTICS_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Project extension: the Java UI derives these values from pagePatientQueue rows.
class PatientStatisticsDTO : public oatpp::DTO
{
	DTO_INIT(PatientStatisticsDTO, DTO);

	API_DTO_FIELD_REQUIRE(Int64, totalCount,
		ZH_WORDS_GETTER("encountermanage.field.totalCount"), true);
	API_DTO_FIELD_REQUIRE(Int64, pendingAuditCount,
		ZH_WORDS_GETTER("encountermanage.field.pendingAuditCount"), true);
	API_DTO_FIELD_REQUIRE(Int64, pendingExecuteCount,
		ZH_WORDS_GETTER("encountermanage.field.pendingExecuteCount"), true);

public:
	PatientStatisticsDTO()
	{
		totalCount = 1;
		pendingAuditCount = v_int64(0);
		pendingExecuteCount = v_int64(0);
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENT_STATISTICS_DTO_
