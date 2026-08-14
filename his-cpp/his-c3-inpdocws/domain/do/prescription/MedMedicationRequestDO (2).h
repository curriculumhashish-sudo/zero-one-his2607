#pragma once

#ifndef _MED_MEDICATION_REQUEST_DO_H_
#define _MED_MEDICATION_REQUEST_DO_H_

#include "../DoInclude.h"
#include <cstdint>

class MedMedicationRequestDO : public BaseDO
{
	// Primary key
	MYSQL_SYNTHESIZE(string, id, Id);
	// Tenant identifier
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// Creation time
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// Last update time
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// Creator identifier
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// Last updater identifier
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// Soft deletion flag
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	// Deletion time
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// Deleter identifier
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// Optimistic lock version
	MYSQL_SYNTHESIZE(int, version, Version);
	// Business number
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	// Prescription number
	MYSQL_SYNTHESIZE(string, prescriptionNo, PrescriptionNo);
	// Group identifier
	MYSQL_SYNTHESIZE(string, groupId, GroupId);
	// Package identifier
	MYSQL_SYNTHESIZE(string, packageId, PackageId);
	// Status enumeration value
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	// Status reason
	MYSQL_SYNTHESIZE(int, statusReason, StatusReason);
	// Status change time
	MYSQL_SYNTHESIZE(string, statusChangedTime, StatusChangedTime);
	// Intent enumeration value
	MYSQL_SYNTHESIZE(string, intentEnum, IntentEnum);
	// Category enumeration value
	MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);
	// Priority enumeration value
	MYSQL_SYNTHESIZE(string, priorityEnum, PriorityEnum);
	// Perform flag
	MYSQL_SYNTHESIZE(bool, performFlag, PerformFlag);
	// Medication identifier
	MYSQL_SYNTHESIZE(string, medicationId, MedicationId);
	// Patient identifier
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// Encounter identifier
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	// Requester identifier
	MYSQL_SYNTHESIZE(string, requesterId, RequesterId);
	// Location identifier
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	// Organization identifier
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	// Dispensing pharmacy location identifier
	MYSQL_SYNTHESIZE(int64_t, performLocation, PerformLocation);
	// Performing organization identifier
	MYSQL_SYNTHESIZE(int64_t, performOrg, PerformOrg);
	// Performer enumeration value
	MYSQL_SYNTHESIZE(string, performerEnum, PerformerEnum);
	// Performer identifier
	MYSQL_SYNTHESIZE(string, performerId, PerformerId);
	// Device definition identifier
	MYSQL_SYNTHESIZE(string, deviceDefId, DeviceDefId);
	// Recorder identifier
	MYSQL_SYNTHESIZE(string, recorderId, RecorderId);
	// Request authored time
	MYSQL_SYNTHESIZE(string, reqAuthoredTime, ReqAuthoredTime);
	// Report flag
	MYSQL_SYNTHESIZE(bool, reportFlag, ReportFlag);
	// Supporting information
	MYSQL_SYNTHESIZE(string, supportInfo, SupportInfo);
	// Reason JSON
	MYSQL_SYNTHESIZE(string, reasonJson, ReasonJson);
	// Therapy enumeration value
	MYSQL_SYNTHESIZE(string, therapyEnum, TherapyEnum);
	// Effective dose start time
	MYSQL_SYNTHESIZE(string, effectiveDoseStart, EffectiveDoseStart);
	// Requested quantity
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	// Execution count
	MYSQL_SYNTHESIZE(int, executeNum, ExecuteNum);
	// Unit code
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	// Administration method code
	MYSQL_SYNTHESIZE(string, methodCode, MethodCode);
	// Administration rate code
	MYSQL_SYNTHESIZE(string, rateCode, RateCode);
	// Dose quantity
	MYSQL_SYNTHESIZE(double, dose, Dose);
	// Dose unit code
	MYSQL_SYNTHESIZE(string, doseUnitCode, DoseUnitCode);
	// Maximum dose
	MYSQL_SYNTHESIZE(double, maxDose, MaxDose);
	// First dose
	MYSQL_SYNTHESIZE(double, firstDose, FirstDose);
	// First dose unit code
	MYSQL_SYNTHESIZE(string, firstDoseUnitCode, FirstDoseUnitCode);
	// First dose duration
	MYSQL_SYNTHESIZE(string, firstDuration, FirstDuration);
	// Dispensing interval
	MYSQL_SYNTHESIZE(string, dispenseInterval, DispenseInterval);
	// Quantity per dispensing event
	MYSQL_SYNTHESIZE(int, dispensePerQuantity, DispensePerQuantity);
	// Duration per dispensing event
	MYSQL_SYNTHESIZE(int, dispensePerDuration, DispensePerDuration);
	// Lot number
	MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
	// Trace number
	MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
	// Skin test flag
	MYSQL_SYNTHESIZE(bool, skinTestFlag, SkinTestFlag);
	// Infusion flag
	MYSQL_SYNTHESIZE(bool, infusionFlag, InfusionFlag);
	// Infusion speed code
	MYSQL_SYNTHESIZE(string, speedCode, SpeedCode);
	// Contract identifier
	MYSQL_SYNTHESIZE(string, contractId, ContractId);
	// Condition definition identifier
	MYSQL_SYNTHESIZE(string, conditionDefinitionId, ConditionDefinitionId);
	// Encounter diagnosis identifier
	MYSQL_SYNTHESIZE(string, encounterDiagnosisId, EncounterDiagnosisId);
	// Performer check identifier
	MYSQL_SYNTHESIZE(string, performerCheckId, PerformerCheckId);
	// Check time
	MYSQL_SYNTHESIZE(string, checkTime, CheckTime);
	// Effective dose end time
	MYSQL_SYNTHESIZE(string, effectiveDoseEnd, EffectiveDoseEnd);
	// Refund medication identifier
	MYSQL_SYNTHESIZE(string, refundMedicineId, RefundMedicineId);
	// Prescription type code
	MYSQL_SYNTHESIZE(string, rxTypeCode, RxTypeCode);
	// Medical insurance class enumeration value
	MYSQL_SYNTHESIZE(string, ybClassEnum, YbClassEnum);
	// Dosage instruction
	MYSQL_SYNTHESIZE(string, dosageInstruction, DosageInstruction);
	// Source table name
	MYSQL_SYNTHESIZE(string, basedOnTable, BasedOnTable);
	// Source record identifier
	MYSQL_SYNTHESIZE(string, basedOnId, BasedOnId);
	// Traditional Chinese medicine flag
	MYSQL_SYNTHESIZE(bool, tcmFlag, TcmFlag);
	// Content JSON
	MYSQL_SYNTHESIZE(string, contentJson, ContentJson);
	// Chinese herbs dose quantity
	MYSQL_SYNTHESIZE(double, chineseHerbsDoseQuantity, ChineseHerbsDoseQuantity);
	// Suffering flag
	MYSQL_SYNTHESIZE(bool, sufferingFlag, SufferingFlag);
	// Sort number
	MYSQL_SYNTHESIZE(int, sortNumber, SortNumber);
	// Generation source enumeration value
	MYSQL_SYNTHESIZE(string, generateSourceEnum, GenerateSourceEnum);
	// Remark
	MYSQL_SYNTHESIZE(string, remark, Remark);

