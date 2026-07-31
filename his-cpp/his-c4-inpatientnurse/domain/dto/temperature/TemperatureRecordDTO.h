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
#ifndef _TEMPERATURE_RECORD_DTO_
#define _TEMPERATURE_RECORD_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class TemperatureSheetDetailItemDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetDetailItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, itemCode,
		ZH_WORDS_GETTER("temperature.field.itemCode"), false);
	API_DTO_FIELD_REQUIRE(String, itemValue,
		ZH_WORDS_GETTER("temperature.field.itemValue"), false);
	API_DTO_FIELD_REQUIRE(String, remark,
		ZH_WORDS_GETTER("temperature.field.remark"), false);
	API_DTO_FIELD_REQUIRE(String, termCode,
		ZH_WORDS_GETTER("temperature.field.termCode"), false);
};

class TemperatureSheetTemperaturePointDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetTemperaturePointDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, recordedAt,
		ZH_WORDS_GETTER("temperature.field.recordedAt"), false);
	API_DTO_FIELD_REQUIRE(String, temperature,
		ZH_WORDS_GETTER("temperature.field.temperature"), false);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("temperature.field.vitalSignId"), false);
};

class TemperatureSheetGridSlotCellDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetGridSlotCellDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<TemperatureSheetDetailItemDTO::Wrapper>, detailItems,
		ZH_WORDS_GETTER("temperature.field.detailItems"), true);
	API_DTO_FIELD_REQUIRE(String, timeSlot,
		ZH_WORDS_GETTER("temperature.field.timeSlot"), false);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("temperature.field.vitalSignId"), false);
};

class TemperatureSheetGridRowDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetGridRowDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, recordDate,
		ZH_WORDS_GETTER("temperature.field.recordDate"), false);
	API_DTO_FIELD_REQUIRE(List<TemperatureSheetGridSlotCellDTO::Wrapper>, slotCells,
		ZH_WORDS_GETTER("temperature.field.slotCells"), false);
};

class TemperatureSheetPaginationDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetPaginationDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, dateFrom,
		ZH_WORDS_GETTER("temperature.field.dateFrom"), false);
	API_DTO_FIELD_REQUIRE(String, dateTo,
		ZH_WORDS_GETTER("temperature.field.dateTo"), false);
	API_DTO_FIELD_REQUIRE(Int32, pageIndex,
		ZH_WORDS_GETTER("temperature.field.pageIndex"), false);
	API_DTO_FIELD_REQUIRE(Int32, totalPages,
		ZH_WORDS_GETTER("temperature.field.totalPages"), false);
};

class TemperatureSheetDerivedMetaDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetDerivedMetaDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, hospitalizationDaysByDate,
		ZH_WORDS_GETTER("temperature.field.hospitalizationDaysByDate"), false);
	API_DTO_FIELD_REQUIRE(String, postoperativeDaysByDate,
		ZH_WORDS_GETTER("temperature.field.postoperativeDaysByDate"), false);
};

class TemperatureSheetFillSlotDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetFillSlotDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<TemperatureSheetDetailItemDTO::Wrapper>, detailItems,
		ZH_WORDS_GETTER("temperature.field.detailItems"), true);
	API_DTO_FIELD_REQUIRE(String, timeSlot,
		ZH_WORDS_GETTER("temperature.field.timeSlot"), false);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("temperature.field.vitalSignId"), false);
};

class TemperatureSheetGetDataRequestDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetGetDataRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("temperature.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, focusRecordDate,
		ZH_WORDS_GETTER("temperature.field.focusRecordDate"), false);
	API_DTO_FIELD_REQUIRE(Int32, pageIndex,
		ZH_WORDS_GETTER("temperature.field.pageIndex"), false);
	API_DTO_FIELD_REQUIRE(String, patientId,
		ZH_WORDS_GETTER("temperature.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(String, windowRange,
		ZH_WORDS_GETTER("temperature.field.windowRange"), true);

public:
	TemperatureSheetGetDataRequestDTO()
	{
		encounterId = "337907884548657152";
		patientId = "326940849401761792";
		windowRange = "7d";
		pageIndex = 1;
	}
};

class TemperatureSheetGetDataResponseDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetGetDataResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(TemperatureSheetDerivedMetaDTO::Wrapper, derivedMeta,
		ZH_WORDS_GETTER("temperature.field.derivedMeta"), false);
	API_DTO_FIELD_REQUIRE(List<TemperatureSheetFillSlotDTO::Wrapper>, fillSlots,
		ZH_WORDS_GETTER("temperature.field.fillSlots"), false);
	API_DTO_FIELD_REQUIRE(List<TemperatureSheetGridRowDTO::Wrapper>, gridRows,
		ZH_WORDS_GETTER("temperature.field.gridRows"), false);
	API_DTO_FIELD_REQUIRE(TemperatureSheetPaginationDTO::Wrapper, pagination,
		ZH_WORDS_GETTER("temperature.field.pagination"), false);
	API_DTO_FIELD_REQUIRE(List<TemperatureSheetTemperaturePointDTO::Wrapper>, temperaturePoints,
		ZH_WORDS_GETTER("temperature.field.temperaturePoints"), false);

public:
	TemperatureSheetGetDataResponseDTO()
	{
		gridRows = List<TemperatureSheetGridRowDTO::Wrapper>::createShared();
		temperaturePoints = List<TemperatureSheetTemperaturePointDTO::Wrapper>::createShared();
	}
};

class TemperatureSheetSaveRequestDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetSaveRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<TemperatureSheetDetailItemDTO::Wrapper>, detailItems,
		ZH_WORDS_GETTER("temperature.field.detailItems"), false);
	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("temperature.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, patientId,
		ZH_WORDS_GETTER("temperature.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(String, recordDate,
		ZH_WORDS_GETTER("temperature.field.recordDate"), true);
	API_DTO_FIELD_REQUIRE(String, timeSlot,
		ZH_WORDS_GETTER("temperature.field.timeSlot"), true);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("temperature.field.vitalSignId"), false);

public:
	TemperatureSheetSaveRequestDTO()
	{
		encounterId = "337907884548657152";
		patientId = "326940849401761792";
		recordDate = "2026-07-24";
		timeSlot = "10";

		detailItems = List<TemperatureSheetDetailItemDTO::Wrapper>::createShared();
		auto add = [this](const char* code, const char* value) {
			auto item = TemperatureSheetDetailItemDTO::createShared();
			item->itemCode = code;
			item->itemValue = value;
			item->termCode = "vital_signs_code";
			detailItems->push_back(item);
		};
		// Vital signs
		add("TEMP", "36.5");
		add("PULSE", "80");
		add("RESPIRATION", "18");
		add("HEART_RATE", "80");
		add("SYSTOLIC_PRESSURE", "120");
		add("DIASTOLIC_PRESSURE", "80");
		// Event flags
		add("PHYSICAL_COOLING", "false");
		add("TEMP_AFTER_PHYSICAL_COOLING", "");
		add("DRUG_COOLING", "false");
		add("TEMP_AFTER_DRUG_COOLING", "");
		add("TEMP_NOT_RISING", "false");
		add("DISCHARGE", "false");
		add("DISCHARGE_TIME", "");
		// Special event: Transfer/Surgery/Delivery/Death/ExternalExam/Refuse
		add("SPECIAL_EVENT", "");
		add("SPECIAL_EVENT_TIME", "");
		// Supplementary (daily summary below the grid)
		add("WEIGHT", "65");
		add("WAIST", "80");
		add("HEIGHT", "170");
		add("SKIN_TEST_POSITIVE", "");
		// Previous-day summary (saved on date X, displayed on date X-1)
		add("STOOL", "1");
		add("URINE_FREQ", "6");
		add("INTAKE", "1500");
		add("URINE", "1200");
		add("OTHER", "100");
	}
};

class TemperatureSheetSaveResponseDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetSaveResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, busNo,
		ZH_WORDS_GETTER("temperature.field.busNo"), false);
	API_DTO_FIELD_REQUIRE(String, nextTimeSlot,
		ZH_WORDS_GETTER("temperature.field.nextTimeSlot"), false);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("temperature.field.vitalSignId"), false);

public:
	TemperatureSheetSaveResponseDTO()
	{
		busNo = "T20260724001";
		vitalSignId = "338131147144708100";
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TEMPERATURE_RECORD_DTO_
