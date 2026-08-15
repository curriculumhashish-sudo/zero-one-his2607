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
#ifndef _NURSING_RECORD_DTO_
#define _NURSING_RECORD_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class NursingRecordRowDTO : public oatpp::DTO
{
	DTO_INIT(NursingRecordRowDTO, DTO);

	// ======================== Primary Key & Record Identity ========================
	// Date time display (MM-dd HH:mm)
	API_DTO_FIELD_REQUIRE(String, dateTime, ZH_WORDS_GETTER("nursing.field.dateTime"), false);
	// Vital sign record ID (primary key)
	API_DTO_FIELD_REQUIRE(String, vitalSignId, ZH_WORDS_GETTER("nursing.field.vitalSignId"), false);
	// Record date (yyyy-MM-dd)
	API_DTO_FIELD_REQUIRE(String, recordDate, ZH_WORDS_GETTER("nursing.field.recordDate"), false);
	// Record time (HH:mm)
	API_DTO_FIELD_REQUIRE(String, recordTime, ZH_WORDS_GETTER("nursing.field.recordTime"), false);
	API_DTO_FIELD_REQUIRE(String, clientRowId,
		ZH_WORDS_GETTER("nursing.field.clientRowId"), false);

	// ======================== Record Metadata ========================
	// Patient identifier
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("nursing.field.patientId"), true);
	// Encounter identifier
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("nursing.field.encounterId"), true);
	// Recorded timestamp (format: yyyy-MM-dd HH:mm:ss)
	API_DTO_FIELD_REQUIRE(String, recordedAt, ZH_WORDS_GETTER("nursing.field.recordedAt"), true);
	// Business number
	API_DTO_FIELD_REQUIRE(String, busNo, ZH_WORDS_GETTER("nursing.field.busNo"), true);
	// Location / department identifier
	API_DTO_FIELD_REQUIRE(String, locationId, ZH_WORDS_GETTER("nursing.field.locationId"), false);
	// Baby number
	API_DTO_FIELD_REQUIRE(Int32, babyNo, ZH_WORDS_GETTER("nursing.field.babyNo"), false);
	// Recorder identifier
	API_DTO_FIELD_REQUIRE(String, recorderId, ZH_WORDS_GETTER("nursing.field.recorderId"), true);
	// Remark
	API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("nursing.field.remark"), false);
	API_DTO_FIELD_REQUIRE(Int64, version, ZH_WORDS_GETTER("nursing.field.version"), true);
	API_DTO_FIELD_REQUIRE(String, createdBy, ZH_WORDS_GETTER("nursing.field.createdBy"), false);
	API_DTO_FIELD_REQUIRE(String, createdAt, ZH_WORDS_GETTER("nursing.field.createdAt"), false);
	API_DTO_FIELD_REQUIRE(String, updatedBy, ZH_WORDS_GETTER("nursing.field.updatedBy"), false);
	API_DTO_FIELD_REQUIRE(String, updatedAt, ZH_WORDS_GETTER("nursing.field.updatedAt"), false);

	// ======================== Core Nursing Measurement Fields (15 fields) ========================
	// Temperature (unit: C)
	API_DTO_FIELD_REQUIRE(String, temperature, ZH_WORDS_GETTER("nursing.field.temperature"), false);
	// Pulse (unit: bpm)
	API_DTO_FIELD_REQUIRE(String, pulse, ZH_WORDS_GETTER("nursing.field.pulse"), false);
	// Heart rate (unit: bpm)
	API_DTO_FIELD_REQUIRE(String, heartRate, ZH_WORDS_GETTER("nursing.field.heartRate"), false);
	// Respiration (unit: breaths/min)
	API_DTO_FIELD_REQUIRE(String, respiration, ZH_WORDS_GETTER("nursing.field.respiration"), false);
	API_DTO_FIELD_REQUIRE(String, respiratory, ZH_WORDS_GETTER("nursing.field.respiratory"), false);
	// Blood pressure (unit: mmHg)
	API_DTO_FIELD_REQUIRE(String, bloodPressure, ZH_WORDS_GETTER("nursing.field.bloodPressure"), false);
	// Consciousness status
	API_DTO_FIELD_REQUIRE(String, consciousness, ZH_WORDS_GETTER("nursing.field.consciousness"), false);
	// Left pupil size (unit: mm)
	API_DTO_FIELD_REQUIRE(String, pupilLeft, ZH_WORDS_GETTER("nursing.field.pupilLeft"), false);
	// Right pupil size (unit: mm)
	API_DTO_FIELD_REQUIRE(String, pupilRight, ZH_WORDS_GETTER("nursing.field.pupilRight"), false);
	// Intake name (e.g. glucose, saline)
	API_DTO_FIELD_REQUIRE(String, intakeName, ZH_WORDS_GETTER("nursing.field.intakeName"), false);
	// Intake volume (unit: ml)
	API_DTO_FIELD_REQUIRE(String, intakeVolume, ZH_WORDS_GETTER("nursing.field.intakeVolume"), false);
	API_DTO_FIELD_REQUIRE(String, intakeAmount, ZH_WORDS_GETTER("nursing.field.intakeAmount"), false);
	// Output name (e.g. urine, drainage)
	API_DTO_FIELD_REQUIRE(String, outputName, ZH_WORDS_GETTER("nursing.field.outputName"), false);
	// Output volume (unit: ml)
	API_DTO_FIELD_REQUIRE(String, outputVolume, ZH_WORDS_GETTER("nursing.field.outputVolume"), false);
	API_DTO_FIELD_REQUIRE(String, outputAmount, ZH_WORDS_GETTER("nursing.field.outputAmount"), false);
	// Output color
	API_DTO_FIELD_REQUIRE(String, outputColor, ZH_WORDS_GETTER("nursing.field.outputColor"), false);
	// Condition observation and nursing record
	API_DTO_FIELD_REQUIRE(String, observation, ZH_WORDS_GETTER("nursing.field.observation"), false);
	// Nurse signature
	API_DTO_FIELD_REQUIRE(String, nurseSignature, ZH_WORDS_GETTER("nursing.field.nurseSignature"), false);
	API_DTO_FIELD_REQUIRE(String, oxygenSaturation, ZH_WORDS_GETTER("nursing.field.oxygenSaturation"), false);
	API_DTO_FIELD_REQUIRE(String, oxygenFlow, ZH_WORDS_GETTER("nursing.field.oxygenFlow"), false);
	API_DTO_FIELD_REQUIRE(String, oxygenMode, ZH_WORDS_GETTER("nursing.field.oxygenMode"), false);
	API_DTO_FIELD_REQUIRE(String, oxygenRate, ZH_WORDS_GETTER("nursing.field.oxygenRate"), false);
	API_DTO_FIELD_REQUIRE(String, skinStatus, ZH_WORDS_GETTER("nursing.field.skinStatus"), false);
	API_DTO_FIELD_REQUIRE(String, conditionMeasure, ZH_WORDS_GETTER("nursing.field.conditionMeasure"), false);
	API_DTO_FIELD_REQUIRE(String, tubeCare, ZH_WORDS_GETTER("nursing.field.tubeCare"), false);
	API_DTO_FIELD_REQUIRE(String, intakeRoute, ZH_WORDS_GETTER("nursing.field.intakeRoute"), false);
	API_DTO_FIELD_REQUIRE(String, outputColorCharacter, ZH_WORDS_GETTER("nursing.field.outputColorCharacter"), false);

