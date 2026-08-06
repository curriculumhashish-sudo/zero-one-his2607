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
	API_DTO_FIELD_REQUIRE(String, cooledTemperature,
		ZH_WORDS_GETTER("temperature.field.cooledTemperature"), false);
	API_DTO_FIELD_REQUIRE(String, pulse,
		ZH_WORDS_GETTER("temperature.field.pulse"), false);
	API_DTO_FIELD_REQUIRE(String, heartRate,
		ZH_WORDS_GETTER("temperature.field.heartRate"), false);
	API_DTO_FIELD_REQUIRE(String, respiration,
		ZH_WORDS_GETTER("temperature.field.respiration"), false);
	API_DTO_FIELD_REQUIRE(String, systolicPressure,
		ZH_WORDS_GETTER("temperature.field.systolicPressure"), false);
	API_DTO_FIELD_REQUIRE(String, diastolicPressure,
		ZH_WORDS_GETTER("temperature.field.diastolicPressure"), false);
	API_DTO_FIELD_REQUIRE(String, specialEventCode,
		ZH_WORDS_GETTER("temperature.field.specialEventCode"), false);
	API_DTO_FIELD_REQUIRE(String, specialEventTime,
		ZH_WORDS_GETTER("temperature.field.specialEventTime"), false);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("temperature.field.vitalSignId"), false);
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("temperature.field.version"), false);
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
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("temperature.field.version"), false);
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

	API_DTO_FIELD_REQUIRE(Fields<Int32>, hospitalizationDaysByDate,
		ZH_WORDS_GETTER("temperature.field.hospitalizationDaysByDate"), false);
	API_DTO_FIELD_REQUIRE(Fields<Int32>, postoperativeDaysByDate,
		ZH_WORDS_GETTER("temperature.field.postoperativeDaysByDate"), false);
	API_DTO_FIELD_REQUIRE(Fields<Int32>, weekNoByDate,
		ZH_WORDS_GETTER("temperature.field.weekNoByDate"), false);
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
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("temperature.field.version"), false);
};

class TemperatureInitOptionDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureInitOptionDTO, DTO);
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("temperature.field.code"), true);
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("temperature.field.name"), true);
	API_DTO_FIELD_REQUIRE(String, unit, ZH_WORDS_GETTER("temperature.field.unit"), false);
	API_DTO_FIELD_REQUIRE(String, valueFormat, ZH_WORDS_GETTER("temperature.field.valueFormat"), false);
	API_DTO_FIELD_REQUIRE(String, reasonableRange, ZH_WORDS_GETTER("temperature.field.reasonableRange"), false);
	API_DTO_FIELD_REQUIRE(Boolean, requiresEventTime, ZH_WORDS_GETTER("temperature.field.requiresEventTime"), false);
	API_DTO_FIELD_REQUIRE(Boolean, verticalDisplay, ZH_WORDS_GETTER("temperature.field.verticalDisplay"), false);
	API_DTO_FIELD_REQUIRE(Boolean, requiresCooledTemperature, ZH_WORDS_GETTER("temperature.field.requiresCooledTemperature"), false);
	API_DTO_FIELD_REQUIRE(Int32, displayOrder, ZH_WORDS_GETTER("temperature.field.displayOrder"), false);
};

class TemperatureInitDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureInitDTO, DTO);
	API_DTO_FIELD_REQUIRE(String, defaultWindowRange, ZH_WORDS_GETTER("temperature.field.defaultWindowRange"), false);
	API_DTO_FIELD_REQUIRE(List<TemperatureInitOptionDTO::Wrapper>, windowRanges, ZH_WORDS_GETTER("temperature.field.windowRanges"), false);
	API_DTO_FIELD_REQUIRE(List<TemperatureInitOptionDTO::Wrapper>, timeSlots, ZH_WORDS_GETTER("temperature.field.timeSlots"), false);
	API_DTO_FIELD_REQUIRE(List<TemperatureInitOptionDTO::Wrapper>, specialEvents, ZH_WORDS_GETTER("temperature.field.specialEvents"), false);
	API_DTO_FIELD_REQUIRE(List<TemperatureInitOptionDTO::Wrapper>, measurementItems, ZH_WORDS_GETTER("temperature.field.measurementItems"), false);

public:
	TemperatureInitDTO()
	{
		defaultWindowRange = "7d";
		windowRanges = List<TemperatureInitOptionDTO::Wrapper>::createShared();
		timeSlots = List<TemperatureInitOptionDTO::Wrapper>::createShared();
		specialEvents = List<TemperatureInitOptionDTO::Wrapper>::createShared();
		measurementItems = List<TemperatureInitOptionDTO::Wrapper>::createShared();
		auto append = [](const List<TemperatureInitOptionDTO::Wrapper>& target,
			const char* code, const oatpp::String& name, const char* unit = nullptr) {
			auto option = TemperatureInitOptionDTO::createShared();
			option->code = code;
			option->name = name;
			if (unit) option->unit = unit;
			target->push_back(option);
		};
		append(windowRanges, "3d", ZH_WORDS_GETTER("business-text.temperature.window-3d"));
		append(windowRanges, "7d", ZH_WORDS_GETTER("business-text.temperature.window-7d"));
		append(windowRanges, "14d", ZH_WORDS_GETTER("business-text.temperature.window-14d"));
		for (const char* value : {"02", "06", "10", "14", "18", "22"})
		{
			auto option = TemperatureInitOptionDTO::createShared();
			option->code = value;
			option->name = value;
			timeSlots->push_back(option);
		}
		append(specialEvents, "ADMISSION", ZH_WORDS_GETTER("business-text.temperature.event-admission"));
		append(specialEvents, "TRANSFER", ZH_WORDS_GETTER("business-text.temperature.event-transfer"));
		append(specialEvents, "SURGERY", ZH_WORDS_GETTER("business-text.temperature.event-surgery"));
		append(specialEvents, "DELIVERY", ZH_WORDS_GETTER("business-text.temperature.event-delivery"));
		append(specialEvents, "DISCHARGE", ZH_WORDS_GETTER("business-text.temperature.event-discharge"));
		append(specialEvents, "DEATH", ZH_WORDS_GETTER("business-text.temperature.event-death"));
		append(specialEvents, "EXTERNAL_EXAM", ZH_WORDS_GETTER("business-text.temperature.event-external-exam"));
		append(specialEvents, "REFUSED", ZH_WORDS_GETTER("business-text.temperature.event-refused"));
		append(specialEvents, "PHYSICAL_COOLING", ZH_WORDS_GETTER("business-text.temperature.event-physical-cooling"));
		append(specialEvents, "DRUG_COOLING", ZH_WORDS_GETTER("business-text.temperature.event-drug-cooling"));
		append(specialEvents, "TEMP_NOT_RISING", ZH_WORDS_GETTER("business-text.temperature.event-temp-not-rising"));
		append(specialEvents, "PULSE_DEFICIT", ZH_WORDS_GETTER("business-text.temperature.event-pulse-deficit"));
		append(measurementItems, "TEMP", ZH_WORDS_GETTER("temperature.field.temperature"), "C");
		append(measurementItems, "PULSE", ZH_WORDS_GETTER("temperature.field.pulse"), "bpm");
		append(measurementItems, "HEART_RATE", ZH_WORDS_GETTER("temperature.field.heartRate"), "bpm");
		append(measurementItems, "RESPIRATION", ZH_WORDS_GETTER("temperature.field.respiration"), "breaths/min");
	}
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
