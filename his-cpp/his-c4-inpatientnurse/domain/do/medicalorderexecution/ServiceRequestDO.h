#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/27 00:00:00

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
#ifndef _WKFSERVICEREQUESTDO_H_
#define _WKFSERVICEREQUESTDO_H_

#include "../DoInclude.h"

// Complete mapping for database table wkf_service_request.
class WkfServiceRequestDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, prescriptionNo, PrescriptionNo);
	MYSQL_SYNTHESIZE(string, basedOnTable, BasedOnTable);
	MYSQL_SYNTHESIZE(string, basedOnId, BasedOnId);
	MYSQL_SYNTHESIZE(string, requisitionId, RequisitionId);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, intentEnum, IntentEnum);
	MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);
	MYSQL_SYNTHESIZE(string, groupId, GroupId);
	MYSQL_SYNTHESIZE(string, priorityEnum, PriorityEnum);
	MYSQL_SYNTHESIZE(bool, performFlag, PerformFlag);
	MYSQL_SYNTHESIZE(string, activityId, ActivityId);
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, authoredTime, AuthoredTime);
	MYSQL_SYNTHESIZE(string, requesterId, RequesterId);
	MYSQL_SYNTHESIZE(string, performerTypeCode, PerformerTypeCode);
	MYSQL_SYNTHESIZE(string, performerId, PerformerId);
	MYSQL_SYNTHESIZE(string, performerCheckId, PerformerCheckId);
	MYSQL_SYNTHESIZE(string, checkTime, CheckTime);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(string, reasonText, ReasonText);
	MYSQL_SYNTHESIZE(string, contractId, ContractId);
	MYSQL_SYNTHESIZE(string, occurrenceStartTime, OccurrenceStartTime);
	MYSQL_SYNTHESIZE(string, occurrenceEndTime, OccurrenceEndTime);
	MYSQL_SYNTHESIZE(string, relevantHistoryId, RelevantHistoryId);
	MYSQL_SYNTHESIZE(string, refundServiceId, RefundServiceId);
	MYSQL_SYNTHESIZE(string, contentJson, ContentJson);
	MYSQL_SYNTHESIZE(string, ybClassEnum, YbClassEnum);
	MYSQL_SYNTHESIZE(int, printCount, PrintCount);
	MYSQL_SYNTHESIZE(string, conditionDefinitionId, ConditionDefinitionId);
	MYSQL_SYNTHESIZE(string, encounterDiagnosisId, EncounterDiagnosisId);
	MYSQL_SYNTHESIZE(string, therapyEnum, TherapyEnum);
	MYSQL_SYNTHESIZE(string, rateCode, RateCode);
	MYSQL_SYNTHESIZE(string, parentId, ParentId);
	MYSQL_SYNTHESIZE(string, generateSourceEnum, GenerateSourceEnum);
	MYSQL_SYNTHESIZE(string, remark, Remark);
	MYSQL_SYNTHESIZE(string, entrustContent, EntrustContent);
	MYSQL_SYNTHESIZE(int, sortNumber, SortNumber);
	MYSQL_SYNTHESIZE(int, executionCount, ExecutionCount);

public:
	WkfServiceRequestDO() : BaseDO("wkf_service_request")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, true);
		MYSQL_ADD_FIELD_NULLABLE("prescription_no", "s", prescriptionNo, true);
		MYSQL_ADD_FIELD_NULLABLE("based_on_table", "s", basedOnTable, true);
		MYSQL_ADD_FIELD_NULLABLE("based_on_id", "s", basedOnId, true);
		MYSQL_ADD_FIELD_NULLABLE("requisition_id", "s", requisitionId, true);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("intent_enum", "s", intentEnum, true);
		MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
		MYSQL_ADD_FIELD_NULLABLE("group_id", "s", groupId, true);
		MYSQL_ADD_FIELD_NULLABLE("priority_enum", "s", priorityEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("perform_flag", "b", performFlag, true);
		MYSQL_ADD_FIELD("activity_id", "s", activityId);
		MYSQL_ADD_FIELD("quantity", "d", quantity);
		MYSQL_ADD_FIELD("unit_code", "s", unitCode);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD_NULLABLE("authored_time", "dt", authoredTime, true);
		MYSQL_ADD_FIELD("requester_id", "s", requesterId);
		MYSQL_ADD_FIELD_NULLABLE("performer_type_code", "s", performerTypeCode, true);
		MYSQL_ADD_FIELD_NULLABLE("performer_id", "s", performerId, true);
		MYSQL_ADD_FIELD_NULLABLE("performer_check_id", "s", performerCheckId, true);
		MYSQL_ADD_FIELD_NULLABLE("check_time", "dt", checkTime, true);
		MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, true);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
		MYSQL_ADD_FIELD_NULLABLE("reason_text", "s", reasonText, true);
		MYSQL_ADD_FIELD_NULLABLE("contract_id", "s", contractId, true);
		MYSQL_ADD_FIELD_NULLABLE("occurrence_start_time", "dt", occurrenceStartTime, true);
		MYSQL_ADD_FIELD_NULLABLE("occurrence_end_time", "dt", occurrenceEndTime, true);
		MYSQL_ADD_FIELD_NULLABLE("relevant_history_id", "s", relevantHistoryId, true);
		MYSQL_ADD_FIELD_NULLABLE("refund_service_id", "s", refundServiceId, true);
		MYSQL_ADD_FIELD_NULLABLE("content_json", "s", contentJson, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_class_enum", "s", ybClassEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("print_count", "i", printCount, true);
		MYSQL_ADD_FIELD_NULLABLE("condition_definition_id", "s", conditionDefinitionId, true);
		MYSQL_ADD_FIELD_NULLABLE("encounter_diagnosis_id", "s", encounterDiagnosisId, true);
		MYSQL_ADD_FIELD_NULLABLE("therapy_enum", "s", therapyEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("rate_code", "s", rateCode, true);
		MYSQL_ADD_FIELD_NULLABLE("parent_id", "s", parentId, true);
		MYSQL_ADD_FIELD_NULLABLE("generate_source_enum", "s", generateSourceEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("remark", "s", remark, true);
		MYSQL_ADD_FIELD_NULLABLE("entrust_content", "s", entrustContent, true);
		MYSQL_ADD_FIELD_NULLABLE("sort_number", "i", sortNumber, true);
		MYSQL_ADD_FIELD_NULLABLE("execution_count", "i", executionCount, true);
	}
};

typedef std::shared_ptr<WkfServiceRequestDO> PtrWkfServiceRequestDO;

#endif // _WKFSERVICEREQUESTDO_H_