public:
	NursingRecordRowDTO()
	{
		babyNo = 0;
		version = static_cast<v_int64>(0);
	}
};

class NursingRecordPaperHeaderDTO : public oatpp::DTO
{
	DTO_INIT(NursingRecordPaperHeaderDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, inpatientNo,
		ZH_WORDS_GETTER("nursing.field.inpatientNo"), false);
	API_DTO_FIELD_REQUIRE(String, name,
		ZH_WORDS_GETTER("nursing.field.patientName"), false);
	API_DTO_FIELD_REQUIRE(String, gender,
		ZH_WORDS_GETTER("nursing.field.gender"), false);
	API_DTO_FIELD_REQUIRE(String, age,
		ZH_WORDS_GETTER("nursing.field.age"), false);
	API_DTO_FIELD_REQUIRE(String, admitAt,
		ZH_WORDS_GETTER("nursing.field.admitAt"), false);
	API_DTO_FIELD_REQUIRE(String, dischargeAt,
		ZH_WORDS_GETTER("nursing.field.dischargeAt"), false);
	API_DTO_FIELD_REQUIRE(String, wardName,
		ZH_WORDS_GETTER("nursing.field.wardName"), false);
	API_DTO_FIELD_REQUIRE(String, bedNo,
		ZH_WORDS_GETTER("nursing.field.bedNo"), false);
};

