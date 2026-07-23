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
#ifndef _MEDICAL_ORDER_EXECUTION_DTO_H_
#define _MEDICAL_ORDER_EXECUTION_DTO_H_

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * DTO for the "Verify Pass" operation.
 * Request body: POST /medical-order/verify-pass
 * JSON: { "serviceRequestId": "...", "verifierId": "..." }
 */
class VerifyPassDTO : public oatpp::DTO
{
    DTO_INIT(VerifyPassDTO, DTO);

    // ID of the order to verify (required)
    DTO_FIELD(String, serviceRequestId, "serviceRequestId");
    DTO_FIELD_INFO(serviceRequestId) {
        info->description = "service request ID to verify";
        info->required = true;
    }

    // ID of the nurse performing verification (required)
    DTO_FIELD(String, verifierId, "verifierId");
    DTO_FIELD_INFO(verifierId) {
        info->description = "verifier (nurse) identifier";
        info->required = true;
    }
};

/**
 * DTO for the "Return Order" operation.
 * Request body: POST /medical-order/return-order
 * JSON: { "serviceRequestId": "...", "returnReason": "..." }
 */
class ReturnOrderDTO : public oatpp::DTO
{
    DTO_INIT(ReturnOrderDTO, DTO);

    // ID of the order to return (required)
    DTO_FIELD(String, serviceRequestId, "serviceRequestId");
    DTO_FIELD_INFO(serviceRequestId) {
        info->description = "service request ID to return";
        info->required = true;
    }

    // Reason for rejection (required)
    DTO_FIELD(String, returnReason, "returnReason");
    DTO_FIELD_INFO(returnReason) {
        info->description = "reason for returning the order";
        info->required = true;
    }
};

/**
 * DTO for the "Start Execution" operation.
 * Request body: POST /medical-order/execute-order
 * JSON: { "serviceRequestId": "...", "performerId": "..." }
 */
class ExecuteOrderDTO : public oatpp::DTO
{
    DTO_INIT(ExecuteOrderDTO, DTO);

    // ID of the order to execute (required)
    DTO_FIELD(String, serviceRequestId, "serviceRequestId");
    DTO_FIELD_INFO(serviceRequestId) {
        info->description = "service request ID to execute";
        info->required = true;
    }

    // ID of the nurse performing execution (required)
    DTO_FIELD(String, performerId, "performerId");
    DTO_FIELD_INFO(performerId) {
        info->description = "performer (nurse) identifier";
        info->required = true;
    }
};

/**
 * DTO for the "Complete Execution" operation.
 * Request body: POST /medical-order/complete-order
 * JSON: { "serviceRequestId": "...", "performerId": "..." }
 */
class CompleteOrderDTO : public oatpp::DTO
{
    DTO_INIT(CompleteOrderDTO, DTO);

    // ID of the order to complete (required)
    DTO_FIELD(String, serviceRequestId, "serviceRequestId");
    DTO_FIELD_INFO(serviceRequestId) {
        info->description = "service request ID to complete";
        info->required = true;
    }

    // ID of the nurse completing execution (required)
    DTO_FIELD(String, performerId, "performerId");
    DTO_FIELD_INFO(performerId) {
        info->description = "performer (nurse) identifier";
        info->required = true;
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_EXECUTION_DTO_H_
