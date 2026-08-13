#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/20 10:00:00

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
#ifndef _TEMPERATURE_DTO_
    #define _TEMPERATURE_DTO_
    #include "../../GlobalInclude.h"

    #include OATPP_CODEGEN_BEGIN(DTO)

// ============================================================================
// ============================================================================

/**
 */
class TemperatureTimeSlotDTO : public oatpp::DTO
{
    DTO_INIT(TemperatureTimeSlotDTO, DTO);
    API_DTO_FIELD_REQUIRE(String, timeSlot, ZH_WORDS_GETTER("temperature.field.timeSlot"), true);
    API_DTO_FIELD_REQUIRE(String, temperature, ZH_WORDS_GETTER("temperature.field.temperature"), false);
    API_DTO_FIELD_REQUIRE(String, pulse, ZH_WORDS_GETTER("temperature.field.pulse"), false);
    API_DTO_FIELD_REQUIRE(String, respiration, ZH_WORDS_GETTER("temperature.field.respiration"), false);
    API_DTO_FIELD_REQUIRE(String, heartRate, ZH_WORDS_GETTER("temperature.field.heartRate"), false);
    API_DTO_FIELD_REQUIRE(String, systolicPressure, ZH_WORDS_GETTER("temperature.field.systolicPressure"), false);
    API_DTO_FIELD_REQUIRE(String, diastolicPressure, ZH_WORDS_GETTER("temperature.field.diastolicPressure"), false);
    API_DTO_FIELD_REQUIRE(Boolean, physicalCooling, ZH_WORDS_GETTER("temperature.field.physicalCooling"), false);
    API_DTO_FIELD_REQUIRE(String,
                          tempAfterPhysicalCooling,
                          ZH_WORDS_GETTER("temperature.field.tempAfterPhysicalCooling"),
                          false);
    API_DTO_FIELD_REQUIRE(Boolean, drugCooling, ZH_WORDS_GETTER("temperature.field.drugCooling"), false);
    API_DTO_FIELD_REQUIRE(String,
                          tempAfterDrugCooling,
                          ZH_WORDS_GETTER("temperature.field.tempAfterDrugCooling"),
                          false);
    API_DTO_FIELD_REQUIRE(Boolean, tempNotRising, ZH_WORDS_GETTER("temperature.field.tempNotRising"), false);
    API_DTO_FIELD_REQUIRE(Boolean, discharge, ZH_WORDS_GETTER("temperature.field.discharge"), false);
    API_DTO_FIELD_REQUIRE(String, dischargeTime, ZH_WORDS_GETTER("temperature.field.dischargeTime"), false);
    API_DTO_FIELD_REQUIRE(String, specialEvent, ZH_WORDS_GETTER("temperature.field.specialEvent"), false);
    API_DTO_FIELD_REQUIRE(String, specialEventTime, ZH_WORDS_GETTER("temperature.field.specialEventTime"), false);
};

// ============================================================================
// ============================================================================

/**
 */
class TemperatureSupplementaryDTO : public oatpp::DTO
{
    DTO_INIT(TemperatureSupplementaryDTO, DTO);
    API_DTO_FIELD_REQUIRE(String, weight, ZH_WORDS_GETTER("temperature.field.weight"), false);
    API_DTO_FIELD_REQUIRE(String, urinationTimes, ZH_WORDS_GETTER("temperature.field.urinationTimes"), false);
    API_DTO_FIELD_REQUIRE(String, waist, ZH_WORDS_GETTER("temperature.field.waist"), false);
    API_DTO_FIELD_REQUIRE(String, defecationTimes, ZH_WORDS_GETTER("temperature.field.defecationTimes"), false);
    API_DTO_FIELD_REQUIRE(String, height, ZH_WORDS_GETTER("temperature.field.height"), false);
    API_DTO_FIELD_REQUIRE(String, intake, ZH_WORDS_GETTER("temperature.field.intake"), false);
    API_DTO_FIELD_REQUIRE(String, skinTestPositive, ZH_WORDS_GETTER("temperature.field.skinTestPositive"), false);
    API_DTO_FIELD_REQUIRE(String, urineVolume, ZH_WORDS_GETTER("temperature.field.urineVolume"), false);
    API_DTO_FIELD_REQUIRE(String, other, ZH_WORDS_GETTER("temperature.field.other"), false);
};

