/*
 Copyright (c) 2025-2066, Zero One Technology Co., Ltd.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#pragma once

// Prevents duplicate withdrawal DTO declarations.
#ifndef _TCM_PRESCRIPTION_WITHDRAW_DTO_H_
#define _TCM_PRESCRIPTION_WITHDRAW_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Carries and validates a TCM prescription-withdrawal request.
 */
class TcmPrescriptionWithdrawDTO : public oatpp::DTO
{
    DTO_INIT(TcmPrescriptionWithdrawDTO, DTO);
    // Encounter identifier.
    API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("tcm-prescription.field.encounter-id"), true, nullptr);

    // Patient identifier.
    API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("tcm-prescription.field.patient-id"), true, nullptr);

    // Requested action, which must be WITHDRAW.
    API_DTO_FIELD(String, action, ZH_WORDS_GETTER("tcm-prescription.field.action"), true, nullptr);

    // Medication request identifiers included in the withdrawal.
    API_DTO_FIELD(List<String>, medicationRequestIds, ZH_WORDS_GETTER("tcm-prescription.field.medication-request-ids"),
                  true, {});

    // Prescription group key parsed from the medication request content.
    API_DTO_FIELD(String, prescriptionGroupKey, ZH_WORDS_GETTER("tcm-prescription.field.prescription-group-key"), true,
                  nullptr);

    // Prescription number shared by the requested detail records.
    API_DTO_FIELD(String, prescriptionNo, ZH_WORDS_GETTER("tcm-prescription.field.prescription-no"), true, nullptr);

    // Requested scope, which must be PRESCRIPTION.
    API_DTO_FIELD(String, scope, ZH_WORDS_GETTER("tcm-prescription.field.scope"), true, nullptr);

  public:
    // Returns an empty string when all structural request rules are satisfied.
    std::string validate()
    {
        if (!encounterId || encounterId->empty())
            return "encounterId is required.";

        if (!patientId || patientId->empty())
            return "patientId is required.";

        if (!action || action->compare("WITHDRAW") != 0)
            return "action must be WITHDRAW.";

        if (!medicationRequestIds || medicationRequestIds->empty())
            return "medicationRequestIds are required.";

        for (const auto &requestId : *medicationRequestIds)
        {
            if (!requestId || requestId->empty())
                return "medicationRequestIds cannot contain empty values.";
        }

        if (!prescriptionGroupKey || prescriptionGroupKey->empty())
            return "prescriptionGroupKey is required.";

        if (!prescriptionNo || prescriptionNo->empty())
            return "prescriptionNo is required.";

        if (!scope || scope->compare("PRESCRIPTION") != 0)
            return "scope must be PRESCRIPTION.";

        return "";
    }
};

#include OATPP_CODEGEN_END(DTO)

// Ends the withdrawal DTO include guard.
#endif // !_TCM_PRESCRIPTION_WITHDRAW_DTO_H_
