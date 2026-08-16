#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/24 16:00:00

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
#ifndef _MEDICAL_ORDER_ITEM_DTO_H_
#define _MEDICAL_ORDER_ITEM_DTO_H_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class OrderExecutionSlotDTO : public oatpp::DTO
{
	DTO_INIT(OrderExecutionSlotDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, planTime,
		ZH_WORDS_GETTER("medical-order-execution.field.planTime"), true);
	API_DTO_FIELD_REQUIRE(String, procedureId,
		ZH_WORDS_GETTER("medical-order-execution.field.procedureId"), false);
	API_DTO_FIELD_REQUIRE(String, statusEnum,
		ZH_WORDS_GETTER("medical-order-execution.field.statusEnum"), true);
	API_DTO_FIELD_REQUIRE(String, recordedTime,
		ZH_WORDS_GETTER("medical-order-execution.field.recordedTime"), false);
};

class OrderExecutionChargeBindingDTO : public oatpp::DTO
{
	DTO_INIT(OrderExecutionChargeBindingDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, bindType,
		ZH_WORDS_GETTER("medical-order-execution.field.bindingType"), true);
	API_DTO_FIELD_REQUIRE(String, bindingType,
		ZH_WORDS_GETTER("medical-order-execution.field.bindingType"), true);
	API_DTO_FIELD_REQUIRE(String, bindingId,
		ZH_WORDS_GETTER("medical-order-execution.field.bindingId"), true);
	API_DTO_FIELD_REQUIRE(String, activityId,
		ZH_WORDS_GETTER("medical-order-execution.field.activityId"), false);
	API_DTO_FIELD_REQUIRE(String, deviceId,
		ZH_WORDS_GETTER("medical-order-execution.field.deviceId"), false);
	API_DTO_FIELD_REQUIRE(String, chargeItemDefinitionId,
		ZH_WORDS_GETTER("medical-order-execution.field.chargeItemDefinitionId"), true);
	API_DTO_FIELD_REQUIRE(String, name,
		ZH_WORDS_GETTER("medical-order-execution.field.name"), false);
	API_DTO_FIELD_REQUIRE(String, specificationText,
		ZH_WORDS_GETTER("medical-order-execution.field.specificationText"), false);
	API_DTO_FIELD_REQUIRE(Float64, unitPrice,
		ZH_WORDS_GETTER("medical-order-execution.field.unitPrice"), false);
	API_DTO_FIELD_REQUIRE(Float64, quantity,
		ZH_WORDS_GETTER("medical-order-execution.field.quantity"), false);
	API_DTO_FIELD_REQUIRE(String, unitCode,
		ZH_WORDS_GETTER("medical-order-execution.field.unitCode"), false);
	API_DTO_FIELD_REQUIRE(String, priorityEnum,
		ZH_WORDS_GETTER("medical-order-execution.field.priorityEnum"), false);
};

/**
 * DTO: single medical order item for verification and execution list rows.
 * Contains the fields needed by the frontend (subset of wkf_service_request columns).
 */
class MedicalOrderItemDTO : public oatpp::DTO
{
    DTO_INIT(MedicalOrderItemDTO, DTO);

    // ============================================================
    // Basic info
    // ============================================================

    // Service request primary key
    API_DTO_FIELD_DEFAULT(String, id, "service request ID");

    // Source request table, used by execution commands to select the correct workflow table
    API_DTO_FIELD_DEFAULT(String, orderType, "order source: SERVICE_REQUEST or MED_REQUEST");

    // Business number
    API_DTO_FIELD_DEFAULT(String, busNo, "business number");

    // Prescription number
    API_DTO_FIELD_DEFAULT(String, prescriptionNo, "prescription number");

    // ============================================================
    // Status fields
    // ============================================================

    // Workflow status: pending_verification / verified / returned / executing / completed
    API_DTO_FIELD_DEFAULT(String, statusEnum, "current workflow status");

    // Order category: medication / procedure / lab
    API_DTO_FIELD_DEFAULT(String, categoryEnum, "order category");

    // Order type: long_term / temporary
    API_DTO_FIELD_DEFAULT(String, intentEnum, "order type (long_term/temporary)");

	API_DTO_FIELD_DEFAULT(String, therapyEnum,
		ZH_WORDS_GETTER("medical-order-execution.field.therapyEnum"));

    // Order priority
    API_DTO_FIELD_DEFAULT(String, priorityEnum, "order priority");

    // ============================================================
    // Patient info
    // ============================================================

    // Patient identifier
    API_DTO_FIELD_DEFAULT(String, patientId, "patient identifier");

    // Encounter (visit) identifier
    API_DTO_FIELD_DEFAULT(String, encounterId, "encounter identifier");

    // ============================================================
    // Time
    // ============================================================

    // Order creation time
    API_DTO_FIELD_DEFAULT(String, authoredTime, "order creation time");

	API_DTO_FIELD_DEFAULT(String, planTime,
		ZH_WORDS_GETTER("medical-order-execution.field.planTime"));

	API_DTO_FIELD_DEFAULT(String, adviceDisplayName,
		ZH_WORDS_GETTER("medical-order-execution.field.adviceDisplayName"));

