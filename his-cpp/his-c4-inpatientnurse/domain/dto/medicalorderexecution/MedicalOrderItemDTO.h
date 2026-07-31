#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/24 16:00:00

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
#ifndef _MEDICAL_ORDER_ITEM_DTO_H_
#define _MEDICAL_ORDER_ITEM_DTO_H_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * DTO: single medical order item for verification and execution list rows.
 * Contains the fields needed by the frontend (subset of wkf_service_request columns).
 */
class MedicalOrderItemDTO : public oatpp::DTO
{
    DTO_INIT(MedicalOrderItemDTO, DTO);

    // ============================================================
    // Basic info
    // ============================================================

    // Service request primary key
    API_DTO_FIELD_DEFAULT(String, id, "service request ID");

    // Business number
    API_DTO_FIELD_DEFAULT(String, busNo, "business number");

    // Prescription number
    API_DTO_FIELD_DEFAULT(String, prescriptionNo, "prescription number");

    // ============================================================
    // Status fields
    // ============================================================

    // Workflow status: pending_verification / verified / returned / executing / completed
    API_DTO_FIELD_DEFAULT(String, statusEnum, "current workflow status");

    // Order category: medication / procedure / lab
    API_DTO_FIELD_DEFAULT(String, categoryEnum, "order category");

    // Order type: long_term / temporary
    API_DTO_FIELD_DEFAULT(String, intentEnum, "order type (long_term/temporary)");

    // Order priority
    API_DTO_FIELD_DEFAULT(String, priorityEnum, "order priority");

    // ============================================================
    // Patient info
    // ============================================================

    // Patient identifier
    API_DTO_FIELD_DEFAULT(String, patientId, "patient identifier");

    // Encounter (visit) identifier
    API_DTO_FIELD_DEFAULT(String, encounterId, "encounter identifier");

    // ============================================================
    // Time
    // ============================================================

    // Order creation time
    API_DTO_FIELD_DEFAULT(String, authoredTime, "order creation time");

    // ============================================================
    // Quantity
    // ============================================================

    // Order quantity
    API_DTO_FIELD_DEFAULT(Float64, quantity, "quantity");

    // Unit code
    API_DTO_FIELD_DEFAULT(String, unitCode, "unit code");

    // ============================================================
    // Personnel
    // ============================================================

    // Requester (doctor) identifier
    API_DTO_FIELD_DEFAULT(String, requesterId, "requester (doctor) identifier");

    // Performer (nurse) identifier
    API_DTO_FIELD_DEFAULT(String, performerId, "performer (nurse) identifier");

    // Verifier identifier
    API_DTO_FIELD_DEFAULT(String, performerCheckId, "verifier identifier");

    // Verification timestamp
    API_DTO_FIELD_DEFAULT(String, checkTime, "verification timestamp");

    // ============================================================
    // Location
    // ============================================================

    // Department / location identifier
    API_DTO_FIELD_DEFAULT(String, locationId, "department/location identifier");

    // ============================================================
    // Execution
    // ============================================================

    // Execution count
    API_DTO_FIELD_DEFAULT(Int32, executionCount, "execution count");

    // ============================================================
    // Notes
    // ============================================================

    // Remark / notes
    API_DTO_FIELD_DEFAULT(String, remark, "remark/notes");

    // Entrust content / nurse instructions
    API_DTO_FIELD_DEFAULT(String, entrustContent, "entrust content / nurse instructions");

    // Return/rejection reason text
    API_DTO_FIELD_DEFAULT(String, reasonText, "return/rejection reason");

public:
    MedicalOrderItemDTO()
    {
        id = "337907884548657152";
        busNo = "MO202607240001";
        prescriptionNo = "RX202607240001";
        statusEnum = "verified";
        categoryEnum = "medication";
        intentEnum = "temporary";
        priorityEnum = "routine";
        patientId = "326940849401761792";
        encounterId = "337907884548657152";
        authoredTime = "2026-07-24 08:00:00";
        quantity = 1.0;
        unitCode = "tablet";
        requesterId = "DOCTOR001";
        performerId = "NURSE001";
        locationId = "334526752078102528";
        executionCount = 0;
        remark = ZH_WORDS_GETTER("business-text.sample.medication-remark");
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_ITEM_DTO_H_
