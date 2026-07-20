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
#ifndef _WKF_SERVICE_REQUEST_DO_H_
#define _WKF_SERVICE_REQUEST_DO_H_

#include "../DoInclude.h"

/**
 * Data Object mapping to table: wkf_service_request
 * Core aggregate for medical order verification and execution workflows.
 */
class WkfServiceRequestDO : public BaseDO {
    // Primary key
    MYSQL_SYNTHESIZE(std::string, id, Id);
    // Tenant identifier
    MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
    // Business number
    MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
    // Prescription number
    MYSQL_SYNTHESIZE(std::string, prescriptionNo, PrescriptionNo);
    // Status enum: pending_verification / verified / returned / executing / completed
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    // Intent enum: long_term / temporary
    MYSQL_SYNTHESIZE(std::string, intentEnum, IntentEnum);
    // Category enum: medication / procedure / lab
    MYSQL_SYNTHESIZE(std::string, categoryEnum, CategoryEnum);
    // Group identifier (orders in same group verified/executed together)
    MYSQL_SYNTHESIZE(std::string, groupId, GroupId);
    // Priority enum
    MYSQL_SYNTHESIZE(std::string, priorityEnum, PriorityEnum);
    // Perform flag
    MYSQL_SYNTHESIZE(int, performFlag, PerformFlag);
    // Activity identifier (links to treatment item)
    MYSQL_SYNTHESIZE(std::string, activityId, ActivityId);
    // Quantity
    MYSQL_SYNTHESIZE(double, quantity, Quantity);
    // Unit code
    MYSQL_SYNTHESIZE(std::string, unitCode, UnitCode);
    // Patient identifier
    MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
    // Encounter (visit) identifier
    MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
    // Authored time (when doctor created the order)
    MYSQL_SYNTHESIZE(std::string, authoredTime, AuthoredTime);
    // Requester identifier (doctor who ordered)
    MYSQL_SYNTHESIZE(std::string, requesterId, RequesterId);
    // Performer type code
    MYSQL_SYNTHESIZE(std::string, performerTypeCode, PerformerTypeCode);
    // Performer identifier (nurse who executes)
    MYSQL_SYNTHESIZE(std::string, performerId, PerformerId);
    // Verification nurse identifier (nurse who verified)
    MYSQL_SYNTHESIZE(std::string, performerCheckId, PerformerCheckId);
    // Check time (verification timestamp)
    MYSQL_SYNTHESIZE(std::string, checkTime, CheckTime);
    // Location / department identifier
    MYSQL_SYNTHESIZE(std::string, locationId, LocationId);
    // Organization identifier
    MYSQL_SYNTHESIZE(std::string, orgId, OrgId);
    // Reason text (e.g., return reason for rejected orders)
    MYSQL_SYNTHESIZE(std::string, reasonText, ReasonText);
    // Contract identifier
    MYSQL_SYNTHESIZE(std::string, contractId, ContractId);
    // Occurrence start time
    MYSQL_SYNTHESIZE(std::string, occurrenceStartTime, OccurrenceStartTime);
    // Occurrence end time
    MYSQL_SYNTHESIZE(std::string, occurrenceEndTime, OccurrenceEndTime);
    // Relevant history identifier
    MYSQL_SYNTHESIZE(std::string, relevantHistoryId, RelevantHistoryId);
    // Refund service identifier
    MYSQL_SYNTHESIZE(std::string, refundServiceId, RefundServiceId);
    // Content JSON (extension data)
    MYSQL_SYNTHESIZE(std::string, contentJson, ContentJson);
    // Medical insurance class enum
    MYSQL_SYNTHESIZE(std::string, ybClassEnum, YbClassEnum);
    // Print count
    MYSQL_SYNTHESIZE(int, printCount, PrintCount);
    // Condition definition ID (diagnosis reference)
    MYSQL_SYNTHESIZE(std::string, conditionDefinitionId, ConditionDefinitionId);
    // Encounter diagnosis ID
    MYSQL_SYNTHESIZE(std::string, encounterDiagnosisId, EncounterDiagnosisId);
    // Therapy enum
    MYSQL_SYNTHESIZE(std::string, therapyEnum, TherapyEnum);
    // Rate code (e.g., infusion rate)
    MYSQL_SYNTHESIZE(std::string, rateCode, RateCode);
    // Parent identifier (for split child orders)
    MYSQL_SYNTHESIZE(std::string, parentId, ParentId);
    // Generate source enum
    MYSQL_SYNTHESIZE(std::string, generateSourceEnum, GenerateSourceEnum);
    // Remark / notes
    MYSQL_SYNTHESIZE(std::string, remark, Remark);
    // Entrust content (nurse execution instructions)
    MYSQL_SYNTHESIZE(std::string, entrustContent, EntrustContent);
    // Sort number for display order
    MYSQL_SYNTHESIZE(int, sortNumber, SortNumber);
    // Execution count (for non-medication orders)
    MYSQL_SYNTHESIZE(int, executionCount, ExecutionCount);
    // Optimistic lock version
    MYSQL_SYNTHESIZE(int, version, Version);
    // Soft delete flag: 0=active, 1=deleted
    MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);

