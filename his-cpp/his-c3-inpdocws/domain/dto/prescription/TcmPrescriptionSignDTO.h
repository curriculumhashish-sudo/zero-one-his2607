/*
 Copyright Zero One Star. All rights reserved.

 签发中药处方请求 DTO

 @Author: nick1484065161
 @Date: 2026/07/20 00:00:00

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

#pragma once

#ifndef _TCM_PRESCRIPTION_SIGN_DTO_H_
#define _TCM_PRESCRIPTION_SIGN_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/**
 * Define one prescription signing target.
 */
 
class TcmPrescriptionSignTargetDTO : public oatpp::DTO
{
    DTO_INIT(TcmPrescriptionSignTargetDTO, DTO);
        
    API_DTO_FIELD(String, prescriptionGroupKey, ZH_WORDS_GETTER("tcm-prescription.field.prescription-group-key"),true,"tcm-rx-1782719488981-gejhfw");
    // Identify the medication request to sign.
    API_DTO_FIELD(String,medicationRequestId,ZH_WORDS_GETTER("tcm-prescription.field.medication-request-id"),true,"329891761734291456");
};


/**
  * Define the traditional Chinese medicine prescription signing request.
  */
class TcmPrescriptionSignDTO : public oatpp::DTO
{
    DTO_INIT(TcmPrescriptionSignDTO, DTO);
    // Identify the inpatient encounter.
    API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("tcm-prescription.field.encounter-id"), true, "329874660147597312");
    // Identify the patient for prescription signing.
    API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("tcm-prescription.field.patient-id"), true, "326940849401761792");
    // Contain all medication requests to sign.
    API_DTO_FIELD(List<TcmPrescriptionSignTargetDTO::Wrapper>,signTargets,ZH_WORDS_GETTER("tcm-prescription.field.sign-targets"),true,{});

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TCM_PRESCRIPTION_SIGN_DTO_H_
