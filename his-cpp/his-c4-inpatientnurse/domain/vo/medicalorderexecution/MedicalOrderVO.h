#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: fanpai
 @Date: 2026/07/20 16:00:00

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
#ifndef _MEDICAL_ORDER_VO_H_
#define _MEDICAL_ORDER_VO_H_

#include "domain/vo/JsonVO.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * View Object: single medical order item for verification and execution list rows.
 * Contains only the fields needed by the frontend (subset of wkf_service_request columns).
 */
class MedicalOrderItemVO : public oatpp::DTO
{
    DTO_INIT(MedicalOrderItemVO, DTO);

    // Basic info
    DTO_FIELD(String, id, "id");
    DTO_FIELD_INFO(id) {
        info->description = "service request ID";
    }

    DTO_FIELD(String, busNo, "busNo");
    DTO_FIELD_INFO(busNo) {
        info->description = "business number";
    }

    DTO_FIELD(String, prescriptionNo, "prescriptionNo");
    DTO_FIELD_INFO(prescriptionNo) {
        info->description = "prescription number";
    }

    // Status
    DTO_FIELD(String, statusEnum, "statusEnum");
    DTO_FIELD_INFO(statusEnum) {
        info->description = "current workflow status";
    }

    DTO_FIELD(String, categoryEnum, "categoryEnum");
    DTO_FIELD_INFO(categoryEnum) {
        info->description = "order category";
    }

    DTO_FIELD(String, intentEnum, "intentEnum");
    DTO_FIELD_INFO(intentEnum) {
        info->description = "order type (long_term/temporary)";
    }

    DTO_FIELD(String, priorityEnum, "priorityEnum");
    DTO_FIELD_INFO(priorityEnum) {
        info->description = "order priority";
    }

    // Patient
    DTO_FIELD(String, patientId, "patientId");
    DTO_FIELD_INFO(patientId) {
        info->description = "patient identifier";
    }

    DTO_FIELD(String, encounterId, "encounterId");
    DTO_FIELD_INFO(encounterId) {
        info->description = "encounter identifier";
    }

    // Time
    DTO_FIELD(String, authoredTime, "authoredTime");
    DTO_FIELD_INFO(authoredTime) {
        info->description = "order creation time";
    }

    // Quantity
    DTO_FIELD(Float64, quantity, "quantity");
    DTO_FIELD_INFO(quantity) {
        info->description = "quantity";
    }

    DTO_FIELD(String, unitCode, "unitCode");
    DTO_FIELD_INFO(unitCode) {
        info->description = "unit code";
    }

    // Personnel
    DTO_FIELD(String, requesterId, "requesterId");
    DTO_FIELD_INFO(requesterId) {
        info->description = "requester (doctor) identifier";
    }

    DTO_FIELD(String, performerId, "performerId");
    DTO_FIELD_INFO(performerId) {
        info->description = "performer (nurse) identifier";
    }

    DTO_FIELD(String, performerCheckId, "performerCheckId");
    DTO_FIELD_INFO(performerCheckId) {
        info->description = "verifier identifier";
    }

    DTO_FIELD(String, checkTime, "checkTime");
    DTO_FIELD_INFO(checkTime) {
        info->description = "verification timestamp";
    }

    // Location
    DTO_FIELD(String, locationId, "locationId");
    DTO_FIELD_INFO(locationId) {
        info->description = "department/location identifier";
    }

    // Execution
    DTO_FIELD(Int32, executionCount, "executionCount");
    DTO_FIELD_INFO(executionCount) {
        info->description = "execution count";
    }

    // Notes
    DTO_FIELD(String, remark, "remark");
    DTO_FIELD_INFO(remark) {
        info->description = "remark/notes";
    }

    DTO_FIELD(String, entrustContent, "entrustContent");
    DTO_FIELD_INFO(entrustContent) {
        info->description = "entrust content / nurse instructions";
    }

    DTO_FIELD(String, reasonText, "reasonText");
    DTO_FIELD_INFO(reasonText) {
        info->description = "return/rejection reason";
    }
};

// Response type aliases
// Paginated list: { code, message, data: { pageIndex, pageSize, total, pages, rows: [...] } }
typedef JsonVO<PageDTO<MedicalOrderItemVO::Wrapper>::Wrapper> MedicalOrderPageJsonVO;

// Single item detail: { code, message, data: { ... } }
typedef JsonVO<MedicalOrderItemVO::Wrapper> MedicalOrderDetailJsonVO;

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_VO_H_