	API_DTO_FIELD_DEFAULT(String, procedureId,
		ZH_WORDS_GETTER("medical-order-execution.field.procedureId"));
	API_DTO_FIELD_DEFAULT(String, listRowKey,
		ZH_WORDS_GETTER("medical-order-execution.field.listRowKey"));
	API_DTO_FIELD_DEFAULT(String, statusDisplay,
		ZH_WORDS_GETTER("medical-order-execution.field.statusDisplay"));
	API_DTO_FIELD_DEFAULT(String, therapyDisplay,
		ZH_WORDS_GETTER("medical-order-execution.field.therapyDisplay"));
	API_DTO_FIELD_DEFAULT(String, specificationText,
		ZH_WORDS_GETTER("medical-order-execution.field.specificationText"));
	API_DTO_FIELD_DEFAULT(String, dosageDisplay,
		ZH_WORDS_GETTER("medical-order-execution.field.dosageDisplay"));
	API_DTO_FIELD_DEFAULT(String, usageDisplay,
		ZH_WORDS_GETTER("medical-order-execution.field.usageDisplay"));
	API_DTO_FIELD_DEFAULT(String, frequencyDisplay,
		ZH_WORDS_GETTER("medical-order-execution.field.frequencyDisplay"));
	API_DTO_FIELD_DEFAULT(String, stopTime,
		ZH_WORDS_GETTER("medical-order-execution.field.stopTime"));
	API_DTO_FIELD_DEFAULT(String, requesterName,
		ZH_WORDS_GETTER("medical-order-execution.field.requesterName"));
	API_DTO_FIELD_DEFAULT(String, performerLocationDisplay,
		ZH_WORDS_GETTER("medical-order-execution.field.performerLocationDisplay"));
	API_DTO_FIELD_DEFAULT(Boolean, tcmFlag,
		ZH_WORDS_GETTER("medical-order-execution.field.tcmFlag"));
	API_DTO_FIELD_DEFAULT(Float64, chineseHerbsDoseQuantity,
		ZH_WORDS_GETTER("medical-order-execution.field.chineseHerbsDoseQuantity"));
	API_DTO_FIELD_DEFAULT(List<String>, lineRequestIds,
		ZH_WORDS_GETTER("medical-order-execution.field.lineRequestIds"));
	API_DTO_FIELD_DEFAULT(List<OrderExecutionSlotDTO::Wrapper>, pendingSlots,
		ZH_WORDS_GETTER("medical-order-execution.field.pendingSlots"));
	API_DTO_FIELD_DEFAULT(List<OrderExecutionSlotDTO::Wrapper>, executedSlots,
		ZH_WORDS_GETTER("medical-order-execution.field.executedSlots"));
	API_DTO_FIELD_DEFAULT(List<OrderExecutionSlotDTO::Wrapper>, skippedSlots,
		ZH_WORDS_GETTER("medical-order-execution.field.skippedSlots"));
	API_DTO_FIELD_DEFAULT(List<OrderExecutionSlotDTO::Wrapper>, cancelledSlots,
		ZH_WORDS_GETTER("medical-order-execution.field.cancelledSlots"));
	API_DTO_FIELD_DEFAULT(List<OrderExecutionChargeBindingDTO::Wrapper>, bindingItems,
		ZH_WORDS_GETTER("medical-order-execution.field.bindingItems"));
	API_DTO_FIELD_DEFAULT(List<OrderExecutionChargeBindingDTO::Wrapper>, bindItems,
		ZH_WORDS_GETTER("medical-order-execution.field.bindingItems"));

    // ============================================================
    // Quantity
    // ============================================================

    // Order quantity
    API_DTO_FIELD_DEFAULT(Float64, quantity, "quantity");

    // Unit code
    API_DTO_FIELD_DEFAULT(String, unitCode, "unit code");

    // ============================================================
    // Personnel
    // ============================================================

    // Requester (doctor) identifier
    API_DTO_FIELD_DEFAULT(String, requesterId, "requester (doctor) identifier");

    // Performer (nurse) identifier
    API_DTO_FIELD_DEFAULT(String, performerId, "performer (nurse) identifier");

    // Verifier identifier
    API_DTO_FIELD_DEFAULT(String, performerCheckId, "verifier identifier");

    // Verification timestamp
    API_DTO_FIELD_DEFAULT(String, checkTime, "verification timestamp");

    // ============================================================
    // Location
    // ============================================================

    // Department / location identifier
    API_DTO_FIELD_DEFAULT(String, locationId, "department/location identifier");

    // ============================================================
    // Execution
    // ============================================================

    // Execution count
    API_DTO_FIELD_DEFAULT(Int32, executionCount, "execution count");

    // ============================================================
    // Notes
    // ============================================================

    // Remark / notes
    API_DTO_FIELD_DEFAULT(String, remark, "remark/notes");

    // Entrust content / nurse instructions
    API_DTO_FIELD_DEFAULT(String, entrustContent, "entrust content / nurse instructions");

    // Return/rejection reason text
    API_DTO_FIELD_DEFAULT(String, reasonText, "return/rejection reason");

public:
    MedicalOrderItemDTO()
    {
		quantity = 0.0;
		chineseHerbsDoseQuantity = 0.0;
		tcmFlag = false;
        executionCount = 0;
		lineRequestIds = List<String>::createShared();
		pendingSlots = List<OrderExecutionSlotDTO::Wrapper>::createShared();
		executedSlots = List<OrderExecutionSlotDTO::Wrapper>::createShared();
		skippedSlots = List<OrderExecutionSlotDTO::Wrapper>::createShared();
		cancelledSlots = List<OrderExecutionSlotDTO::Wrapper>::createShared();
		bindingItems = List<OrderExecutionChargeBindingDTO::Wrapper>::createShared();
		bindItems = List<OrderExecutionChargeBindingDTO::Wrapper>::createShared();
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_ITEM_DTO_H_