class NursingRecordListResponseDTO : public oatpp::DTO
{
	DTO_INIT(NursingRecordListResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(NursingRecordPaperHeaderDTO::Wrapper, paperHeader,
		ZH_WORDS_GETTER("nursing.field.paperHeader"), false);
	API_DTO_FIELD_REQUIRE(List<NursingRecordRowDTO::Wrapper>, rows,
		ZH_WORDS_GETTER("nursing.field.rows"), false);

public:
	NursingRecordListResponseDTO()
	{
		paperHeader = NursingRecordPaperHeaderDTO::createShared();
		rows = List<NursingRecordRowDTO::Wrapper>::createShared();
	}
};

class NursingRecordSaveItemDTO : public oatpp::DTO
{
	DTO_INIT(NursingRecordSaveItemDTO, DTO);

	// ======================== Primary Key & Record Identity ========================
	// Date time display (MM-dd HH:mm)
	API_DTO_FIELD_REQUIRE(String, dateTime,
		ZH_WORDS_GETTER("nursing.field.dateTime"), false);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("nursing.field.vitalSignId"), false);
	API_DTO_FIELD_REQUIRE(String, clientRowId,
		ZH_WORDS_GETTER("nursing.field.clientRowId"), true);
	API_DTO_FIELD_REQUIRE(Int64, expectedVersion,
		ZH_WORDS_GETTER("nursing.field.expectedVersion"), false);
	API_DTO_FIELD_REQUIRE(String, recordDate,
		ZH_WORDS_GETTER("nursing.field.recordDate"), true);
	API_DTO_FIELD_REQUIRE(String, recordTime,
		ZH_WORDS_GETTER("nursing.field.recordTime"), true);

