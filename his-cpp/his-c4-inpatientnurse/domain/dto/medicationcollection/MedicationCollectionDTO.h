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
#ifndef _MEDICATION_COLLECTION_DTO_
#define _MEDICATION_COLLECTION_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicationDispenseTimePointDTO : public oatpp::DTO
{
	DTO_INIT(MedicationDispenseTimePointDTO, DTO);

	API_DTO_FIELD(String, dispenseId,
		ZH_WORDS_GETTER("medicationcollection.field.dispenseId"), false,
		"338131147144708096");
	API_DTO_FIELD(String, procedureId,
		ZH_WORDS_GETTER("medicationcollection.field.procedureId"), false,
		"338131147144708097");
	API_DTO_FIELD(String, plannedDispenseTime,
		ZH_WORDS_GETTER("medicationcollection.field.plannedDispenseTime"), false,
		"2026-07-21 08:00:00");
	API_DTO_FIELD(Float64, quantity,
		ZH_WORDS_GETTER("medicationcollection.field.quantity"), false, 1.0);
	API_DTO_FIELD(String, unitCode,
		ZH_WORDS_GETTER("medicationcollection.field.unitCode"), false, "片");
	API_DTO_FIELD(String, unitCodeText,
		ZH_WORDS_GETTER("medicationcollection.field.unitCodeText"), false, "片");
	API_DTO_FIELD(Float64, chargeUnitPrice,
		ZH_WORDS_GETTER("medicationcollection.field.chargeUnitPrice"), false, 2.50);
};

class MedicationSummaryOrderRowDTO : public oatpp::DTO
{
	DTO_INIT(MedicationSummaryOrderRowDTO, DTO);

	API_DTO_FIELD(String, medReqId,
		ZH_WORDS_GETTER("medicationcollection.field.medReqId"), false,
		"337990013773271040");
	API_DTO_FIELD(String, medicationId,
		ZH_WORDS_GETTER("medicationcollection.field.medicationId"), false,
		"334568157811871744");
	API_DTO_FIELD(String, medicationName,
		ZH_WORDS_GETTER("medicationcollection.field.medicationName"), false,
		"替米沙坦片");
	API_DTO_FIELD(String, specificationText,
		ZH_WORDS_GETTER("medicationcollection.field.specificationText"), false,
		"40mg*14片");
	API_DTO_FIELD(String, groupId,
		ZH_WORDS_GETTER("medicationcollection.field.groupId"), false,
		"GRP202607210001");
	API_DTO_FIELD(String, therapyEnum,
		ZH_WORDS_GETTER("medicationcollection.field.therapyEnum"), false,
		"TEMPORARY");
	API_DTO_FIELD(String, therapyEnumText,
		ZH_WORDS_GETTER("medicationcollection.field.therapyEnumText"), false,
		"临时");
	API_DTO_FIELD(String, dosageInstruction,
		ZH_WORDS_GETTER("medicationcollection.field.dosageInstruction"), false,
		"40mg");
	API_DTO_FIELD(String, methodCode,
		ZH_WORDS_GETTER("medicationcollection.field.methodCode"), false, "PO");
	API_DTO_FIELD(String, methodCodeText,
		ZH_WORDS_GETTER("medicationcollection.field.methodCodeText"), false,
		"口服");
	API_DTO_FIELD(String, frequencyCode,
		ZH_WORDS_GETTER("medicationcollection.field.frequencyCode"), false, "QD");
	API_DTO_FIELD(String, frequencyCodeText,
		ZH_WORDS_GETTER("medicationcollection.field.frequencyCodeText"), false,
		"每日一次");
	API_DTO_FIELD(String, locationId,
		ZH_WORDS_GETTER("medicationcollection.field.locationId"), false,
		"334526752078102528");
	API_DTO_FIELD(String, locationName,
		ZH_WORDS_GETTER("medicationcollection.field.locationName"), false,
		"住院药房");
	API_DTO_FIELD_REQUIRE(List<MedicationDispenseTimePointDTO::Wrapper>, timePoints,
		ZH_WORDS_GETTER("medicationcollection.field.timePoints"), true);

public:
	MedicationSummaryOrderRowDTO()
	{
		timePoints = List<MedicationDispenseTimePointDTO::Wrapper>::createShared();
		timePoints->push_back(MedicationDispenseTimePointDTO::createShared());
	}
};

