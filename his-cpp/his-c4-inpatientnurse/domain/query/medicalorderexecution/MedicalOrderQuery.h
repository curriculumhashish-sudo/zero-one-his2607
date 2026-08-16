#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
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

#include "../../GlobalInclude.h"
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

    // Workflow status filter (e.g., "pending_verification", "verified", "executing", "completed")
    API_DTO_FIELD_REQUIRE(String, statusEnum, "status filter: pending_verification | verified | returned | executing | completed", false);

    // Patient ID filter
    API_DTO_FIELD_REQUIRE(String, patientId, "patient identifier, e.g. P-20240101-0001", false);

    // Encounter (visit) ID filter
    API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("medical-order-execution.field.encounterId"), true);

    // Start date filter (authored_time >= startDate)
    API_DTO_FIELD_REQUIRE(String, startDate, "order authored start date, format: yyyy-MM-dd HH:mm:ss", false);

    // End date filter (authored_time <= endDate)
    API_DTO_FIELD_REQUIRE(String, endDate, "order authored end date, format: yyyy-MM-dd HH:mm:ss", false);

    // Department / location filter
    API_DTO_FIELD_REQUIRE(String, locationId, "department/location identifier, e.g. WARD-01", false);

    // Category filter (e.g., medication, procedure, lab)
    API_DTO_FIELD_REQUIRE(String, categoryEnum, "category filter: medication | procedure | lab", false);

    // Priority filter (e.g., routine, urgent, stat)
    API_DTO_FIELD_REQUIRE(String, priorityEnum, "priority filter: routine | urgent | stat", false);

    // Order type filter (long_term / temporary)
    API_DTO_FIELD_REQUIRE(String, intentEnum, "order type filter: long_term | temporary", false);

	API_DTO_FIELD_REQUIRE(String, therapyEnum,
		ZH_WORDS_GETTER("medical-order-execution.field.therapyEnum"), false);

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_QUERY_H_