	// ======================== Core Nursing Measurement Fields (15 fields) ========================
	API_DTO_FIELD_REQUIRE(String, temperature,
		ZH_WORDS_GETTER("nursing.field.temperature"), false);
	API_DTO_FIELD_REQUIRE(String, pulse,
		ZH_WORDS_GETTER("nursing.field.pulse"), false);
	API_DTO_FIELD_REQUIRE(String, heartRate,
		ZH_WORDS_GETTER("nursing.field.heartRate"), false);
	API_DTO_FIELD_REQUIRE(String, respiration,
		ZH_WORDS_GETTER("nursing.field.respiration"), false);
	API_DTO_FIELD_REQUIRE(String, respiratory,
		ZH_WORDS_GETTER("nursing.field.respiratory"), false);
	API_DTO_FIELD_REQUIRE(String, bloodPressure,
		ZH_WORDS_GETTER("nursing.field.bloodPressure"), false);
	API_DTO_FIELD_REQUIRE(String, consciousness,
		ZH_WORDS_GETTER("nursing.field.consciousness"), false);
	API_DTO_FIELD_REQUIRE(String, pupilLeft,
		ZH_WORDS_GETTER("nursing.field.pupilLeft"), false);
	API_DTO_FIELD_REQUIRE(String, pupilRight,
		ZH_WORDS_GETTER("nursing.field.pupilRight"), false);
	API_DTO_FIELD_REQUIRE(String, intakeName,
		ZH_WORDS_GETTER("nursing.field.intakeName"), false);
	API_DTO_FIELD_REQUIRE(String, intakeVolume,
		ZH_WORDS_GETTER("nursing.field.intakeVolume"), false);
	API_DTO_FIELD_REQUIRE(String, intakeAmount,
		ZH_WORDS_GETTER("nursing.field.intakeAmount"), false);
	API_DTO_FIELD_REQUIRE(String, outputName,
		ZH_WORDS_GETTER("nursing.field.outputName"), false);
	API_DTO_FIELD_REQUIRE(String, outputVolume,
		ZH_WORDS_GETTER("nursing.field.outputVolume"), false);
	API_DTO_FIELD_REQUIRE(String, outputAmount,
		ZH_WORDS_GETTER("nursing.field.outputAmount"), false);
	API_DTO_FIELD_REQUIRE(String, outputColor,
		ZH_WORDS_GETTER("nursing.field.outputColor"), false);
	API_DTO_FIELD_REQUIRE(String, observation,
		ZH_WORDS_GETTER("nursing.field.observation"), false);
	API_DTO_FIELD_REQUIRE(String, nurseSignature,
		ZH_WORDS_GETTER("nursing.field.nurseSignature"), false);
	API_DTO_FIELD_REQUIRE(String, clientRowId,
		ZH_WORDS_GETTER("nursing.field.clientRowId"), true);
	API_DTO_FIELD_REQUIRE(Int64, expectedVersion,
		ZH_WORDS_GETTER("nursing.field.expectedVersion"), false);
	API_DTO_FIELD_REQUIRE(String, oxygenSaturation,
		ZH_WORDS_GETTER("nursing.field.oxygenSaturation"), false);
	API_DTO_FIELD_REQUIRE(String, oxygenFlow,
		ZH_WORDS_GETTER("nursing.field.oxygenFlow"), false);
	API_DTO_FIELD_REQUIRE(String, oxygenMode,
		ZH_WORDS_GETTER("nursing.field.oxygenMode"), false);
	API_DTO_FIELD_REQUIRE(String, oxygenRate,
		ZH_WORDS_GETTER("nursing.field.oxygenRate"), false);
	API_DTO_FIELD_REQUIRE(String, skinStatus,
		ZH_WORDS_GETTER("nursing.field.skinStatus"), false);
	API_DTO_FIELD_REQUIRE(String, conditionMeasure,
		ZH_WORDS_GETTER("nursing.field.conditionMeasure"), false);
	API_DTO_FIELD_REQUIRE(String, tubeCare,
		ZH_WORDS_GETTER("nursing.field.tubeCare"), false);
	API_DTO_FIELD_REQUIRE(String, intakeRoute,
		ZH_WORDS_GETTER("nursing.field.intakeRoute"), false);
	API_DTO_FIELD_REQUIRE(String, outputColorCharacter,
		ZH_WORDS_GETTER("nursing.field.outputColorCharacter"), false);

public:
	NursingRecordSaveItemDTO() = default;
};