class MedicationSummaryPatientGroupDTO : public oatpp::DTO
{
	DTO_INIT(MedicationSummaryPatientGroupDTO, DTO);

	API_DTO_FIELD(String, encounterId,
		ZH_WORDS_GETTER("medicationcollection.field.encounterId"), false,
		"337907884548657152");
	API_DTO_FIELD(String, patientId,
		ZH_WORDS_GETTER("medicationcollection.field.patientId"), false,
		"326940849401761792");
	API_DTO_FIELD(String, patientName,
		ZH_WORDS_GETTER("medicationcollection.field.patientName"), false,
		"小孙");
	API_DTO_FIELD(String, bedDisplay,
		ZH_WORDS_GETTER("medicationcollection.field.bedDisplay"), false,
		"1号床");
	API_DTO_FIELD_REQUIRE(List<MedicationSummaryOrderRowDTO::Wrapper>, orderRows,
		ZH_WORDS_GETTER("medicationcollection.field.orderRows"), true);

public:
	MedicationSummaryPatientGroupDTO()
	{
		orderRows = List<MedicationSummaryOrderRowDTO::Wrapper>::createShared();
		orderRows->push_back(MedicationSummaryOrderRowDTO::createShared());
	}
};

class SummaryOrderItemPageDTO : public oatpp::DTO
{
	DTO_INIT(SummaryOrderItemPageDTO, DTO);

	API_DTO_FIELD(UInt64, pageIndex,
		ZH_WORDS_GETTER("medicationcollection.field.pageIndex"), true, 1);
	API_DTO_FIELD(UInt64, pageSize,
		ZH_WORDS_GETTER("medicationcollection.field.pageSize"), true, 10);
	API_DTO_FIELD(Int64, total,
		ZH_WORDS_GETTER("medicationcollection.field.total"), true, 1);
	API_DTO_FIELD(Int64, pages,
		ZH_WORDS_GETTER("medicationcollection.field.pages"), true, 1);
	API_DTO_FIELD_REQUIRE(List<MedicationSummaryPatientGroupDTO::Wrapper>, patientGroups,
		ZH_WORDS_GETTER("medicationcollection.field.patientGroups"), true);

public:
	SummaryOrderItemPageDTO()
	{
		patientGroups = List<MedicationSummaryPatientGroupDTO::Wrapper>::createShared();
		patientGroups->push_back(MedicationSummaryPatientGroupDTO::createShared());
	}
};

class SummaryMedicationItemDTO : public oatpp::DTO
{
	DTO_INIT(SummaryMedicationItemDTO, DTO);

	API_DTO_FIELD(String, dispenseId,
		ZH_WORDS_GETTER("medicationcollection.field.dispenseId"), true,
		"338131147144708096");
	API_DTO_FIELD(String, encounterId,
		ZH_WORDS_GETTER("medicationcollection.field.encounterId"), true,
		"337907884548657152");
};

class SummaryMedicationDTO : public oatpp::DTO
{
	DTO_INIT(SummaryMedicationDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<SummaryMedicationItemDTO::Wrapper>, dispenseItems,
		ZH_WORDS_GETTER("medicationcollection.field.dispenseItems"), true);

public:
	SummaryMedicationDTO()
	{
		dispenseItems = List<SummaryMedicationItemDTO::Wrapper>::createShared();
		dispenseItems->push_back(SummaryMedicationItemDTO::createShared());
	}
};

class SummaryMedicationResultDTO : public oatpp::DTO
{
	DTO_INIT(SummaryMedicationResultDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, summaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNos"), true);
	API_DTO_FIELD(String, message,
		ZH_WORDS_GETTER("medicationcollection.field.message"), false,
		"Medication summary created.");

public:
	SummaryMedicationResultDTO()
	{
		summaryNos = List<String>::createShared();
		summaryNos->push_back("MS202607210001");
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICATION_COLLECTION_DTO_