public:
	MedMedicationRequestDO() : BaseDO("med_medication_request")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD("deleted_at", "dt", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("prescription_no", "s", prescriptionNo);
		MYSQL_ADD_FIELD("group_id", "s", groupId);
		MYSQL_ADD_FIELD("package_id", "s", packageId);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("status_reason", "i", statusReason);
		MYSQL_ADD_FIELD("status_changed_time", "dt", statusChangedTime);
		MYSQL_ADD_FIELD("intent_enum", "s", intentEnum);
		MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
		MYSQL_ADD_FIELD("priority_enum", "s", priorityEnum);
		MYSQL_ADD_FIELD("perform_flag", "b", performFlag);
		MYSQL_ADD_FIELD("medication_id", "s", medicationId);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("requester_id", "s", requesterId);
		MYSQL_ADD_FIELD("location_id", "s", locationId);
		MYSQL_ADD_FIELD("org_id", "s", orgId);
		MYSQL_ADD_FIELD("perform_location", "ll", performLocation);
		MYSQL_ADD_FIELD("perform_org", "ll", performOrg);
		MYSQL_ADD_FIELD("performer_enum", "s", performerEnum);
		MYSQL_ADD_FIELD("performer_id", "s", performerId);
		MYSQL_ADD_FIELD("device_def_id", "s", deviceDefId);
		MYSQL_ADD_FIELD("recorder_id", "s", recorderId);
		MYSQL_ADD_FIELD("req_authored_time", "dt", reqAuthoredTime);
		MYSQL_ADD_FIELD("report_flag", "b", reportFlag);
		MYSQL_ADD_FIELD("support_info", "s", supportInfo);
		MYSQL_ADD_FIELD("reason_json", "s", reasonJson);
		MYSQL_ADD_FIELD("therapy_enum", "s", therapyEnum);
		MYSQL_ADD_FIELD("effective_dose_start", "dt", effectiveDoseStart);
		MYSQL_ADD_FIELD("quantity", "d", quantity);
		MYSQL_ADD_FIELD("execute_num", "i", executeNum);
		MYSQL_ADD_FIELD("unit_code", "s", unitCode);
		MYSQL_ADD_FIELD("method_code", "s", methodCode);
		MYSQL_ADD_FIELD("rate_code", "s", rateCode);
		MYSQL_ADD_FIELD("dose", "d", dose);
		MYSQL_ADD_FIELD("dose_unit_code", "s", doseUnitCode);
		MYSQL_ADD_FIELD("max_dose", "d", maxDose);
		MYSQL_ADD_FIELD("first_dose", "d", firstDose);
		MYSQL_ADD_FIELD("first_dose_unit_code", "s", firstDoseUnitCode);
		MYSQL_ADD_FIELD("first_duration", "s", firstDuration);
		MYSQL_ADD_FIELD("dispense_interval", "s", dispenseInterval);
		MYSQL_ADD_FIELD("dispense_per_quantity", "i", dispensePerQuantity);
		MYSQL_ADD_FIELD("dispense_per_duration", "i", dispensePerDuration);
		MYSQL_ADD_FIELD("lot_number", "s", lotNumber);
		MYSQL_ADD_FIELD("trace_no", "s", traceNo);
		MYSQL_ADD_FIELD("skin_test_flag", "b", skinTestFlag);
		MYSQL_ADD_FIELD("infusion_flag", "b", infusionFlag);
		MYSQL_ADD_FIELD("speed_code", "s", speedCode);
		MYSQL_ADD_FIELD("contract_id", "s", contractId);
		MYSQL_ADD_FIELD("condition_definition_id", "s", conditionDefinitionId);
		MYSQL_ADD_FIELD("encounter_diagnosis_id", "s", encounterDiagnosisId);
		MYSQL_ADD_FIELD("performer_check_id", "s", performerCheckId);
		MYSQL_ADD_FIELD("check_time", "dt", checkTime);
		MYSQL_ADD_FIELD("effective_dose_end", "dt", effectiveDoseEnd);
		MYSQL_ADD_FIELD("refund_medicine_id", "s", refundMedicineId);
		MYSQL_ADD_FIELD("rx_type_code", "s", rxTypeCode);
		MYSQL_ADD_FIELD("yb_class_enum", "s", ybClassEnum);
		MYSQL_ADD_FIELD("dosage_instruction", "s", dosageInstruction);
		MYSQL_ADD_FIELD("based_on_table", "s", basedOnTable);
		MYSQL_ADD_FIELD("based_on_id", "s", basedOnId);
		MYSQL_ADD_FIELD("tcm_flag", "b", tcmFlag);
		MYSQL_ADD_FIELD("content_json", "s", contentJson);
		MYSQL_ADD_FIELD("chinese_herbs_dose_quantity", "d", chineseHerbsDoseQuantity);
		MYSQL_ADD_FIELD("suffering_flag", "b", sufferingFlag);
		MYSQL_ADD_FIELD("sort_number", "i", sortNumber);
		MYSQL_ADD_FIELD("generate_source_enum", "s", generateSourceEnum);
		MYSQL_ADD_FIELD("remark", "s", remark);
	}
};

typedef std::shared_ptr<MedMedicationRequestDO> PtrMedMedicationRequestDO;

#endif // !_MED_MEDICATION_REQUEST_DO_H_