// ============================================================================
// ============================================================================

/**
 */
class TemperatureSaveDTO : public oatpp::DTO
{
    DTO_INIT(TemperatureSaveDTO, DTO);
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("temperature.field.patientId"), true);
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("temperature.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, recordedDate, ZH_WORDS_GETTER("temperature.field.recordedDate"), true);
    API_DTO_FIELD_REQUIRE(String, locationId, ZH_WORDS_GETTER("temperature.field.locationId"), false);
    API_DTO_FIELD_REQUIRE(String, recorderId, ZH_WORDS_GETTER("temperature.field.recorderId"), true);
    API_DTO_FIELD_REQUIRE(String, busNo, ZH_WORDS_GETTER("temperature.field.busNo"), false);
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("temperature.field.remark"), false);
    API_DTO_FIELD_REQUIRE(List<TemperatureTimeSlotDTO::Wrapper>, timeSlots,
                          ZH_WORDS_GETTER("temperature.field.timeSlots"), true);
    API_DTO_FIELD_REQUIRE(TemperatureSupplementaryDTO::Wrapper,
                          supplementary,
                          ZH_WORDS_GETTER("temperature.field.supplementary"),
                          false);
};

// ============================================================================
// ============================================================================

/**
 */
class TemperatureDetailDTO : public oatpp::DTO
{
    DTO_INIT(TemperatureDetailDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("temperature.field.id"), true);
    API_DTO_FIELD_REQUIRE(String, createdBy, ZH_WORDS_GETTER("temperature.field.createdBy"), true);
    API_DTO_FIELD_REQUIRE(String, createdAt, ZH_WORDS_GETTER("temperature.field.createdAt"), true);
    API_DTO_FIELD_REQUIRE(String, updatedBy, ZH_WORDS_GETTER("temperature.field.updatedBy"), false);
    API_DTO_FIELD_REQUIRE(String, updatedAt, ZH_WORDS_GETTER("temperature.field.updatedAt"), false);

    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("temperature.field.patientId"), true);
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("temperature.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, recordedDate, ZH_WORDS_GETTER("temperature.field.recordedDate"), true);
    API_DTO_FIELD_REQUIRE(String, typeEnum, ZH_WORDS_GETTER("temperature.field.typeEnum"), true);
    API_DTO_FIELD_REQUIRE(String, locationId, ZH_WORDS_GETTER("temperature.field.locationId"), false);
    API_DTO_FIELD_REQUIRE(String, recorderId, ZH_WORDS_GETTER("temperature.field.recorderId"), true);
    API_DTO_FIELD_REQUIRE(String, busNo, ZH_WORDS_GETTER("temperature.field.busNo"), false);
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("temperature.field.remark"), false);

    API_DTO_FIELD_REQUIRE(List<TemperatureTimeSlotDTO::Wrapper>, timeSlots,
                          ZH_WORDS_GETTER("temperature.field.timeSlots"), true);

    API_DTO_FIELD_REQUIRE(TemperatureSupplementaryDTO::Wrapper,
                          supplementary,
                          ZH_WORDS_GETTER("temperature.field.supplementary"),
                          false);

    API_DTO_FIELD_REQUIRE(String, patientName, ZH_WORDS_GETTER("temperature.field.patientName"), false);
    API_DTO_FIELD_REQUIRE(String, patientGender, ZH_WORDS_GETTER("temperature.field.patientGender"), false);
    API_DTO_FIELD_REQUIRE(Int32, patientAge, ZH_WORDS_GETTER("temperature.field.patientAge"), false);
    API_DTO_FIELD_REQUIRE(String, ward, ZH_WORDS_GETTER("temperature.field.ward"), false);
    API_DTO_FIELD_REQUIRE(String, bedNo, ZH_WORDS_GETTER("temperature.field.bedNo"), false);
    API_DTO_FIELD_REQUIRE(String, medicalRecordNo, ZH_WORDS_GETTER("temperature.field.medicalRecordNo"), false);
    API_DTO_FIELD_REQUIRE(String, admissionTime, ZH_WORDS_GETTER("temperature.field.admissionTime"), false);
    API_DTO_FIELD_REQUIRE(String, dischargeTime, ZH_WORDS_GETTER("temperature.field.dischargeTime"), false);
};

    #include OATPP_CODEGEN_END(DTO)
#endif  // !_TEMPERATURE_DTO_
