#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 Defines the medication request data object used by TCM prescription actions.

 @Author: nick1484065161
 @Date: 2026/07/23 00:00:00

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

#ifndef _MED_MEDICATION_REQUEST_DO_H_
#define _MED_MEDICATION_REQUEST_DO_H_

#include "../DoInclude.h"

/**
 * Maps the medication request fields required by TCM prescription actions.
 */
class MedMedicationRequestDO : public BaseDO
{
	// Medication request primary key.
	MYSQL_SYNTHESIZE(string, id, Id);
	// Tenant identifier.
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// Optimistic locking version.
	MYSQL_SYNTHESIZE(int, version, Version);
	// Prescription number shared by prescription detail records.
	MYSQL_SYNTHESIZE(string, prescriptionNo, PrescriptionNo);
	// Medication request group identifier.
	MYSQL_SYNTHESIZE(string, groupId, GroupId);
	// Current medication request status.
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	// Numeric reason associated with the current status.
	MYSQL_SYNTHESIZE(int, statusReason, StatusReason);
	// Time when the status last changed.
	MYSQL_SYNTHESIZE(string, statusChangedTime, StatusChangedTime);
	// Patient identifier.
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// Encounter identifier.
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	// Planned or actual end time of the medication request.
	MYSQL_SYNTHESIZE(string, effectiveDoseEnd, EffectiveDoseEnd);
	// Therapy type used to distinguish long-term and temporary orders.
	MYSQL_SYNTHESIZE(string, therapyEnum, TherapyEnum);
	// Indicates whether the request is a TCM medication request.
	MYSQL_SYNTHESIZE(int, tcmFlag, TcmFlag);
	// Extended prescription content stored as JSON text.
	MYSQL_SYNTHESIZE(string, contentJson, ContentJson);
	// Soft deletion flag.
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	// Identifier of the last operator.
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// Time of the last update.
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);

	// Derived from contentJson and not mapped to a physical database column.
	MYSQL_SYNTHESIZE(string, prescriptionGroupKey, PrescriptionGroupKey);

public:
	MedMedicationRequestDO()
		: BaseDO("med_medication_request")
	{
		// Register physical database columns for ORM mapping.
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("prescription_no", "s", prescriptionNo);
		MYSQL_ADD_FIELD("group_id", "s", groupId);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("status_reason", "i", statusReason);
		MYSQL_ADD_FIELD("status_changed_time", "s", statusChangedTime);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("effective_dose_end", "s", effectiveDoseEnd);
		MYSQL_ADD_FIELD("therapy_enum", "s", therapyEnum);
		MYSQL_ADD_FIELD("tcm_flag", "i", tcmFlag);
		MYSQL_ADD_FIELD("content_json", "s", contentJson);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
	}
};

typedef std::shared_ptr<MedMedicationRequestDO> PtrMedMedicationRequestDO;

#endif // !_MED_MEDICATION_REQUEST_DO_H_
