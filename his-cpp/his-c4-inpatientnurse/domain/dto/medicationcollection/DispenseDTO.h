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
	// summary bill no
	DTO_FIELD(String, summaryNo) = "SUM20260719001";
	DTO_FIELD_INFO(summaryNo) {
		info->description = ZH_WORDS_GETTER("dispense.field.summary-no");
	}
	// bill status enum (WAIT_REVIEW / DISPENSED)
	DTO_FIELD(String, statusEnum) = "WAIT_REVIEW";
	DTO_FIELD_INFO(statusEnum) {
		info->description = ZH_WORDS_GETTER("dispense.field.status-enum");
	}
	// destination location id (collecting ward)
	DTO_FIELD(String, desLocationId) = "LOC-WARD-0301";
	DTO_FIELD_INFO(desLocationId) {
		info->description = ZH_WORDS_GETTER("dispense.field.des-location-id");
	}
	// drug item count of this summary bill
	DTO_FIELD(Int32, itemCount) = 3;
	DTO_FIELD_INFO(itemCount) {
		info->description = ZH_WORDS_GETTER("dispense.field.item-count");
	}
	// planned dispense time
	DTO_FIELD(String, plannedDispenseTime) = "2026-07-19 09:30:00";
	DTO_FIELD_INFO(plannedDispenseTime) {
		info->description = ZH_WORDS_GETTER("dispense.field.planned-dispense-time");
	}
	// dispense time
	DTO_FIELD(String, dispenseTime) = "2026-07-19 10:05:21";
	DTO_FIELD_INFO(dispenseTime) {
		info->description = ZH_WORDS_GETTER("dispense.field.dispense-time");
	}
};

/**
 * Page DTO of summary drug-collection bills.
 */
class DispenseSummaryPageDTO : public PageDTO<DispenseSummaryDTO::Wrapper>
{
	DTO_INIT(DispenseSummaryPageDTO, PageDTO<DispenseSummaryDTO::Wrapper>);
};

/**
 * Dispense detail row DTO (one drug item of a summary bill).
 * Fields defined by the business flow chart: id / name / quantity / unitCode.
 */
class DispenseDetailDTO : public oatpp::DTO
{
	DTO_INIT(DispenseDetailDTO, DTO);
	// detail id (one per drug item)
	DTO_FIELD(String, id) = "1";
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("dispense.field.detail-id");
	}
	// medication name (joined from med_medication_definition)
	DTO_FIELD(String, name) = ZH_WORDS_GETTER("dispense.mock.med1-name");
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("dispense.field.medication-name");
	}
	// requested quantity
	DTO_FIELD(Float64, quantity) = 2.0;
	DTO_FIELD_INFO(quantity) {
		info->description = ZH_WORDS_GETTER("dispense.field.quantity");
	}
	// unit code (specification)
	DTO_FIELD(String, unitCode) = ZH_WORDS_GETTER("dispense.mock.med1-unit");
	DTO_FIELD_INFO(unitCode) {
		info->description = ZH_WORDS_GETTER("dispense.field.unit-code");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DISPENSE_DTO_
