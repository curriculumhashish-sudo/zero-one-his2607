#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/08/06 00:00:00

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
#ifndef _ADMISSION_TRANSFER_WORKBENCH_DTO_
#define _ADMISSION_TRANSFER_WORKBENCH_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class BedBoardBedDTO : public oatpp::DTO
{
	DTO_INIT(BedBoardBedDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, bedId, ZH_WORDS_GETTER("admissiontransfer.field.bedId"), true);
	API_DTO_FIELD_REQUIRE(String, bedName, ZH_WORDS_GETTER("admissiontransfer.field.bedName"), true);
	API_DTO_FIELD_REQUIRE(String, wardId, ZH_WORDS_GETTER("admissiontransfer.field.wardId"), true);
	API_DTO_FIELD_REQUIRE(String, wardName, ZH_WORDS_GETTER("admissiontransfer.field.wardName"), true);
	API_DTO_FIELD_REQUIRE(String, houseId, ZH_WORDS_GETTER("admissiontransfer.field.houseId"), true);
	API_DTO_FIELD_REQUIRE(String, houseName, ZH_WORDS_GETTER("admissiontransfer.field.houseName"), true);
	API_DTO_FIELD_REQUIRE(Int32, displayOrder, ZH_WORDS_GETTER("admissiontransfer.field.displayOrder"), true);
	API_DTO_FIELD_REQUIRE(String, operationalEnum, ZH_WORDS_GETTER("admissiontransfer.field.operationalEnum"), true);
	API_DTO_FIELD_REQUIRE(Int64, version, ZH_WORDS_GETTER("admissiontransfer.field.version"), true);

public:
	BedBoardBedDTO()
	{
		displayOrder = 0;
		version = static_cast<v_int64>(0);
	}
};

