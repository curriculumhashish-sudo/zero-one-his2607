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
#include "domain/dto/medicalorderexecution/MedicalOrderItemDTO.h"

/**
 * Response VO types for medical order execution.
 *
 * All responses follow the unified envelope:
 *   { "code": 10000, "message": "success", "data": <payload> }
 *
 * Paginated response example:
 *   {
 *     "code": 10000,
 *     "message": "success",
 *     "data": {
 *       "pageIndex": 1,
 *       "pageSize": 10,
 *       "total": 25,
 *       "pages": 3,
 *       "rows": [
 *         {
 *           "id": "sr-001",
 *           "busNo": "BUS-20240101-0001",
 *           "prescriptionNo": "PRS-001",
 *           "statusEnum": "pending_verification",
 *           "categoryEnum": "medication",
 *           "intentEnum": "long_term",
 *           "priorityEnum": "routine",
 *           "patientId": "P-001",
 *           "encounterId": "ENC-001",
 *           "authoredTime": "2024-01-01 08:00:00",
 *           "quantity": 2.0,
 *           "unitCode": "tab",
 *           "requesterId": "D-001",
 *           "performerId": "N-001",
 *           "performerCheckId": null,
 *           "checkTime": null,
 *           "locationId": "WARD-01",
 *           "executionCount": 0,
 *           "remark": "take after meals",
 *           "entrustContent": null,
 *           "reasonText": null
 *         }
 *       ]
 *     }
 *   }
 */

// Paginated response: JsonVO<PageDTO<MedicalOrderItemDTO>>
typedef JsonVO<PageDTO<MedicalOrderItemDTO::Wrapper>::Wrapper> MedicalOrderPageJsonVO;

// Single item response: JsonVO<MedicalOrderItemDTO>
typedef JsonVO<MedicalOrderItemDTO::Wrapper> MedicalOrderDetailJsonVO;

#endif // !_MEDICAL_ORDER_VO_H_
