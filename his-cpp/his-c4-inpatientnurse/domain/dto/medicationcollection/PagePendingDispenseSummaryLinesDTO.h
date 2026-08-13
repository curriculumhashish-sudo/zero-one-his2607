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
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicationSummaryWorkbenchQueryDTO : public PageQuery
{
	DTO_INIT(MedicationSummaryWorkbenchQueryDTO, PageQuery);

	API_DTO_FIELD_REQUIRE(List<String>, encounterIds,
		ZH_WORDS_GETTER("medicationcollection.field.encounterIds"), true);
	API_DTO_FIELD_REQUIRE(String, medicineCategoryTab,
		ZH_WORDS_GETTER("medicationcollection.field.medicineCategoryTab"), true);
	API_DTO_FIELD_REQUIRE(String, therapyFilter,
		ZH_WORDS_GETTER("medicationcollection.field.therapyFilter"), true);
	API_DTO_FIELD_REQUIRE(String, queryDeadline,
		ZH_WORDS_GETTER("medicationcollection.field.queryDeadline"), false);
	API_DTO_FIELD_REQUIRE(String, keyword,
		ZH_WORDS_GETTER("medicationcollection.field.keyword"), false);
	API_DTO_FIELD_REQUIRE(String, startDate,
		ZH_WORDS_GETTER("medicationcollection.field.startDate"), false);
	API_DTO_FIELD_REQUIRE(String, endDate,
		ZH_WORDS_GETTER("medicationcollection.field.endDate"), false);
	API_DTO_FIELD_REQUIRE(String, sortBy,
		ZH_WORDS_GETTER("medicationcollection.field.sortBy"), false);
	API_DTO_FIELD_REQUIRE(String, sortDirection,
		ZH_WORDS_GETTER("medicationcollection.field.sortDirection"), false);
	API_DTO_FIELD_REQUIRE(String, where,
		ZH_WORDS_GETTER("medicationcollection.field.where"), false);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->pageIndex = 1;
		dto->pageSize = 10;
		dto->encounterIds = List<String>::createShared();
		dto->encounterIds->push_back("337907884548657152");
		dto->medicineCategoryTab = "west";
		dto->therapyFilter = "all";
		dto->queryDeadline = "2026-08-13 23:59:59";
		dto->sortBy = "plannedDispenseTime";
		dto->sortDirection = "DESC";
		return dto;
	}
};

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
		quantity = 0.0;
		chargeUnitPrice = 0.0;
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
		timePoints = List<NurseStationDispenseSummaryTimePointDTO::Wrapper>::createShared();
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
		orderRows = List<NurseStationDispenseSummaryOrderRowDTO::Wrapper>::createShared();
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
		this->total = static_cast<v_int64>(0);
		this->pages = static_cast<v_int64>(0);
		this->rows = List<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>::createShared();
		patientGroups = List<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PAGE_PENDING_DISPENSE_SUMMARY_LINES_DTO_
