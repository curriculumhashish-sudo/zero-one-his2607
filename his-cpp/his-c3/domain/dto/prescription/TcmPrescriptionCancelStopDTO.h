/*
 Copyright Zero One Star. All rights reserved.

 Traditional Chinese medicine cancel-stop request DTO

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

// Prevents this header from being included more than once.
#ifndef _TCM_PRESCRIPTION_CANCEL_STOP_DTO_H_
#define _TCM_PRESCRIPTION_CANCEL_STOP_DTO_H_

// Provides project-wide definitions and DTO macros.
#include "../../GlobalInclude.h"

// Begins Oatpp DTO code generation.
#include OATPP_CODEGEN_BEGIN(DTO)

// Carries the request parameters used to cancel prescription stops.
class TcmPrescriptionCancelStopDTO : public oatpp::DTO
{
    // Initializes Oatpp metadata for this DTO type.
    DTO_INIT(TcmPrescriptionCancelStopDTO, DTO);

    // Identifies the current inpatient encounter.
    API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("tcm-prescription.field.encounter-id"), true, nullptr);

    // Identifies the current patient.
    API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("tcm-prescription.field.patient-id"), true, nullptr);

    // Lists the prescription numbers whose stops should be canceled.
    API_DTO_FIELD(List<String>, prescriptionNos, ZH_WORDS_GETTER("tcm-prescription.field.prescription-nos"), true, {});

public:
    // Returns an empty string when all structural request rules are satisfied.
    std::string validate()
    {
        if (!encounterId || encounterId->empty())
            return "encounterId is required.";

        if (!patientId || patientId->empty())
            return "patientId is required.";

        if (!prescriptionNos || prescriptionNos->empty())
            return "prescriptionNos are required.";

        for (const auto& prescriptionNo : *prescriptionNos)
        {
            if (!prescriptionNo || prescriptionNo->empty())
                return "prescriptionNos cannot contain empty values.";
        }

        return "";
    }
};

// Ends Oatpp DTO code generation.
#include OATPP_CODEGEN_END(DTO)

// Ends the header guard.
#endif // _TCM_PRESCRIPTION_CANCEL_STOP_DTO_H_
