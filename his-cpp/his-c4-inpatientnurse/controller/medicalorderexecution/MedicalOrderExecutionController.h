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
#ifndef _MEDICAL_ORDER_EXECUTION_CONTROLLER_H_
#define _MEDICAL_ORDER_EXECUTION_CONTROLLER_H_

#include "ApiHelper.h"
#include "domain/query/medicalorderexecution/MedicalOrderQuery.h"
#include "domain/dto/medicalorderexecution/MedicalOrderExecutionDTO.h"
#include "domain/vo/medicalorderexecution/MedicalOrderVO.h"
#include "service/medicalorderexecution/MedicalOrderExecutionService.h"
#include "controller/RequestValidation.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * Inpatient nurse medical-order execution controller.
 *
 * Swagger UI: http://localhost:8090/swagger/ui
 *
 * This controller exposes only the query and execute operations. Verification
 * operations belong to the medicalorderverification module, while cancel and
 * skip operations are exposed by OrderExecuteControlController.
 */

#define API_TAG ZH_WORDS_GETTER("medical-order-execution.tag")

class MedicalOrderExecutionController : public oatpp::web::server::api::ApiController
{
    // Generates constructor, auth handler, and factory method (token validation via CustomerAuthorizeHandler)
    API_ACCESS_DECLARE(MedicalOrderExecutionController);

public:
    // Get execution items with filters and pagination.
    // GET /medical-order/execution-items?pageIndex=1&pageSize=10
    // ========================================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("medical-order-execution.execution-items.summary"),
        queryExecutionItems,
        MedicalOrderPageJsonVO::Wrapper,
        API_TAG,
        API_DEF_QUERY_PARAM_BUILD(MedicalOrderQuery);
        info->queryParams["statusEnum"].addExample("default", String("VERIFIED"));
        info->queryParams["patientId"].addExample("default", String("326940849401761792"));
        info->queryParams["encounterId"].addExample("default", String("337907884548657152"));
        info->queryParams["startDate"].addExample("default", String("2026-07-01 00:00:00"));
        info->queryParams["endDate"].addExample("default", String("2026-07-24 23:59:59"));
        info->queryParams["locationId"].addExample("default", String("334526752078102528"));
        info->queryParams["categoryEnum"].addExample("default", String("MEDICATION"));
        info->queryParams["priorityEnum"].addExample("default", String("ROUTINE"));
        info->queryParams["intentEnum"].addExample("default", String("TEMPORARY"));
        info->queryParams["therapyEnum"].addExample("default", String("TEMPORARY"));
        info->responses[Status::CODE_200].addExample(
            "success", MedicalOrderPageJsonVO::createShared());
    )
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/medical-order/execution-items",
        queryExecutionItems,
        MedicalOrderQuery,
		execQueryExecutionItems(query, authObject->getPayload()))

    // Execute a medical order.
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("medical-order-execution.execute-order.summary"),
        execStartExecution,
        StringJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", ExecuteOrderDTO::createExample());
        info->responses[Status::CODE_200].addExample(
            "success", StringJsonVO::createShared());
    )
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/medical-order/execute-order",
        execStartExecution,
        C4_REQUIRED_BODY_DTO(ExecuteOrderDTO::Wrapper, dto),
        execStartExecution(dto, authObject->getPayload()))

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medical-order-execution.workbench-query.summary"),
		queryExecutionWorkbench,
		OrderExecutionWorkbenchJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", OrderExecutionWorkbenchQueryDTO::createExample());
		info->responses[Status::CODE_200].addExample(
			"success", OrderExecutionWorkbenchJsonVO::createShared());
	)
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/nurse-station/order-executions/query",
		queryExecutionWorkbench,
		C4_REQUIRED_BODY_DTO(OrderExecutionWorkbenchQueryDTO::Wrapper, dto),
		execQueryExecutionWorkbench(dto, authObject->getPayload()))

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medical-order-execution.workbench-execute.summary"),
		executeOrderBatch,
		BatchOrderExecutionJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", BatchExecuteOrderRequestDTO::createExample());
		info->responses[Status::CODE_200].addExample(
			"success", BatchOrderExecutionJsonVO::createShared());
	)
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/nurse-station/order-executions/execute",
		executeOrderBatch,
		C4_REQUIRED_BODY_DTO(BatchExecuteOrderRequestDTO::Wrapper, dto),
		execExecuteOrderBatch(dto, authObject->getPayload()))

private:
    MedicalOrderPageJsonVO::Wrapper execQueryExecutionItems(
		const MedicalOrderQuery::Wrapper& query, const PayloadDTO& payload);
    StringJsonVO::Wrapper execStartExecution(
        const ExecuteOrderDTO::Wrapper& dto, const PayloadDTO& payload);
	OrderExecutionWorkbenchJsonVO::Wrapper execQueryExecutionWorkbench(
		const OrderExecutionWorkbenchQueryDTO::Wrapper& dto,
		const PayloadDTO& payload);
	BatchOrderExecutionJsonVO::Wrapper execExecuteOrderBatch(
		const BatchExecuteOrderRequestDTO::Wrapper& dto,
		const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_MEDICAL_ORDER_EXECUTION_CONTROLLER_H_
