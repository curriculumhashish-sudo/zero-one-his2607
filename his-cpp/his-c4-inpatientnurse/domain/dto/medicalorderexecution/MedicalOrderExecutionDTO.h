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
#ifndef _MEDICAL_ORDER_EXECUTION_DTO_H_
#define _MEDICAL_ORDER_EXECUTION_DTO_H_

#include "../../GlobalInclude.h"
#include "MedicalOrderItemDTO.h"
#include "OrderExecuteDTO.h"

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
    API_DTO_FIELD_REQUIRE(String, serviceRequestId,
        "service request ID to verify", true);

    // ID of the nurse performing verification (required)
    API_DTO_FIELD_REQUIRE(String, verifierId,
        "verifier (nurse) identifier", true);
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
    API_DTO_FIELD_REQUIRE(String, serviceRequestId,
        "service request ID to return", true);

    // Reason for rejection (required)
    API_DTO_FIELD_REQUIRE(String, returnReason,
        "reason for returning the order", true);
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
    API_DTO_FIELD_REQUIRE(String, serviceRequestId,
        "service request ID to execute", true);

    API_DTO_FIELD_REQUIRE(String, orderType,
        "order type: SERVICE_REQUEST or MED_REQUEST", false);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("medical-order-execution.field.encounterId"), true);

	API_DTO_FIELD_REQUIRE(String, planTime,
		ZH_WORDS_GETTER("medical-order-execution.field.planTime"), true);

    // ID of the nurse performing execution (required)
    API_DTO_FIELD_REQUIRE(String, performerId,
        "performer (nurse) identifier", false);

public:
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->serviceRequestId = "337907884548657152";
        dto->orderType = "SERVICE_REQUEST";
		dto->encounterId = "337907884548657152";
		dto->planTime = "2026-07-24 08:00:00";
        dto->performerId = "326940849401761792";
        return dto;
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
    API_DTO_FIELD_REQUIRE(String, serviceRequestId,
        "service request ID to complete", true);

    // ID of the nurse completing execution (required)
    API_DTO_FIELD_REQUIRE(String, performerId,
        "performer (nurse) identifier", true);
};

class OrderExecutionWorkbenchQueryDTO : public oatpp::DTO
{
	DTO_INIT(OrderExecutionWorkbenchQueryDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, encounterIds,
		ZH_WORDS_GETTER("medical-order-execution.field.encounterIds"), true);
	API_DTO_FIELD_REQUIRE(String, executeStatus,
		ZH_WORDS_GETTER("medical-order-execution.field.executeStatus"), true);
	API_DTO_FIELD_REQUIRE(String, therapyType,
		ZH_WORDS_GETTER("medical-order-execution.field.therapyType"), true);
	API_DTO_FIELD_REQUIRE(String, medicineCategory,
		ZH_WORDS_GETTER("medical-order-execution.field.medicineCategory"), true);
	API_DTO_FIELD_REQUIRE(String, executionDeadline,
		ZH_WORDS_GETTER("medical-order-execution.field.executionDeadline"), true);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->encounterIds = List<String>::createShared();
		dto->encounterIds->push_back("337907884548657152");
		dto->executeStatus = "PENDING";
		dto->therapyType = "ALL";
		dto->medicineCategory = "WEST";
		dto->executionDeadline = "2026-08-13 23:59:59";
		return dto;
	}
};

