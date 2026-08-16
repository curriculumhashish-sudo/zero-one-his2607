#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/24 00:00:00

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
#ifndef _NURSING_RECORD_QUERY_
#define _NURSING_RECORD_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class NursingRecordQuery : public oatpp::DTO
{
	DTO_INIT(NursingRecordQuery, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("nursing.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, patientId,
		ZH_WORDS_GETTER("nursing.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(String, recordedAtFrom,
		ZH_WORDS_GETTER("nursing.field.recordedAtFrom"), false);
	API_DTO_FIELD_REQUIRE(String, recordedAtTo,
		ZH_WORDS_GETTER("nursing.field.recordedAtTo"), false);
	API_DTO_FIELD_REQUIRE(Boolean, sortAsc,
		ZH_WORDS_GETTER("nursing.field.sortAsc"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_NURSING_RECORD_QUERY_
