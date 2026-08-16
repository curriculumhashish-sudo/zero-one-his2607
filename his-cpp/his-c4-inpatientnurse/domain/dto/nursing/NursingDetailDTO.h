#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/19 10:00:00

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
#ifndef _NURSING_DETAIL_DTO_
    #define _NURSING_DETAIL_DTO_
    #include "../../GlobalInclude.h"

    #include OATPP_CODEGEN_BEGIN(DTO)

// ============================================================================
// ============================================================================

/**
 *
 */
class NursingDetailDTO : public oatpp::DTO
{
    DTO_INIT(NursingDetailDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("nursing.field.id"), true);
    API_DTO_FIELD_REQUIRE(String, createdBy, ZH_WORDS_GETTER("nursing.field.createdBy"), true);
    API_DTO_FIELD_REQUIRE(String, createdAt, ZH_WORDS_GETTER("nursing.field.createdAt"), true);
    API_DTO_FIELD_REQUIRE(String, updatedBy, ZH_WORDS_GETTER("nursing.field.updatedBy"), false);
    API_DTO_FIELD_REQUIRE(String, updatedAt, ZH_WORDS_GETTER("nursing.field.updatedAt"), false);

    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("nursing.field.patientId"), true);
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("nursing.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, recordedAt, ZH_WORDS_GETTER("nursing.field.recordedAt"), true);
    API_DTO_FIELD_REQUIRE(String, typeEnum, ZH_WORDS_GETTER("nursing.field.typeEnum"), true);
    API_DTO_FIELD_REQUIRE(String, busNo, ZH_WORDS_GETTER("nursing.field.busNo"), true);
    API_DTO_FIELD_REQUIRE(String, locationId, ZH_WORDS_GETTER("nursing.field.locationId"), false);
    API_DTO_FIELD_REQUIRE(Int32, babyNo, ZH_WORDS_GETTER("nursing.field.babyNo"), false);
    API_DTO_FIELD_REQUIRE(String, recorderId, ZH_WORDS_GETTER("nursing.field.recorderId"), true);
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("nursing.field.remark"), false);

    API_DTO_FIELD_REQUIRE(String, temperature, ZH_WORDS_GETTER("nursing.field.temperature"), false);
    API_DTO_FIELD_REQUIRE(String, pulse, ZH_WORDS_GETTER("nursing.field.pulse"), false);
    API_DTO_FIELD_REQUIRE(String, heartRate, ZH_WORDS_GETTER("nursing.field.heartRate"), false);
    API_DTO_FIELD_REQUIRE(String, respiration, ZH_WORDS_GETTER("nursing.field.respiration"), false);
    API_DTO_FIELD_REQUIRE(String, bloodPressure, ZH_WORDS_GETTER("nursing.field.bloodPressure"), false);
    API_DTO_FIELD_REQUIRE(String, consciousness, ZH_WORDS_GETTER("nursing.field.consciousness"), false);
    API_DTO_FIELD_REQUIRE(String, pupilLeft, ZH_WORDS_GETTER("nursing.field.pupilLeft"), false);
    API_DTO_FIELD_REQUIRE(String, pupilRight, ZH_WORDS_GETTER("nursing.field.pupilRight"), false);
    API_DTO_FIELD_REQUIRE(String, intakeName, ZH_WORDS_GETTER("nursing.field.intakeName"), false);
    API_DTO_FIELD_REQUIRE(String, intakeVolume, ZH_WORDS_GETTER("nursing.field.intakeVolume"), false);
    API_DTO_FIELD_REQUIRE(String, outputName, ZH_WORDS_GETTER("nursing.field.outputName"), false);
    API_DTO_FIELD_REQUIRE(String, outputVolume, ZH_WORDS_GETTER("nursing.field.outputVolume"), false);
    API_DTO_FIELD_REQUIRE(String, outputColor, ZH_WORDS_GETTER("nursing.field.outputColor"), false);
    API_DTO_FIELD_REQUIRE(String, observation, ZH_WORDS_GETTER("nursing.field.observation"), false);
    API_DTO_FIELD_REQUIRE(String, nurseSignature, ZH_WORDS_GETTER("nursing.field.nurseSignature"), false);

    API_DTO_FIELD_REQUIRE(String, patientName, ZH_WORDS_GETTER("nursing.field.patientName"), false);
    API_DTO_FIELD_REQUIRE(String, patientGender, ZH_WORDS_GETTER("nursing.field.patientGender"), false);
    API_DTO_FIELD_REQUIRE(Int32, patientAge, ZH_WORDS_GETTER("nursing.field.patientAge"), false);
    API_DTO_FIELD_REQUIRE(String, ward, ZH_WORDS_GETTER("nursing.field.ward"), false);
    API_DTO_FIELD_REQUIRE(String, bedNo, ZH_WORDS_GETTER("nursing.field.bedNo"), false);
    API_DTO_FIELD_REQUIRE(String, medicalRecordNo, ZH_WORDS_GETTER("nursing.field.medicalRecordNo"), false);
    API_DTO_FIELD_REQUIRE(String, admissionTime, ZH_WORDS_GETTER("nursing.field.admissionTime"), false);
    API_DTO_FIELD_REQUIRE(String, dischargeTime, ZH_WORDS_GETTER("nursing.field.dischargeTime"), false);
};

/**
 */
class NursingPageDTO : public PageDTO<NursingDetailDTO::Wrapper>
{
    DTO_INIT(NursingPageDTO, PageDTO<NursingDetailDTO::Wrapper>);
};

    #include OATPP_CODEGEN_END(DTO)
#endif  // !_NURSING_DETAIL_DTO_
