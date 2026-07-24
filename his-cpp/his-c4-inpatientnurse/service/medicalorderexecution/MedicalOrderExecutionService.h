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
#ifndef _MEDICAL_ORDER_EXECUTION_SERVICE_H_
#define _MEDICAL_ORDER_EXECUTION_SERVICE_H_

#include "domain/vo/medicalorderexecution/MedicalOrderVO.h"
#include "domain/query/medicalorderexecution/MedicalOrderQuery.h"
#include "domain/dto/medicalorderexecution/MedicalOrderExecutionDTO.h"
#include "domain/vo/BaseJsonVO.h"

/**
 * Service layer for medical order execution and verification.
 * Core workflow: doctor orders -> nurse verifies -> nurse executes -> complete.
 *
 * Six requirements:
 *   1. Get verification items (query with filters + pagination)
 *   2. Verify pass (status: pending_verification -> verified)
 *   3. Return order  (status: pending_verification -> returned, with reason)
 *   4. Get execution items (query with filters + pagination)
 *   5. Start execution (status: verified -> executing)
 *   6. Complete execution (status: executing -> completed)
 */
class MedicalOrderExecutionService
{
public:
    MedicalOrderExecutionService() = default;
    ~MedicalOrderExecutionService() = default;

    /**
     * Query orders awaiting verification.
     * Defaults to status_enum = 'pending_verification' if not specified.
     */
    MedicalOrderPageJsonVO::Wrapper queryVerificationItems(const MedicalOrderQuery::Wrapper& query);

    /**
     * Approve an order after verification.
     * Advances status from pending_verification to verified.
     */
    StringJsonVO::Wrapper execVerifyPass(const VerifyPassDTO::Wrapper& dto);

    /**
     * Reject an order back to the doctor with a reason.
     * Status changes to 'returned', reason is recorded.
     */
    StringJsonVO::Wrapper execReturnOrder(const ReturnOrderDTO::Wrapper& dto);

    /**
     * Query orders ready for execution.
     * Defaults to status_enum = 'verified' if not specified.
     */
    MedicalOrderPageJsonVO::Wrapper queryExecutionItems(const MedicalOrderQuery::Wrapper& query);

    /**
     * Start executing an order.
     * Status transitions from verified to executing.
     * Records the performer and increments execution count.
     */
    StringJsonVO::Wrapper execStartExecution(const ExecuteOrderDTO::Wrapper& dto);

    /**
     * Complete execution of an order.
     * Status transitions from executing to completed.
     */
    StringJsonVO::Wrapper execCompleteExecution(const CompleteOrderDTO::Wrapper& dto);
};

#endif // !_MEDICAL_ORDER_EXECUTION_SERVICE_H_
