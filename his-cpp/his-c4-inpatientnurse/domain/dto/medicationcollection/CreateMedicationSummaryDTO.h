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
#ifndef _CREATE_MEDICATION_SUMMARY_DTO_
#define _CREATE_MEDICATION_SUMMARY_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CreateMedicationSummaryDispenseItemDTO : public oatpp::DTO
{
	DTO_INIT(CreateMedicationSummaryDispenseItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, dispenseId,
		ZH_WORDS_GETTER("medicationcollection.field.dispenseId"), true);
	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("medicationcollection.field.encounterId"), true);

public:
	CreateMedicationSummaryDispenseItemDTO()
	{
		dispenseId = "338131147144708096";
		encounterId = "337907884548657152";
	}
};

class CreateMedicationSummaryDTO : public oatpp::DTO
{
	DTO_INIT(CreateMedicationSummaryDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<CreateMedicationSummaryDispenseItemDTO::Wrapper>, dispenseItems,
		ZH_WORDS_GETTER("medicationcollection.field.dispenseItems"), true);

public:
	CreateMedicationSummaryDTO()
	{
		dispenseItems = List<CreateMedicationSummaryDispenseItemDTO::Wrapper>::createShared();
		dispenseItems->push_back(CreateMedicationSummaryDispenseItemDTO::createShared());
	}
};

class CreateMedicationSummaryResponseDTO : public oatpp::DTO
{
	DTO_INIT(CreateMedicationSummaryResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, summaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNos"), true);
	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("medicationcollection.field.message"), false);

public:
	CreateMedicationSummaryResponseDTO()
	{
		summaryNos = List<String>::createShared();
		summaryNos->push_back("MS202607210001");
		message = "Medication summary created.";
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_CREATE_MEDICATION_SUMMARY_DTO_
