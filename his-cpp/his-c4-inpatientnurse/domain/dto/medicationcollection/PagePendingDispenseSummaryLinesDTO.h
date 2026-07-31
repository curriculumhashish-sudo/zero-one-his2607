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
#ifndef _PAGE_PENDING_DISPENSE_SUMMARY_LINES_DTO_
#define _PAGE_PENDING_DISPENSE_SUMMARY_LINES_DTO_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class NurseStationDispenseSummaryTimePointDTO : public oatpp::DTO
{
	DTO_INIT(NurseStationDispenseSummaryTimePointDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, dispenseId,
		ZH_WORDS_GETTER("medicationcollection.field.dispenseId"), false);
	API_DTO_FIELD_REQUIRE(String, procedureId,
		ZH_WORDS_GETTER("medicationcollection.field.procedureId"), false);
	API_DTO_FIELD_REQUIRE(String, plannedDispenseTime,
		ZH_WORDS_GETTER("medicationcollection.field.plannedDispenseTime"), false);
	API_DTO_FIELD_REQUIRE(Float64, quantity,
		ZH_WORDS_GETTER("medicationcollection.field.quantity"), false);
	API_DTO_FIELD_REQUIRE(String, unitCode,
		ZH_WORDS_GETTER("medicationcollection.field.unitCode"), false);
	API_DTO_FIELD_REQUIRE(String, unitCodeText,
		ZH_WORDS_GETTER("medicationcollection.field.unitCodeText"), false);
	API_DTO_FIELD_REQUIRE(Float64, chargeUnitPrice,
		ZH_WORDS_GETTER("medicationcollection.field.chargeUnitPrice"), false);

public:
	NurseStationDispenseSummaryTimePointDTO()
	{
		dispenseId = "338131147144708096";
		procedureId = "338131147144708097";
		plannedDispenseTime = "2026-07-21 08:00:00";
		quantity = 1.0;
		unitCode = ZH_WORDS_GETTER("business-text.sample.tablet-unit");
		unitCodeText = ZH_WORDS_GETTER("business-text.sample.tablet-unit");
		chargeUnitPrice = 2.50;
	}
};

class NurseStationDispenseSummaryOrderRowDTO : public oatpp::DTO
{
	DTO_INIT(NurseStationDispenseSummaryOrderRowDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, medReqId,
		ZH_WORDS_GETTER("medicationcollection.field.medReqId"), false);
	API_DTO_FIELD_REQUIRE(String, medicationId,
		ZH_WORDS_GETTER("medicationcollection.field.medicationId"), false);
	API_DTO_FIELD_REQUIRE(String, medicationName,
		ZH_WORDS_GETTER("medicationcollection.field.medicationName"), false);
	API_DTO_FIELD_REQUIRE(String, specificationText,
		ZH_WORDS_GETTER("medicationcollection.field.specificationText"), false);
	API_DTO_FIELD_REQUIRE(String, groupId,
		ZH_WORDS_GETTER("medicationcollection.field.groupId"), false);
	API_DTO_FIELD_REQUIRE(String, therapyEnum,
		ZH_WORDS_GETTER("medicationcollection.field.therapyEnum"), false);
	API_DTO_FIELD_REQUIRE(String, therapyEnumText,
		ZH_WORDS_GETTER("medicationcollection.field.therapyEnumText"), false);
	API_DTO_FIELD_REQUIRE(String, dosageInstruction,
		ZH_WORDS_GETTER("medicationcollection.field.dosageInstruction"), false);
	API_DTO_FIELD_REQUIRE(String, methodCode,
		ZH_WORDS_GETTER("medicationcollection.field.methodCode"), false);
	API_DTO_FIELD_REQUIRE(String, methodCodeText,
		ZH_WORDS_GETTER("medicationcollection.field.methodCodeText"), false);
	API_DTO_FIELD_REQUIRE(String, frequencyCode,
		ZH_WORDS_GETTER("medicationcollection.field.frequencyCode"), false);
	API_DTO_FIELD_REQUIRE(String, frequencyCodeText,
		ZH_WORDS_GETTER("medicationcollection.field.frequencyCodeText"), false);
	API_DTO_FIELD_REQUIRE(String, locationId,
		ZH_WORDS_GETTER("medicationcollection.field.locationId"), false);
	API_DTO_FIELD_REQUIRE(String, locationName,
		ZH_WORDS_GETTER("medicationcollection.field.locationName"), false);
	API_DTO_FIELD_REQUIRE(List<NurseStationDispenseSummaryTimePointDTO::Wrapper>, timePoints,
		ZH_WORDS_GETTER("medicationcollection.field.timePoints"), true);

public:
	NurseStationDispenseSummaryOrderRowDTO()
	{
		medReqId = "337990013773271040";
		medicationId = "334568157811871744";
		medicationName = ZH_WORDS_GETTER("business-text.sample.medication-name");
		specificationText = ZH_WORDS_GETTER("business-text.sample.medication-specification");
		groupId = "GRP202607210001";
		therapyEnum = "TEMPORARY";
		therapyEnumText = ZH_WORDS_GETTER("business-text.enum.therapy-temporary");
		dosageInstruction = "40mg";
		methodCode = "PO";
		methodCodeText = ZH_WORDS_GETTER("business-text.sample.medication-method");
		frequencyCode = "QD";
		frequencyCodeText = ZH_WORDS_GETTER("business-text.sample.medication-frequency");
		locationId = "334526752078102528";
		locationName = ZH_WORDS_GETTER("business-text.sample.inpatient-pharmacy");
		timePoints = List<NurseStationDispenseSummaryTimePointDTO::Wrapper>::createShared();
		timePoints->push_back(NurseStationDispenseSummaryTimePointDTO::createShared());
	}
};

