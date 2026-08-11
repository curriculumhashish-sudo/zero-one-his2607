/*
 Copyright Zero One Star. All rights reserved.

 Defines request data used to sign TCM prescriptions.

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

// Prevents duplicate DTO declarations when this header is included repeatedly.
#ifndef _TCM_PRESCRIPTION_SIGN_DTO_H_
#define _TCM_PRESCRIPTION_SIGN_DTO_H_

#include "../../GlobalInclude.h"

// Starts Oat++ DTO code generation.
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Define one prescription signing target.
 */
class TcmPrescriptionSignTargetDTO : public oatpp::DTO
{
    DTO_INIT(TcmPrescriptionSignTargetDTO, DTO);

    // Identify the temporary prescription group stored in content_json.
    API_DTO_FIELD(String, prescriptionGroupKey,
        ZH_WORDS_GETTER("tcm-prescription.field.prescription-group-key"),
        true, nullptr);

    // Identify the medication request to sign.
    API_DTO_FIELD(String, medicationRequestId,
        ZH_WORDS_GETTER("tcm-prescription.field.medication-request-id"),
        true, nullptr);

public:
    std::string validate()
    {
        if (!prescriptionGroupKey ||
            prescriptionGroupKey->find_first_not_of(" \t\r\n") == std::string::npos)
            return "prescriptionGroupKey is required.";

        if (prescriptionGroupKey->size() > 255)
            return "prescriptionGroupKey is too long.";

        if (!medicationRequestId ||
            medicationRequestId->find_first_not_of(" \t\r\n") == std::string::npos)
            return "medicationRequestId is required.";

        if (medicationRequestId->size() > 64)
            return "medicationRequestId is too long.";

        return "";
    }
};


/**
 * Define the traditional Chinese medicine prescription signing request.
 */
class TcmPrescriptionSignDTO : public oatpp::DTO
{
    DTO_INIT(TcmPrescriptionSignDTO, DTO);

    // Identify the inpatient encounter.
    API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("tcm-prescription.field.encounter-id"), true, nullptr);

    // Identify the patient for prescription signing.
    API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("tcm-prescription.field.patient-id"), true, nullptr);

    // Contain all medication requests to sign.
    API_DTO_FIELD(List<TcmPrescriptionSignTargetDTO::Wrapper>, signTargets,
        ZH_WORDS_GETTER("tcm-prescription.field.sign-targets"), true, {});

public:
    std::string validate()
    {
        if (!encounterId ||
            encounterId->find_first_not_of(" \t\r\n") == std::string::npos)
            return "encounterId is required.";

        if (encounterId->size() > 64)
            return "encounterId is too long.";

        if (!patientId ||
            patientId->find_first_not_of(" \t\r\n") == std::string::npos)
            return "patientId is required.";

        if (patientId->size() > 64)
            return "patientId is too long.";

        if (!signTargets || signTargets->empty())
            return "signTargets are required.";

        for (const auto& target : *signTargets)
        {
            if (!target)
                return "signTargets cannot contain null values.";

            const std::string targetError = target->validate();
            if (!targetError.empty())
                return targetError;
        }

        return "";
    }
};

// Ends Oat++ DTO code generation.
#include OATPP_CODEGEN_END(DTO)

// Ends the signing request DTO include guard.
#endif // !_TCM_PRESCRIPTION_SIGN_DTO_H_