class BedBoardOccupantDTO : public oatpp::DTO
{
	DTO_INIT(BedBoardOccupantDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("admissiontransfer.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(String, patientName, ZH_WORDS_GETTER("admissiontransfer.field.patientName"), true);
	API_DTO_FIELD_REQUIRE(String, genderEnumText, ZH_WORDS_GETTER("admissiontransfer.field.genderEnumText"), true);
	API_DTO_FIELD_REQUIRE(String, ageText, ZH_WORDS_GETTER("admissiontransfer.field.ageText"), true);
	API_DTO_FIELD_REQUIRE(String, statusEnum, ZH_WORDS_GETTER("admissiontransfer.field.statusEnum"), true);
	API_DTO_FIELD_REQUIRE(String, statusEnumText, ZH_WORDS_GETTER("admissiontransfer.field.statusEnumText"), true);
};

class BedBoardItemDTO : public oatpp::DTO
{
	DTO_INIT(BedBoardItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(Object<BedBoardBedDTO>, bed, ZH_WORDS_GETTER("admissiontransfer.field.bed"), true);
	API_DTO_FIELD_REQUIRE(Object<BedBoardOccupantDTO>, occupant, ZH_WORDS_GETTER("admissiontransfer.field.occupant"), false);
};

class BedBoardDTO : public oatpp::DTO
{
	DTO_INIT(BedBoardDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<Object<BedBoardItemDTO>>, items, ZH_WORDS_GETTER("admissiontransfer.field.items"), true);

public:
	BedBoardDTO()
	{
		items = List<Object<BedBoardItemDTO>>::createShared();
	}
};

class BedOperationParticipantDTO : public oatpp::DTO
{
	DTO_INIT(BedOperationParticipantDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, practitionerId, ZH_WORDS_GETTER("admissiontransfer.field.practitionerId"), true);
	API_DTO_FIELD_REQUIRE(String, roleCode, ZH_WORDS_GETTER("admissiontransfer.field.roleCode"), true);
};

class AdmissionVitalSignDTO : public oatpp::DTO
{
	DTO_INIT(AdmissionVitalSignDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, temperature, ZH_WORDS_GETTER("admissiontransfer.field.temperature"), false);
	API_DTO_FIELD_REQUIRE(String, pulse, ZH_WORDS_GETTER("admissiontransfer.field.pulse"), false);
	API_DTO_FIELD_REQUIRE(String, heartRate, ZH_WORDS_GETTER("admissiontransfer.field.heartRate"), false);
	API_DTO_FIELD_REQUIRE(String, bloodPressureHigh, ZH_WORDS_GETTER("admissiontransfer.field.bloodPressureHigh"), false);
	API_DTO_FIELD_REQUIRE(String, bloodPressureLow, ZH_WORDS_GETTER("admissiontransfer.field.bloodPressureLow"), false);
	API_DTO_FIELD_REQUIRE(String, heightCm, ZH_WORDS_GETTER("admissiontransfer.field.heightCm"), false);
	API_DTO_FIELD_REQUIRE(String, weightKg, ZH_WORDS_GETTER("admissiontransfer.field.weightKg"), false);
};

class ExecuteBedOperationDTO : public oatpp::DTO
{
	DTO_INIT(ExecuteBedOperationDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, operationType, ZH_WORDS_GETTER("admissiontransfer.field.operationType"), true);
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, targetBedId, ZH_WORDS_GETTER("admissiontransfer.field.targetBedId"), true);
	API_DTO_FIELD_REQUIRE(String, targetHouseId, ZH_WORDS_GETTER("admissiontransfer.field.targetHouseId"), true);
	API_DTO_FIELD_REQUIRE(String, swapEncounterId, ZH_WORDS_GETTER("admissiontransfer.field.swapEncounterId"), false);
	API_DTO_FIELD_REQUIRE(Int64, expectedBedVersion, ZH_WORDS_GETTER("admissiontransfer.field.expectedBedVersion"), true);
	API_DTO_FIELD_REQUIRE(String, idempotencyKey, ZH_WORDS_GETTER("admissiontransfer.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(String, priorityEnum, ZH_WORDS_GETTER("admissiontransfer.field.priorityEnum"), true);
	API_DTO_FIELD_REQUIRE(List<Object<BedOperationParticipantDTO>>, participants, ZH_WORDS_GETTER("admissiontransfer.field.participants"), true);
	API_DTO_FIELD_REQUIRE(Object<AdmissionVitalSignDTO>, admissionVitalSigns, ZH_WORDS_GETTER("admissiontransfer.field.admissionVitalSigns"), false);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->operationType = "ASSIGN";
		dto->encounterId = "337907884548657152";
		dto->targetBedId = "BED30101";
		dto->targetHouseId = "HOUSE-301";
		dto->expectedBedVersion = 3;
		dto->idempotencyKey = "ATD-337907884548657152";
		dto->priorityEnum = "NORMAL";
		dto->participants = List<Object<BedOperationParticipantDTO>>::createShared();
		auto participant = BedOperationParticipantDTO::createShared();
		participant->practitionerId = "326940849401761792";
		participant->roleCode = "PRIMARY_NURSE";
		dto->participants->push_back(participant);
		dto->admissionVitalSigns = AdmissionVitalSignDTO::createShared();
		dto->admissionVitalSigns->temperature = "36.5";
		dto->admissionVitalSigns->pulse = "80";
		dto->admissionVitalSigns->heartRate = "80";
		dto->admissionVitalSigns->bloodPressureHigh = "120";
		dto->admissionVitalSigns->bloodPressureLow = "80";
		dto->admissionVitalSigns->heightCm = "168";
		dto->admissionVitalSigns->weightKg = "60";
		return dto;
	}
};

class BedOperationResponseDTO : public oatpp::DTO
{
	DTO_INIT(BedOperationResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(Boolean, succeeded, ZH_WORDS_GETTER("admissiontransfer.field.succeeded"), true);
	API_DTO_FIELD_REQUIRE(String, targetBedId, ZH_WORDS_GETTER("admissiontransfer.field.targetBedId"), true);
	API_DTO_FIELD_REQUIRE(String, errorCode,
		ZH_WORDS_GETTER("admissiontransfer.field.errorCode"), false);
	API_DTO_FIELD_REQUIRE(String, errorMessage,
		ZH_WORDS_GETTER("admissiontransfer.field.errorMessage"), false);

public:
	BedOperationResponseDTO()
	{
		succeeded = false;
		errorCode = "";
		errorMessage = "";
	}
};

class RecallInpatientDischargeDTO : public oatpp::DTO
{
	DTO_INIT(RecallInpatientDischargeDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->encounterId = "337907884548657152";
		return dto;
	}
};

class RecallInpatientDischargeResponseDTO : public oatpp::DTO
{
	DTO_INIT(RecallInpatientDischargeResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(Boolean, succeeded, ZH_WORDS_GETTER("admissiontransfer.field.succeeded"), true);
	API_DTO_FIELD_REQUIRE(String, targetStatusEnum, ZH_WORDS_GETTER("admissiontransfer.field.targetStatusEnum"), true);
	API_DTO_FIELD_REQUIRE(Boolean, requiresBedAssignment, ZH_WORDS_GETTER("admissiontransfer.field.requiresBedAssignment"), true);

public:
	RecallInpatientDischargeResponseDTO()
	{
		succeeded = false;
		requiresBedAssignment = false;
	}
};

class DischargePendingItemDTO : public oatpp::DTO
{
	DTO_INIT(DischargePendingItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, requestId, ZH_WORDS_GETTER("admissiontransfer.field.requestId"), true);
	API_DTO_FIELD_REQUIRE(String, requestType, ZH_WORDS_GETTER("admissiontransfer.field.requestType"), true);
	API_DTO_FIELD_REQUIRE(String, statusEnum, ZH_WORDS_GETTER("admissiontransfer.field.statusEnum"), true);
	API_DTO_FIELD_REQUIRE(String, content, ZH_WORDS_GETTER("admissiontransfer.field.content"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ADMISSION_TRANSFER_WORKBENCH_DTO_