public:
    /**
     * Register table name and field mappings.
     * Type placeholders: "s"=string, "i"=int, "d"=double, "ull"=uint64_t
     */
    WkfServiceRequestDO() : BaseDO("wkf_service_request") {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("prescription_no", "s", prescriptionNo);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("intent_enum", "s", intentEnum);
        MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
        MYSQL_ADD_FIELD("group_id", "s", groupId);
        MYSQL_ADD_FIELD("priority_enum", "s", priorityEnum);
        MYSQL_ADD_FIELD("perform_flag", "i", performFlag);
        MYSQL_ADD_FIELD("activity_id", "s", activityId);
        MYSQL_ADD_FIELD("quantity", "d", quantity);
        MYSQL_ADD_FIELD("unit_code", "s", unitCode);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("authored_time", "s", authoredTime);
        MYSQL_ADD_FIELD("requester_id", "s", requesterId);
        MYSQL_ADD_FIELD("performer_type_code", "s", performerTypeCode);
        MYSQL_ADD_FIELD("performer_id", "s", performerId);
        MYSQL_ADD_FIELD("performer_check_id", "s", performerCheckId);
        MYSQL_ADD_FIELD("check_time", "s", checkTime);
        MYSQL_ADD_FIELD("location_id", "s", locationId);
        MYSQL_ADD_FIELD("org_id", "s", orgId);
        MYSQL_ADD_FIELD("reason_text", "s", reasonText);
        MYSQL_ADD_FIELD("contract_id", "s", contractId);
        MYSQL_ADD_FIELD("occurrence_start_time", "s", occurrenceStartTime);
        MYSQL_ADD_FIELD("occurrence_end_time", "s", occurrenceEndTime);
        MYSQL_ADD_FIELD("relevant_history_id", "s", relevantHistoryId);
        MYSQL_ADD_FIELD("refund_service_id", "s", refundServiceId);
        MYSQL_ADD_FIELD("content_json", "s", contentJson);
        MYSQL_ADD_FIELD("yb_class_enum", "s", ybClassEnum);
        MYSQL_ADD_FIELD("print_count", "i", printCount);
        MYSQL_ADD_FIELD("condition_definition_id", "s", conditionDefinitionId);
        MYSQL_ADD_FIELD("encounter_diagnosis_id", "s", encounterDiagnosisId);
        MYSQL_ADD_FIELD("therapy_enum", "s", therapyEnum);
        MYSQL_ADD_FIELD("rate_code", "s", rateCode);
        MYSQL_ADD_FIELD("parent_id", "s", parentId);
        MYSQL_ADD_FIELD("generate_source_enum", "s", generateSourceEnum);
        MYSQL_ADD_FIELD("remark", "s", remark);
        MYSQL_ADD_FIELD("entrust_content", "s", entrustContent);
        MYSQL_ADD_FIELD("sort_number", "i", sortNumber);
        MYSQL_ADD_FIELD("execution_count", "i", executionCount);
        MYSQL_ADD_FIELD("version", "i", version);
        MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
    }
};

// Shared pointer alias
typedef std::shared_ptr<WkfServiceRequestDO> PtrWkfServiceRequestDO;

#endif // !_WKF_SERVICE_REQUEST_DO_H_
