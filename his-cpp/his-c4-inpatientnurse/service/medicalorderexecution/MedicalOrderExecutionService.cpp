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
#include "stdafx.h"
#include "MedicalOrderExecutionService.h"
#include "dao/medicalorderexecution/MedicalOrderExecutionDAO.h"

// ============================================================================
// Helper: convert DO to VO (extract only fields needed by frontend).
// DO has 50+ columns matching the DB table; VO exposes ~20 selected fields.
// ============================================================================
static MedicalOrderItemVO::Wrapper doToVo(const PtrWkfServiceRequestDO& sr)
{
    auto vo = MedicalOrderItemVO::createShared();
    // Basic
    vo->id = sr->getId();
    vo->busNo = sr->getBusNo();
    vo->prescriptionNo = sr->getPrescriptionNo();
    // Status
    vo->statusEnum = sr->getStatusEnum();
    vo->intentEnum = sr->getIntentEnum();
    vo->categoryEnum = sr->getCategoryEnum();
    vo->priorityEnum = sr->getPriorityEnum();
    // Patient
    vo->patientId = sr->getPatientId();
    vo->encounterId = sr->getEncounterId();
    vo->authoredTime = sr->getAuthoredTime();
    // Personnel
    vo->requesterId = sr->getRequesterId();
    vo->performerId = sr->getPerformerId();
    vo->performerCheckId = sr->getPerformerCheckId();
    vo->checkTime = sr->getCheckTime();
    // Location
    vo->locationId = sr->getLocationId();
    // Content
    vo->quantity = sr->getQuantity();
    vo->unitCode = sr->getUnitCode();
    vo->executionCount = sr->getExecutionCount();
    // Notes
    vo->remark = sr->getRemark();
    vo->entrustContent = sr->getEntrustContent();
    vo->reasonText = sr->getReasonText();
    return vo;
}

// ============================================================================
// Requirement 1: get verification items
// ============================================================================
MedicalOrderPageJsonVO::Wrapper MedicalOrderExecutionService::queryVerificationItems(
    const MedicalOrderQuery::Wrapper& query)
{
    auto page = PageDTO<MedicalOrderItemVO::Wrapper>::createShared();
    MedicalOrderExecutionDAO dao;

    // Default to pending_verification status
    if (!query->statusEnum) {
        query->statusEnum = "pending_verification";
    }

    auto doList = dao.queryVerificationItems(query);
    auto total = dao.countVerificationItems(query);

    for (const auto& sr : doList) {
        page->addData(doToVo(sr));
    }

    page->pageIndex = query->pageIndex.getValue(1);
    page->pageSize = query->pageSize.getValue(10);
    page->total = v_int64(static_cast<int64_t>(total));
    page->calcPages();

    auto result = MedicalOrderPageJsonVO::createShared();
    result->success(page);
    return result;
}

// ============================================================================
// Requirement 2: verify pass
// ============================================================================
StringJsonVO::Wrapper MedicalOrderExecutionService::execVerifyPass(
    const VerifyPassDTO::Wrapper& dto)
{
    auto result = StringJsonVO::createShared();

    if (!dto->serviceRequestId || dto->serviceRequestId.getValue("").empty()) {
        result->fail(String("serviceRequestId is required"));
        return result;
    }
    if (!dto->verifierId || dto->verifierId.getValue("").empty()) {
        result->fail(String("verifierId is required"));
        return result;
    }

    MedicalOrderExecutionDAO dao;
    int rows = dao.verifyPass(
        dto->serviceRequestId.getValue(""),
        dto->verifierId.getValue(""));

    if (rows > 0) {
        result->success(String("Verification passed"));
    } else {
        result->fail(String("Verification failed: order not found or already processed"));
    }
    return result;
}

// ============================================================================
// Requirement 3: return order
// ============================================================================
StringJsonVO::Wrapper MedicalOrderExecutionService::execReturnOrder(
    const ReturnOrderDTO::Wrapper& dto)
{
    auto result = StringJsonVO::createShared();

    if (!dto->serviceRequestId || dto->serviceRequestId.getValue("").empty()) {
        result->fail(String("serviceRequestId is required"));
        return result;
    }
    if (!dto->returnReason || dto->returnReason.getValue("").empty()) {
        result->fail(String("returnReason is required"));
        return result;
    }

    MedicalOrderExecutionDAO dao;
    int rows = dao.returnOrder(
        dto->serviceRequestId.getValue(""),
        dto->returnReason.getValue(""));

    if (rows > 0) {
        result->success(String("Order returned"));
    } else {
        result->fail(String("Return failed: order not found or already processed"));
    }
    return result;
}

// ============================================================================
// Requirement 4: get execution items
// ============================================================================
MedicalOrderPageJsonVO::Wrapper MedicalOrderExecutionService::queryExecutionItems(
    const MedicalOrderQuery::Wrapper& query)
{
    auto page = PageDTO<MedicalOrderItemVO::Wrapper>::createShared();
    MedicalOrderExecutionDAO dao;

    // Default to verified status (ready to execute)
    if (!query->statusEnum) {
        query->statusEnum = "verified";
    }

    auto doList = dao.queryExecutionItems(query);
    auto total = dao.countExecutionItems(query);

    for (const auto& sr : doList) {
        page->addData(doToVo(sr));
    }

    page->pageIndex = query->pageIndex.getValue(1);
    page->pageSize = query->pageSize.getValue(10);
    page->total = v_int64(static_cast<int64_t>(total));
    page->calcPages();

    auto result = MedicalOrderPageJsonVO::createShared();
    result->success(page);
    return result;
}
