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
#ifndef _MEDICAL_ORDER_QUERY_H_
#define _MEDICAL_ORDER_QUERY_H_

#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Query object for filtering medical orders in verification and execution lists.
 * Extends PageQuery to inherit pagination fields (pageIndex, pageSize).
 * All filter fields are optional -- only populated filters affect the query.
 */
class MedicalOrderQuery : public PageQuery
{
    DTO_INIT(MedicalOrderQuery, PageQuery);

    // Workflow status filter (e.g., "pending_verification", "verified")
    DTO_FIELD(String, statusEnum, "statusEnum");
    DTO_FIELD_INFO(statusEnum) {
        info->description = "status enum filter";
    }

    // Patient ID filter
    DTO_FIELD(String, patientId, "patientId");
    DTO_FIELD_INFO(patientId) {
        info->description = "patient identifier";
    }

    // Encounter (visit) ID filter
    DTO_FIELD(String, encounterId, "encounterId");
    DTO_FIELD_INFO(encounterId) {
        info->description = "encounter identifier";
    }

    // Start date filter (authored_time >= startDate)
    DTO_FIELD(String, startDate, "startDate");
    DTO_FIELD_INFO(startDate) {
        info->description = "order authored start date";
    }

    // End date filter (authored_time <= endDate)
    DTO_FIELD(String, endDate, "endDate");
    DTO_FIELD_INFO(endDate) {
        info->description = "order authored end date";
    }

    // Department / location filter
    DTO_FIELD(String, locationId, "locationId");
    DTO_FIELD_INFO(locationId) {
        info->description = "department/location identifier";
    }

    // Category filter (e.g., medication, procedure, lab)
    DTO_FIELD(String, categoryEnum, "categoryEnum");
    DTO_FIELD_INFO(categoryEnum) {
        info->description = "category enum filter";
    }

    // Priority filter
    DTO_FIELD(String, priorityEnum, "priorityEnum");
    DTO_FIELD_INFO(priorityEnum) {
        info->description = "priority enum filter";
    }

    // Order type filter (long_term / temporary)
    DTO_FIELD(String, intentEnum, "intentEnum");
    DTO_FIELD_INFO(intentEnum) {
        info->description = "intent enum filter (order type)";
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_QUERY_H_