class NurseStationDispenseSummaryPatientGroupDTO : public oatpp::DTO
{
	DTO_INIT(NurseStationDispenseSummaryPatientGroupDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("medicationcollection.field.encounterId"), false);
	API_DTO_FIELD_REQUIRE(String, patientId,
		ZH_WORDS_GETTER("medicationcollection.field.patientId"), false);
	API_DTO_FIELD_REQUIRE(String, patientName,
		ZH_WORDS_GETTER("medicationcollection.field.patientName"), false);
	API_DTO_FIELD_REQUIRE(String, bedDisplay,
		ZH_WORDS_GETTER("medicationcollection.field.bedDisplay"), false);
	API_DTO_FIELD_REQUIRE(List<NurseStationDispenseSummaryOrderRowDTO::Wrapper>, orderRows,
		ZH_WORDS_GETTER("medicationcollection.field.orderRows"), true);

public:
	NurseStationDispenseSummaryPatientGroupDTO()
	{
		encounterId = "337907884548657152";
		patientId = "326940849401761792";
		patientName = ZH_WORDS_GETTER("business-text.sample.patient-name");
		bedDisplay = ZH_WORDS_GETTER("business-text.sample.bed-name");
		orderRows = List<NurseStationDispenseSummaryOrderRowDTO::Wrapper>::createShared();
		orderRows->push_back(NurseStationDispenseSummaryOrderRowDTO::createShared());
	}
};

class PagePendingDispenseSummaryLinesDTO : public PageDTO<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>
{
	DTO_INIT(PagePendingDispenseSummaryLinesDTO,
		PageDTO<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>);

	API_DTO_FIELD_REQUIRE(List<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>, patientGroups,
		ZH_WORDS_GETTER("medicationcollection.field.patientGroups"), true);

public:
	PagePendingDispenseSummaryLinesDTO()
	{
		this->pageIndex = 1;
		this->pageSize = 10;
		this->total = 1;
		this->pages = 1;
		this->rows = List<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>::createShared();
		patientGroups = List<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>::createShared();
		patientGroups->push_back(NurseStationDispenseSummaryPatientGroupDTO::createShared());
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PAGE_PENDING_DISPENSE_SUMMARY_LINES_DTO_
