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
#ifndef _DEVICEDISPENSEDO_H_
#define _DEVICEDISPENSEDO_H_

#include "../DoInclude.h"

// Complete mapping for database table wkf_device_dispense.
class DeviceDispenseDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, deviceReqId, DeviceReqId);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, basedOnTable, BasedOnTable);
	MYSQL_SYNTHESIZE(string, basedOnId, BasedOnId);
	MYSQL_SYNTHESIZE(string, notPerformedReasonEnum, NotPerformedReasonEnum);
	MYSQL_SYNTHESIZE(string, dispenseCategoryEnum, DispenseCategoryEnum);
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
	MYSQL_SYNTHESIZE(string, deviceDefId, DeviceDefId);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, performerId, PerformerId);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(string, partTypeCode, PartTypeCode);
	MYSQL_SYNTHESIZE(double, dispenseQuantity, DispenseQuantity);
	MYSQL_SYNTHESIZE(string, preparerId, PreparerId);
	MYSQL_SYNTHESIZE(string, prepareTime, PrepareTime);
	MYSQL_SYNTHESIZE(string, dispenseTime, DispenseTime);
	MYSQL_SYNTHESIZE(string, limitTime, LimitTime);
	MYSQL_SYNTHESIZE(string, plannedDispenseTime, PlannedDispenseTime);
	MYSQL_SYNTHESIZE(string, desLocationId, DesLocationId);
	MYSQL_SYNTHESIZE(string, recPractitionerId, RecPractitionerId);
	MYSQL_SYNTHESIZE(string, supportInfo, SupportInfo);
	MYSQL_SYNTHESIZE(string, usageInstruction, UsageInstruction);
	MYSQL_SYNTHESIZE(string, dispenseFrequencyCode, DispenseFrequencyCode);
	MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
	MYSQL_SYNTHESIZE(string, procedureId, ProcedureId);

public:
	DeviceDispenseDO() : BaseDO("wkf_device_dispense")
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
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("device_req_id", "s", deviceReqId);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("based_on_table", "s", basedOnTable, true);
		MYSQL_ADD_FIELD_NULLABLE("based_on_id", "s", basedOnId, true);
		MYSQL_ADD_FIELD_NULLABLE("not_performed_reason_enum", "s", notPerformedReasonEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("dispense_category_enum", "s", dispenseCategoryEnum, true);
		MYSQL_ADD_FIELD("quantity", "d", quantity);
		MYSQL_ADD_FIELD("unit_code", "s", unitCode);
		MYSQL_ADD_FIELD_NULLABLE("lot_number", "s", lotNumber, true);
		MYSQL_ADD_FIELD("device_def_id", "s", deviceDefId);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD_NULLABLE("performer_id", "s", performerId, true);
		MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, true);
		MYSQL_ADD_FIELD_NULLABLE("part_type_code", "s", partTypeCode, true);
		MYSQL_ADD_FIELD_NULLABLE("dispense_quantity", "d", dispenseQuantity, true);
		MYSQL_ADD_FIELD_NULLABLE("preparer_id", "s", preparerId, true);
		MYSQL_ADD_FIELD_NULLABLE("prepare_time", "dt", prepareTime, true);
		MYSQL_ADD_FIELD_NULLABLE("dispense_time", "dt", dispenseTime, true);
		MYSQL_ADD_FIELD_NULLABLE("limit_time", "dt", limitTime, true);
		MYSQL_ADD_FIELD_NULLABLE("planned_dispense_time", "dt", plannedDispenseTime, true);
		MYSQL_ADD_FIELD_NULLABLE("des_location_id", "s", desLocationId, true);
		MYSQL_ADD_FIELD_NULLABLE("rec_practitioner_id", "s", recPractitionerId, true);
		MYSQL_ADD_FIELD_NULLABLE("support_info", "s", supportInfo, true);
		MYSQL_ADD_FIELD_NULLABLE("usage_instruction", "s", usageInstruction, true);
		MYSQL_ADD_FIELD_NULLABLE("dispense_frequency_code", "s", dispenseFrequencyCode, true);
		MYSQL_ADD_FIELD_NULLABLE("trace_no", "s", traceNo, true);
		MYSQL_ADD_FIELD_NULLABLE("procedure_id", "s", procedureId, true);
	}
};

typedef std::shared_ptr<DeviceDispenseDO> PtrDeviceDispenseDO;

#endif // _DEVICEDISPENSEDO_H_
