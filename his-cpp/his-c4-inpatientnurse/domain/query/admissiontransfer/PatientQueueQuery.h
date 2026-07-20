#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Codex
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
#ifndef _PATIENT_QUEUE_QUERY_
#define _PATIENT_QUEUE_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientStatisticsQuery : public oatpp::DTO
{
	DTO_INIT(PatientStatisticsQuery, DTO);

	API_DTO_FIELD(String, keyword,
		ZH_WORDS_GETTER("admissiontransfer.field.keyword"), false, "小孙");
	API_DTO_FIELD(String, wardId,
		ZH_WORDS_GETTER("admissiontransfer.field.wardId"), false,
		"334526584536674304");
	API_DTO_FIELD(String, houseId,
		ZH_WORDS_GETTER("admissiontransfer.field.houseId"), false,
		"334526625720438784");
	API_DTO_FIELD(String, nurseStationExecutionDate,
		ZH_WORDS_GETTER("admissiontransfer.field.nurseStationExecutionDate"), false,
		"2026-07-21");
	API_DTO_FIELD(String, nurseStationTaskFilter,
		ZH_WORDS_GETTER("admissiontransfer.field.nurseStationTaskFilter"), false,
		"ALL");
	API_DTO_FIELD(String, nurseStationStatusFilter,
		ZH_WORDS_GETTER("admissiontransfer.field.nurseStationStatusFilter"), false,
		"INPATIENT");
};

class PatientQueueQuery : public PageQuery
{
	DTO_INIT(PatientQueueQuery, PageQuery);

	API_DTO_FIELD(String, keyword,
		ZH_WORDS_GETTER("admissiontransfer.field.keyword"), false, "小孙");
	API_DTO_FIELD(String, wardId,
		ZH_WORDS_GETTER("admissiontransfer.field.wardId"), false,
		"334526584536674304");
	API_DTO_FIELD(String, houseId,
		ZH_WORDS_GETTER("admissiontransfer.field.houseId"), false,
		"334526625720438784");
	API_DTO_FIELD(String, nurseStationExecutionDate,
		ZH_WORDS_GETTER("admissiontransfer.field.nurseStationExecutionDate"), false,
		"2026-07-21");
	API_DTO_FIELD(String, nurseStationTaskFilter,
		ZH_WORDS_GETTER("admissiontransfer.field.nurseStationTaskFilter"), false,
		"ALL");
	API_DTO_FIELD(String, nurseStationStatusFilter,
		ZH_WORDS_GETTER("admissiontransfer.field.nurseStationStatusFilter"), false,
		"INPATIENT");
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENT_QUEUE_QUERY_