class NursingRecordSaveRequestDTO : public oatpp::DTO
{
	DTO_INIT(NursingRecordSaveRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("nursing.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, patientId,
		ZH_WORDS_GETTER("nursing.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("nursing.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(List<NursingRecordSaveItemDTO::Wrapper>, records,
		ZH_WORDS_GETTER("nursing.field.records"), true);
	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("nursing.field.idempotencyKey"), true);

public:
	NursingRecordSaveRequestDTO() = default;

	static Wrapper createExample()
	{
		auto example = createShared();
		example->encounterId = "337907884548657152";
		example->patientId = "326940849401761792";
		example->idempotencyKey = "NURSING-SAVE-20260724-001";
		example->records = List<NursingRecordSaveItemDTO::Wrapper>::createShared();
		auto record = NursingRecordSaveItemDTO::createShared();
		record->clientRowId = "ROW-001";
		record->recordDate = "2026-07-24";
		record->recordTime = "08:30";
		record->temperature = "36.5";
		record->pulse = "80";
		record->heartRate = "80";
		record->respiration = "18";
		record->bloodPressure = "120/80";
		record->oxygenSaturation = "98";
		record->observation = "Stable";
		record->nurseSignature = "NURSE001";
		example->records->push_back(record);
		return example;
	}
};

class NursingRecordSaveItemResultDTO : public oatpp::DTO
{
	DTO_INIT(NursingRecordSaveItemResultDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, clientRowId,
		ZH_WORDS_GETTER("nursing.field.clientRowId"), true);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("nursing.field.vitalSignId"), false);
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("nursing.field.version"), false);
	API_DTO_FIELD_REQUIRE(Boolean, success,
		ZH_WORDS_GETTER("nursing.field.success"), true);
	API_DTO_FIELD_REQUIRE(String, errorCode,
		ZH_WORDS_GETTER("nursing.field.errorCode"), false);
	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("nursing.field.resultMessage"), false);
	API_DTO_FIELD_REQUIRE(NursingRecordRowDTO::Wrapper, record,
		ZH_WORDS_GETTER("nursing.field.savedRecord"), false);

public:
	NursingRecordSaveItemResultDTO()
	{
		version = static_cast<v_int64>(0);
		success = false;
	}
};

class NursingRecordSaveResponseDTO : public oatpp::DTO
{
	DTO_INIT(NursingRecordSaveResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<NursingRecordRowDTO::Wrapper>, rows,
		ZH_WORDS_GETTER("nursing.field.rows"), false);
	API_DTO_FIELD_REQUIRE(Int32, savedCount,
		ZH_WORDS_GETTER("nursing.field.savedCount"), false);
	API_DTO_FIELD_REQUIRE(List<NursingRecordSaveItemResultDTO::Wrapper>, results,
		ZH_WORDS_GETTER("nursing.field.results"), true);

public:
	NursingRecordSaveResponseDTO()
	{
		savedCount = 0;
		rows = List<NursingRecordRowDTO::Wrapper>::createShared();
		results = List<NursingRecordSaveItemResultDTO::Wrapper>::createShared();
	}
};

class NursingRecordDeleteRequestDTO : public oatpp::DTO
{
	DTO_INIT(NursingRecordDeleteRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("nursing.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, patientId,
		ZH_WORDS_GETTER("nursing.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(String, vitalSignId,
		ZH_WORDS_GETTER("nursing.field.vitalSignId"), true);
	API_DTO_FIELD_REQUIRE(Int64, expectedVersion,
		ZH_WORDS_GETTER("nursing.field.expectedVersion"), true);
	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("nursing.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(String, reason,
		ZH_WORDS_GETTER("nursing.field.deleteReason"), false);

public:
	NursingRecordDeleteRequestDTO() = default;

	static Wrapper createExample()
	{
		auto example = createShared();
		example->encounterId = "337907884548657152";
		example->patientId = "326940849401761792";
		example->vitalSignId = "338131147144708099";
		example->expectedVersion = static_cast<v_int64>(0);
		example->idempotencyKey = "NURSING-DELETE-338131147144708099";
		example->reason = "Duplicate entry";
		return example;
	}
};

class NursingRecordDeleteResponseDTO : public oatpp::DTO
{
	DTO_INIT(NursingRecordDeleteResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(Boolean, success,
		ZH_WORDS_GETTER("nursing.field.success"), true);
	API_DTO_FIELD_REQUIRE(String, statusEnum,
		ZH_WORDS_GETTER("nursing.field.statusEnum"), false);
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("nursing.field.version"), false);
	API_DTO_FIELD_REQUIRE(String, errorCode,
		ZH_WORDS_GETTER("nursing.field.errorCode"), false);
	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("nursing.field.resultMessage"), false);

public:
	NursingRecordDeleteResponseDTO()
	{
		success = false;
		version = static_cast<v_int64>(0);
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_NURSING_RECORD_DTO_
