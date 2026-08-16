#pragma once
#ifndef _DEVICE_REQUEST_DO_
#define _DEVICE_REQUEST_DO_

#include "../DoInclude.h"

class DeviceRequestDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, activityId, ActivityId);
	MYSQL_SYNTHESIZE(string, packageId, PackageId);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, intentCode, IntentCode);
	MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);
	MYSQL_SYNTHESIZE(string, priorityEnum, PriorityEnum);
	MYSQL_SYNTHESIZE(bool, performFlag, PerformFlag);
	MYSQL_SYNTHESIZE(string, groupNo, GroupNo);
	MYSQL_SYNTHESIZE(string, deviceTypeCode, DeviceTypeCode);
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
	MYSQL_SYNTHESIZE(string, deviceDefId, DeviceDefId);
	MYSQL_SYNTHESIZE(string, deviceSpecifications, DeviceSpecifications);
	MYSQL_SYNTHESIZE(string, requesterId, RequesterId);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(long long, performLocation, PerformLocation);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, rateCode, RateCode);
	MYSQL_SYNTHESIZE(string, useTime, UseTime);
	MYSQL_SYNTHESIZE(string, useStartTime, UseStartTime);
	MYSQL_SYNTHESIZE(string, useEndTime, UseEndTime);
	MYSQL_SYNTHESIZE(string, useTiming, UseTiming);
	MYSQL_SYNTHESIZE(string, reqAuthoredTime, ReqAuthoredTime);
	MYSQL_SYNTHESIZE(string, performerEnum, PerformerEnum);
	MYSQL_SYNTHESIZE(string, performerId, PerformerId);
	MYSQL_SYNTHESIZE(string, performOrgId, PerformOrgId);
	MYSQL_SYNTHESIZE(bool, asNeedFlag, AsNeedFlag);
	MYSQL_SYNTHESIZE(string, asNeedReason, AsNeedReason);
	MYSQL_SYNTHESIZE(string, contractCode, ContractCode);
	MYSQL_SYNTHESIZE(string, refundDeviceId, RefundDeviceId);
	MYSQL_SYNTHESIZE(string, ybClassEnum, YbClassEnum);
	MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
	MYSQL_SYNTHESIZE(string, conditionDefinitionId, ConditionDefinitionId);
	MYSQL_SYNTHESIZE(string, encounterDiagnosisId, EncounterDiagnosisId);
	MYSQL_SYNTHESIZE(string, observationIdJson, ObservationIdJson);
	MYSQL_SYNTHESIZE(string, supportInfo, SupportInfo);
	MYSQL_SYNTHESIZE(string, basedOnTable, BasedOnTable);
	MYSQL_SYNTHESIZE(string, basedOnId, BasedOnId);
	MYSQL_SYNTHESIZE(string, generateSourceEnum, GenerateSourceEnum);
	MYSQL_SYNTHESIZE(string, remark, Remark);
	MYSQL_SYNTHESIZE(int, sortNumber, SortNumber);

public:
	DeviceRequestDO() : BaseDO("wkf_device_request")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, true);
		MYSQL_ADD_FIELD_NULLABLE("prescription_no", "s", prescriptionNo, true);
		MYSQL_ADD_FIELD_NULLABLE("activity_id", "s", activityId, true);
		MYSQL_ADD_FIELD_NULLABLE("package_id", "s", packageId, true);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("intent_code", "s", intentCode, true);
		MYSQL_ADD_FIELD_NULLABLE("category_enum", "s", categoryEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("priority_enum", "s", priorityEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("perform_flag", "b", performFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("group_no", "s", groupNo, true);
		MYSQL_ADD_FIELD_NULLABLE("device_type_code", "s", deviceTypeCode, true);
		MYSQL_ADD_FIELD("quantity", "d", quantity);
		MYSQL_ADD_FIELD_NULLABLE("unit_code", "s", unitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("lot_number", "s", lotNumber, true);
		MYSQL_ADD_FIELD("device_def_id", "s", deviceDefId);
		MYSQL_ADD_FIELD_NULLABLE("device_specifications", "s", deviceSpecifications, true);
		MYSQL_ADD_FIELD_NULLABLE("requester_id", "s", requesterId, true);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
		MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, true);
		MYSQL_ADD_FIELD_NULLABLE("perform_location", "ll", performLocation, true);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD_NULLABLE("rate_code", "s", rateCode, true);
		MYSQL_ADD_FIELD_NULLABLE("use_time", "s", useTime, true);
		MYSQL_ADD_FIELD_NULLABLE("use_start_time", "s", useStartTime, true);
		MYSQL_ADD_FIELD_NULLABLE("use_end_time", "s", useEndTime, true);
		MYSQL_ADD_FIELD_NULLABLE("use_timing", "s", useTiming, true);
		MYSQL_ADD_FIELD_NULLABLE("req_authored_time", "s", reqAuthoredTime, true);
		MYSQL_ADD_FIELD_NULLABLE("performer_enum", "s", performerEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("performer_id", "s", performerId, true);
		MYSQL_ADD_FIELD_NULLABLE("perform_org_id", "s", performOrgId, true);
		MYSQL_ADD_FIELD_NULLABLE("as_need_flag", "b", asNeedFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("as_need_reason", "s", asNeedReason, true);
		MYSQL_ADD_FIELD_NULLABLE("contract_code", "s", contractCode, true);
		MYSQL_ADD_FIELD_NULLABLE("refund_device_id", "s", refundDeviceId, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_class_enum", "s", ybClassEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("trace_no", "s", traceNo, true);
		MYSQL_ADD_FIELD_NULLABLE("condition_definition_id", "s", conditionDefinitionId, true);
		MYSQL_ADD_FIELD_NULLABLE("encounter_diagnosis_id", "s", encounterDiagnosisId, true);
		MYSQL_ADD_FIELD_NULLABLE("observation_id_json", "s", observationIdJson, true);
		MYSQL_ADD_FIELD_NULLABLE("support_info", "s", supportInfo, true);
		MYSQL_ADD_FIELD_NULLABLE("based_on_table", "s", basedOnTable, true);
		MYSQL_ADD_FIELD_NULLABLE("based_on_id", "s", basedOnId, true);
		MYSQL_ADD_FIELD_NULLABLE("generate_source_enum", "s", generateSourceEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("remark", "s", remark, true);
		MYSQL_ADD_FIELD_NULLABLE("sort_number", "i", sortNumber, true);
	}
};

typedef std::shared_ptr<DeviceRequestDO> PtrDeviceRequestDO;

#endif
