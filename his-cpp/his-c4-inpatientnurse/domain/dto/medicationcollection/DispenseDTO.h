#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c4
 @Date: 2026/07/18 22:00:00

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
#ifndef _DISPENSE_DTO_
#define _DISPENSE_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Summary drug-collection bill row DTO (one row of the summary list).
 * Field names follow table med_medication_dispense.
 */
class DispenseSummaryDTO : public oatpp::DTO
{
	DTO_INIT(DispenseSummaryDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, summaryNo,
		ZH_WORDS_GETTER("dispense.field.summary-no"), true);
	API_DTO_FIELD_REQUIRE(String, statusEnum,
		ZH_WORDS_GETTER("dispense.field.status-enum"), true);
	API_DTO_FIELD_REQUIRE(String, desLocationId,
		ZH_WORDS_GETTER("dispense.field.des-location-id"), true);
	API_DTO_FIELD_REQUIRE(Int32, itemCount,
		ZH_WORDS_GETTER("dispense.field.item-count"), true);
	API_DTO_FIELD_REQUIRE(String, plannedDispenseTime,
		ZH_WORDS_GETTER("dispense.field.planned-dispense-time"), true);
	API_DTO_FIELD_REQUIRE(String, dispenseTime,
		ZH_WORDS_GETTER("dispense.field.dispense-time"), false);

public:
	DispenseSummaryDTO()
	{
		summaryNo = "SUM20260719001";
		statusEnum = "WAIT_REVIEW";
		desLocationId = "LOC-WARD-0301";
		itemCount = 3;
		plannedDispenseTime = "2026-07-19 09:30:00";
		dispenseTime = "2026-07-19 10:05:21";
	}
};

/**
 * Page DTO of summary drug-collection bills.
 */
class DispenseSummaryPageDTO : public PageDTO<DispenseSummaryDTO::Wrapper>
{
	DTO_INIT(DispenseSummaryPageDTO, PageDTO<DispenseSummaryDTO::Wrapper>);

public:
	DispenseSummaryPageDTO()
	{
		this->pageIndex = 1;
		this->pageSize = 10;
		this->total = 1;
		this->pages = 1;
		this->rows = List<DispenseSummaryDTO::Wrapper>::createShared();
		this->rows->push_back(DispenseSummaryDTO::createShared());
	}
};

/**
 * Dispense detail row DTO (one drug item of a summary bill).
 * Fields defined by the business flow chart: id / name / quantity / unitCode.
 */
class DispenseDetailDTO : public oatpp::DTO
{
	DTO_INIT(DispenseDetailDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, id,
		ZH_WORDS_GETTER("dispense.field.detail-id"), true);
	API_DTO_FIELD_REQUIRE(String, name,
		ZH_WORDS_GETTER("dispense.field.medication-name"), true);
	API_DTO_FIELD_REQUIRE(Float64, quantity,
		ZH_WORDS_GETTER("dispense.field.quantity"), true);
	API_DTO_FIELD_REQUIRE(String, unitCode,
		ZH_WORDS_GETTER("dispense.field.unit-code"), true);

public:
	DispenseDetailDTO()
	{
		id = "1";
		name = ZH_WORDS_GETTER("dispense.mock.med1-name");
		quantity = 2.0;
		unitCode = ZH_WORDS_GETTER("dispense.mock.med1-unit");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DISPENSE_DTO_