class OrderExecutionGroupDTO : public oatpp::DTO
{
	DTO_INIT(OrderExecutionGroupDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("medical-order-execution.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(List<MedicalOrderItemDTO::Wrapper>, orders,
		ZH_WORDS_GETTER("medical-order-execution.field.orders"), true);

public:
	OrderExecutionGroupDTO()
	{
		orders = List<MedicalOrderItemDTO::Wrapper>::createShared();
	}
};

class OrderExecutionWorkbenchDTO : public oatpp::DTO
{
	DTO_INIT(OrderExecutionWorkbenchDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<OrderExecutionGroupDTO::Wrapper>, groups,
		ZH_WORDS_GETTER("medical-order-execution.field.groups"), true);

public:
	OrderExecutionWorkbenchDTO()
	{
		groups = List<OrderExecutionGroupDTO::Wrapper>::createShared();
	}
};

class BatchExecuteOrderItemDTO : public oatpp::DTO
{
	DTO_INIT(BatchExecuteOrderItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, orderId,
		ZH_WORDS_GETTER("medical-order-execution.field.orderId"), true);
	API_DTO_FIELD_REQUIRE(String, orderType,
		ZH_WORDS_GETTER("medical-order-execution.field.orderType"), true);
	API_DTO_FIELD_REQUIRE(String, planTime,
		ZH_WORDS_GETTER("medical-order-execution.field.planTime"), true);
	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("medical-order-execution.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, accountId,
		ZH_WORDS_GETTER("medical-order-execution.field.accountId"), true);
	API_DTO_FIELD_REQUIRE(String, actualTime,
		ZH_WORDS_GETTER("medical-order-execution.field.actualTime"), true);
	API_DTO_FIELD_REQUIRE(List<OrderExecutionChargeBindingDTO::Wrapper>, bindItems,
		ZH_WORDS_GETTER("medical-order-execution.field.bindingItems"), false);
	API_DTO_FIELD_REQUIRE(List<String>, lineRequestIds,
		ZH_WORDS_GETTER("medical-order-execution.field.lineRequestIds"), false);
	API_DTO_FIELD_REQUIRE(String, prescriptionNo,
		ZH_WORDS_GETTER("medical-order-execution.field.prescriptionNo"), false);
};

class BatchExecuteOrderRequestDTO : public oatpp::DTO
{
	DTO_INIT(BatchExecuteOrderRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("medical-order-execution.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(List<BatchExecuteOrderItemDTO::Wrapper>, items,
		ZH_WORDS_GETTER("medical-order-execution.field.items"), true);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->idempotencyKey = "EXECUTE-20260813-0001";
		dto->items = List<BatchExecuteOrderItemDTO::Wrapper>::createShared();
		auto item = BatchExecuteOrderItemDTO::createShared();
		item->orderId = "338131147144708096";
		item->orderType = "MED_REQUEST";
		item->planTime = "2026-08-13 08:00:00";
		item->encounterId = "337907884548657152";
		item->accountId = "337907884548657153";
		item->actualTime = "2026-08-13 08:01:00";
		item->bindItems = List<OrderExecutionChargeBindingDTO::Wrapper>::createShared();
		item->lineRequestIds = List<String>::createShared();
		dto->items->push_back(item);
		return dto;
	}
};

class BatchSkipOrderItemDTO : public oatpp::DTO
{
	DTO_INIT(BatchSkipOrderItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, orderId,
		ZH_WORDS_GETTER("medical-order-execution.field.orderId"), true);
	API_DTO_FIELD_REQUIRE(String, orderType,
		ZH_WORDS_GETTER("medical-order-execution.field.orderType"), true);
	API_DTO_FIELD_REQUIRE(String, planTime,
		ZH_WORDS_GETTER("medical-order-execution.field.planTime"), true);
};

class BatchSkipOrderRequestDTO : public oatpp::DTO
{
	DTO_INIT(BatchSkipOrderRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("medical-order-execution.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(String, reason,
		ZH_WORDS_GETTER("medical-order-execution.field.reason"), false);
	API_DTO_FIELD_REQUIRE(List<BatchSkipOrderItemDTO::Wrapper>, items,
		ZH_WORDS_GETTER("medical-order-execution.field.items"), true);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->idempotencyKey = "SKIP-20260813-0001";
		dto->reason = ZH_WORDS_GETTER("business-text.execution.deferred-reason");
		dto->items = List<BatchSkipOrderItemDTO::Wrapper>::createShared();
		auto item = BatchSkipOrderItemDTO::createShared();
		item->orderId = "338131147144708096";
		item->orderType = "MED_REQUEST";
		item->planTime = "2026-08-13 08:00:00";
		dto->items->push_back(item);
		return dto;
	}
};

class BatchCancelExecutionRequestDTO : public oatpp::DTO
{
	DTO_INIT(BatchCancelExecutionRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("medical-order-execution.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(String, reason,
		ZH_WORDS_GETTER("medical-order-execution.field.reason"), true);
	API_DTO_FIELD_REQUIRE(List<CancelExecutionItemDTO::Wrapper>, items,
		ZH_WORDS_GETTER("medical-order-execution.field.items"), true);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->idempotencyKey = "CANCEL-20260813-0001";
		dto->reason = ZH_WORDS_GETTER("business-text.execution.deferred-reason");
		dto->items = List<CancelExecutionItemDTO::Wrapper>::createShared();
		dto->items->push_back(CancelExecutionItemDTO::createExample());
		return dto;
	}
};

class BatchOrderExecutionResultItemDTO : public oatpp::DTO
{
	DTO_INIT(BatchOrderExecutionResultItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, itemKey,
		ZH_WORDS_GETTER("medical-order-execution.field.itemKey"), true);
	API_DTO_FIELD_REQUIRE(Boolean, success,
		ZH_WORDS_GETTER("medical-order-execution.field.success"), true);
	API_DTO_FIELD_REQUIRE(String, procedureId,
		ZH_WORDS_GETTER("medical-order-execution.field.procedureId"), false);
	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("medical-order-execution.field.message"), false);
};

class BatchOrderExecutionResponseDTO : public oatpp::DTO
{
	DTO_INIT(BatchOrderExecutionResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(Boolean, success,
		ZH_WORDS_GETTER("medical-order-execution.field.success"), true);
	API_DTO_FIELD_REQUIRE(Int32, processedCount,
		ZH_WORDS_GETTER("medical-order-execution.field.processedCount"), true);
	API_DTO_FIELD_REQUIRE(List<String>, cancelledProcedureIds,
		ZH_WORDS_GETTER("medical-order-execution.field.cancelledProcedureIds"), false);
	API_DTO_FIELD_REQUIRE(List<String>, rebuiltSummaryNos,
		ZH_WORDS_GETTER("medical-order-execution.field.rebuiltSummaryNos"), false);
	API_DTO_FIELD_REQUIRE(List<BatchOrderExecutionResultItemDTO::Wrapper>, results,
		ZH_WORDS_GETTER("medical-order-execution.field.results"), true);

public:
	BatchOrderExecutionResponseDTO()
	{
		success = false;
		processedCount = 0;
		cancelledProcedureIds = List<String>::createShared();
		rebuiltSummaryNos = List<String>::createShared();
		results = List<BatchOrderExecutionResultItemDTO::Wrapper>::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_EXECUTION_DTO_H_
