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
#ifndef _MEDICAL_ORDER_VO_H_
#define _MEDICAL_ORDER_VO_H_

#include "domain/vo/JsonVO.h"
#include "domain/dto/PageDTO.h"
#include "domain/dto/medicalorderexecution/MedicalOrderItemDTO.h"
#include "domain/dto/medicalorderexecution/MedicalOrderExecutionDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

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

class MedicalOrderPageJsonVO : public JsonVO<PageDTO<MedicalOrderItemDTO::Wrapper>::Wrapper>
{
    DTO_INIT(MedicalOrderPageJsonVO,
        JsonVO<PageDTO<MedicalOrderItemDTO::Wrapper>::Wrapper>);

public:
    MedicalOrderPageJsonVO()
    {
        auto page = PageDTO<MedicalOrderItemDTO::Wrapper>::createShared();
        page->pageIndex = 1;
        page->pageSize = 10;
		page->total = static_cast<v_int64>(0);
		page->pages = static_cast<v_int64>(0);
		page->rows = List<MedicalOrderItemDTO::Wrapper>::createShared();
        data = page;
    }
};

class OrderExecutionWorkbenchJsonVO : public JsonVO<OrderExecutionWorkbenchDTO::Wrapper>
{
	DTO_INIT(OrderExecutionWorkbenchJsonVO,
		JsonVO<OrderExecutionWorkbenchDTO::Wrapper>);

public:
	OrderExecutionWorkbenchJsonVO()
	{
		data = OrderExecutionWorkbenchDTO::createShared();
	}
};

class BatchOrderExecutionJsonVO : public JsonVO<BatchOrderExecutionResponseDTO::Wrapper>
{
	DTO_INIT(BatchOrderExecutionJsonVO,
		JsonVO<BatchOrderExecutionResponseDTO::Wrapper>);

public:
	BatchOrderExecutionJsonVO()
	{
		data = BatchOrderExecutionResponseDTO::createShared();
	}
};

// Single item response: JsonVO<MedicalOrderItemDTO>
typedef JsonVO<MedicalOrderItemDTO::Wrapper> MedicalOrderDetailJsonVO;

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_VO_H_
