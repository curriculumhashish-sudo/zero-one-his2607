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
#ifndef _MEDICAL_ORDER_EXECUTION_CONTROLLER_H_
#define _MEDICAL_ORDER_EXECUTION_CONTROLLER_H_

#include "ApiHelper.h"
#include "domain/query/medicalorderexecution/MedicalOrderQuery.h"
#include "domain/dto/medicalorderexecution/MedicalOrderExecutionDTO.h"
#include "domain/vo/medicalorderexecution/MedicalOrderVO.h"
#include "service/medicalorderexecution/MedicalOrderExecutionService.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * Inpatient Nurse - Medical Order Execution & Verification Controller.
 *
 * Swagger UI: http://localhost:8090/swagger/ui
 *
 * Endpoints:
 *   1. GET  /medical-order/verification-items  - query orders awaiting verification
 *   2. POST /medical-order/verify-pass          - approve verification
 *   3. POST /medical-order/return-order         - reject/return an order
 *   4. GET  /medical-order/execution-items      - query orders ready for execution
 *   5. POST /medical-order/execute-order        - start executing an order
 *   6. POST /medical-order/complete-order       - complete execution of an order
 */

#define API_TAG ZH_WORDS_GETTER("medical-order-execution.tag")

class MedicalOrderExecutionController : public oatpp::web::server::api::ApiController
{
    // Generates constructor, auth handler, and factory method (token validation via CustomerAuthorizeHandler)
    API_ACCESS_DECLARE(MedicalOrderExecutionController);

public:
    // ========================================================================
    // Requirement 1: Get verification items (query with filters + pagination)
    // GET /medical-order/verification-items?pageIndex=1&pageSize=10&patientId=...
    // ========================================================================
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("medical-order-execution.verification-items.summary"),
        queryVerificationItems,
        MedicalOrderQuery,
        MedicalOrderPageJsonVO::Wrapper,
        API_TAG)
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/medical-order/verification-items",
        queryVerificationItems,
        MedicalOrderQuery,
        execQueryVerificationItems(query))

    // ========================================================================
    // Requirement 2: Verify pass
    // POST /medical-order/verify-pass
    // Body: { "serviceRequestId": "...", "verifierId": "..." }
    // ========================================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("medical-order-execution.verify-pass.summary"),
        execVerifyPass,
        StringJsonVO::Wrapper,
        API_TAG)
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/medical-order/verify-pass",
        execVerifyPass,
        BODY_DTO(VerifyPassDTO::Wrapper, dto),
        execVerifyPass(dto))

    // ========================================================================
    // Requirement 3: Return order
    // POST /medical-order/return-order
    // Body: { "serviceRequestId": "...", "returnReason": "..." }
    // ========================================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("medical-order-execution.return-order.summary"),
        execReturnOrder,
        StringJsonVO::Wrapper,
        API_TAG)
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/medical-order/return-order",
        execReturnOrder,
        BODY_DTO(ReturnOrderDTO::Wrapper, dto),
        execReturnOrder(dto))

    // ========================================================================
    // Requirement 4: Get execution items (query with filters + pagination)
    // GET /medical-order/execution-items?pageIndex=1&pageSize=10
    // ========================================================================
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("medical-order-execution.execution-items.summary"),
        queryExecutionItems,
        MedicalOrderQuery,
        MedicalOrderPageJsonVO::Wrapper,
        API_TAG)
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/medical-order/execution-items",
        queryExecutionItems,
        MedicalOrderQuery,
        execQueryExecutionItems(query))

    // ========================================================================
    // Requirement 5: Start executing an order
    // POST /medical-order/execute-order
    // Body: { "serviceRequestId": "...", "performerId": "..." }
    // ========================================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("medical-order-execution.execute-order.summary"),
        execStartExecution,
        StringJsonVO::Wrapper,
        API_TAG)
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/medical-order/execute-order",
        execStartExecution,
        BODY_DTO(ExecuteOrderDTO::Wrapper, dto),
        execStartExecution(dto))

    // ========================================================================
    // Requirement 6: Complete execution of an order
    // POST /medical-order/complete-order
    // Body: { "serviceRequestId": "...", "performerId": "..." }
    // ========================================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("medical-order-execution.complete-order.summary"),
        execCompleteExecution,
        StringJsonVO::Wrapper,
        API_TAG)
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/medical-order/complete-order",
        execCompleteExecution,
        BODY_DTO(CompleteOrderDTO::Wrapper, dto),
        execCompleteExecution(dto))

private:
    MedicalOrderPageJsonVO::Wrapper execQueryVerificationItems(
        const MedicalOrderQuery::Wrapper& query)
    {
        return MedicalOrderExecutionService().queryVerificationItems(query);
    }

    StringJsonVO::Wrapper execVerifyPass(const VerifyPassDTO::Wrapper& dto)
    {
        return MedicalOrderExecutionService().execVerifyPass(dto);
    }

    StringJsonVO::Wrapper execReturnOrder(const ReturnOrderDTO::Wrapper& dto)
    {
        return MedicalOrderExecutionService().execReturnOrder(dto);
    }

    MedicalOrderPageJsonVO::Wrapper execQueryExecutionItems(
        const MedicalOrderQuery::Wrapper& query)
    {
        return MedicalOrderExecutionService().queryExecutionItems(query);
    }

    StringJsonVO::Wrapper execStartExecution(const ExecuteOrderDTO::Wrapper& dto)
    {
        return MedicalOrderExecutionService().execStartExecution(dto);
    }

    StringJsonVO::Wrapper execCompleteExecution(const CompleteOrderDTO::Wrapper& dto)
    {
        return MedicalOrderExecutionService().execCompleteExecution(dto);
    }
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_MEDICAL_ORDER_EXECUTION_CONTROLLER_H_
