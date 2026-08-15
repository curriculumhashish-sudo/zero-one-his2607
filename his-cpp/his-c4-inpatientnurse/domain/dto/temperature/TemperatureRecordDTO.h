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
	API_DTO_FIELD_REQUIRE(String, bloodPressure,
		ZH_WORDS_GETTER("temperature.field.bloodPressure"), false);
	API_DTO_FIELD_REQUIRE(String, specialEvent,
		ZH_WORDS_GETTER("temperature.field.specialEvent"), false);
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
	TemperatureSheetGetDataRequestDTO() = default;
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
	API_DTO_FIELD_REQUIRE(Fields<String>, paperHeader,
		ZH_WORDS_GETTER("temperature.field.paperHeader"), false);

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
	API_DTO_FIELD_REQUIRE(Int64, expectedVersion,
		ZH_WORDS_GETTER("temperature.field.expectedVersion"), false);
	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("temperature.field.idempotencyKey"), false);

public:
	TemperatureSheetSaveRequestDTO() = default;
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
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("temperature.field.version"), false);
	API_DTO_FIELD_REQUIRE(String, errorCode,
		ZH_WORDS_GETTER("temperature.field.errorCode"), false);

public:
	TemperatureSheetSaveResponseDTO()
	{
		version = static_cast<v_int64>(0);
	}
};

class TemperatureInitOptionDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureInitOptionDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, code,
		ZH_WORDS_GETTER("temperature.field.code"), true);
	API_DTO_FIELD_REQUIRE(String, name,
		ZH_WORDS_GETTER("temperature.field.name"), true);
	API_DTO_FIELD_REQUIRE(String, unit,
		ZH_WORDS_GETTER("temperature.field.unit"), false);
	API_DTO_FIELD_REQUIRE(String, valueFormat,
		ZH_WORDS_GETTER("temperature.field.valueFormat"), false);
	API_DTO_FIELD_REQUIRE(String, reasonableRange,
		ZH_WORDS_GETTER("temperature.field.reasonableRange"), false);
	API_DTO_FIELD_REQUIRE(Boolean, eventTimeRequired,
		ZH_WORDS_GETTER("temperature.field.eventTimeRequired"), false);
	API_DTO_FIELD_REQUIRE(Boolean, verticalDisplay,
		ZH_WORDS_GETTER("temperature.field.verticalDisplay"), false);
	API_DTO_FIELD_REQUIRE(Boolean, cooledTemperatureRequired,
		ZH_WORDS_GETTER("temperature.field.cooledTemperatureRequired"), false);
	API_DTO_FIELD_REQUIRE(Int32, displayOrder,
		ZH_WORDS_GETTER("temperature.field.displayOrder"), false);
};

class TemperatureInitResponseDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureInitResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, defaultWindowRange,
		ZH_WORDS_GETTER("temperature.field.defaultWindowRange"), true);
	API_DTO_FIELD_REQUIRE(List<String>, windowRanges,
		ZH_WORDS_GETTER("temperature.field.windowRanges"), true);
	API_DTO_FIELD_REQUIRE(List<TemperatureInitOptionDTO::Wrapper>, timeSlots,
		ZH_WORDS_GETTER("temperature.field.timeSlots"), true);
	API_DTO_FIELD_REQUIRE(List<TemperatureInitOptionDTO::Wrapper>, specialEvents,
		ZH_WORDS_GETTER("temperature.field.specialEvents"), true);
	API_DTO_FIELD_REQUIRE(List<TemperatureInitOptionDTO::Wrapper>, vitalSignItems,
		ZH_WORDS_GETTER("temperature.field.vitalSignItems"), true);

public:
	TemperatureInitResponseDTO()
	{
		defaultWindowRange = "7d";
		windowRanges = List<String>::createShared();
		timeSlots = List<TemperatureInitOptionDTO::Wrapper>::createShared();
		specialEvents = List<TemperatureInitOptionDTO::Wrapper>::createShared();
		vitalSignItems = List<TemperatureInitOptionDTO::Wrapper>::createShared();
	}
};

class TemperatureSheetBatchSaveSlotDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetBatchSaveSlotDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<TemperatureSheetDetailItemDTO::Wrapper>, detailItems,
		ZH_WORDS_GETTER("temperature.field.detailItems"), true);
	API_DTO_FIELD_REQUIRE(String, timeSlot,
		ZH_WORDS_GETTER("temperature.field.timeSlot"), true);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("temperature.field.vitalSignId"), false);
	API_DTO_FIELD_REQUIRE(Int64, expectedVersion,
		ZH_WORDS_GETTER("temperature.field.expectedVersion"), false);
};

class TemperatureSheetBatchSaveRequestDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetBatchSaveRequestDTO, DTO);

public:

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("temperature.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("temperature.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, patientId,
		ZH_WORDS_GETTER("temperature.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(String, recordDate,
		ZH_WORDS_GETTER("temperature.field.recordDate"), true);
	API_DTO_FIELD_REQUIRE(List<TemperatureSheetBatchSaveSlotDTO::Wrapper>, slots,
		ZH_WORDS_GETTER("temperature.field.slots"), true);
	API_DTO_FIELD_REQUIRE(List<TemperatureSheetDetailItemDTO::Wrapper>, dailySupplement,
		ZH_WORDS_GETTER("temperature.field.dailySupplement"), false);
};

class TemperatureSheetBatchSaveItemResultDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetBatchSaveItemResultDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, timeSlot,
		ZH_WORDS_GETTER("temperature.field.timeSlot"), true);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("temperature.field.vitalSignId"), false);
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("temperature.field.version"), false);
	API_DTO_FIELD_REQUIRE(String, nextTimeSlot,
		ZH_WORDS_GETTER("temperature.field.nextTimeSlot"), false);
	API_DTO_FIELD_REQUIRE(Boolean, success,
		ZH_WORDS_GETTER("temperature.field.success"), true);
	API_DTO_FIELD_REQUIRE(String, errorCode,
		ZH_WORDS_GETTER("temperature.field.errorCode"), false);
	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("temperature.field.resultMessage"), false);
	API_DTO_FIELD_REQUIRE(TemperatureSheetGridSlotCellDTO::Wrapper, savedCell,
		ZH_WORDS_GETTER("temperature.field.savedCell"), false);

public:
	TemperatureSheetBatchSaveItemResultDTO()
	{
		version = static_cast<v_int64>(0);
		success = false;
	}
};

class TemperatureSheetBatchSaveResponseDTO : public oatpp::DTO
{
	DTO_INIT(TemperatureSheetBatchSaveResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(Int32, savedCount,
		ZH_WORDS_GETTER("temperature.field.savedCount"), true);
	API_DTO_FIELD_REQUIRE(List<TemperatureSheetBatchSaveItemResultDTO::Wrapper>, results,
		ZH_WORDS_GETTER("temperature.field.results"), true);

public:
	TemperatureSheetBatchSaveResponseDTO()
	{
		savedCount = 0;
		results = List<TemperatureSheetBatchSaveItemResultDTO::Wrapper>::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TEMPERATURE_RECORD_DTO_
