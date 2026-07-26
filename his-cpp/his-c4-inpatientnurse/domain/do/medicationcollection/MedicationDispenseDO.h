#pragma once
#ifndef _MEDICATIONDISPENSE_DO_
#define _MEDICATIONDISPENSE_DO_
#include "../DoInclude.h"

class MedicationDispenseDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, notPerformedReasonEnum, NotPerformedReasonEnum);
	MYSQL_SYNTHESIZE(string, statusChangedTime, StatusChangedTime);
	MYSQL_SYNTHESIZE(string, dispenseEnum, DispenseEnum);
	MYSQL_SYNTHESIZE(string, medicationId, MedicationId);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(string, medReqId, MedReqId);
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	MYSQL_SYNTHESIZE(double, dispenseQuantity, DispenseQuantity);
	MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
	MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
	MYSQL_SYNTHESIZE(string, preparerId, PreparerId);
	MYSQL_SYNTHESIZE(string, prepareTime, PrepareTime);
	MYSQL_SYNTHESIZE(string, practitionerId, PractitionerId);
	MYSQL_SYNTHESIZE(string, dispenseTime, DispenseTime);
	MYSQL_SYNTHESIZE(string, limitTime, LimitTime);
	MYSQL_SYNTHESIZE(string, plannedDispenseTime, PlannedDispenseTime);
	MYSQL_SYNTHESIZE(string, procedureId, ProcedureId);
	MYSQL_SYNTHESIZE(string, summaryNo, SummaryNo);
	MYSQL_SYNTHESIZE(string, desLocationId, DesLocationId);
	MYSQL_SYNTHESIZE(string, recPractitionerId, RecPractitionerId);
	MYSQL_SYNTHESIZE(string, dosageInstruction, DosageInstruction);
	MYSQL_SYNTHESIZE(string, methodCode, MethodCode);
	MYSQL_SYNTHESIZE(string, frequencyCode, FrequencyCode);
	MYSQL_SYNTHESIZE(double, dose, Dose);
	MYSQL_SYNTHESIZE(string, doseUnitCode, DoseUnitCode);
	MYSQL_SYNTHESIZE(double, maxUnit, MaxUnit);
	MYSQL_SYNTHESIZE(string, supportInfo, SupportInfo);
	MYSQL_SYNTHESIZE(string, partTypeCode, PartTypeCode);
	MYSQL_SYNTHESIZE(string, dispenseFrequencyCode, DispenseFrequencyCode);

public:
	MedicationDispenseDO() : BaseDO("med_medication_dispense")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("not_performed_reason_enum", "s", notPerformedReasonEnum);
		MYSQL_ADD_FIELD_NULLABLE("status_changed_time", "s", statusChangedTime);
		MYSQL_ADD_FIELD("dispense_enum", "s", dispenseEnum);
		MYSQL_ADD_FIELD("medication_id", "s", medicationId);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("location_id", "s", locationId);
		MYSQL_ADD_FIELD("med_req_id", "s", medReqId);
		MYSQL_ADD_FIELD("quantity", "d", quantity);
		MYSQL_ADD_FIELD("unit_code", "s", unitCode);
		MYSQL_ADD_FIELD_NULLABLE("dispense_quantity", "d", dispenseQuantity);
		MYSQL_ADD_FIELD_NULLABLE("lot_number", "s", lotNumber);
		MYSQL_ADD_FIELD_NULLABLE("trace_no", "s", traceNo);
		MYSQL_ADD_FIELD_NULLABLE("preparer_id", "s", preparerId);
		MYSQL_ADD_FIELD_NULLABLE("prepare_time", "s", prepareTime);
		MYSQL_ADD_FIELD_NULLABLE("practitioner_id", "s", practitionerId);
		MYSQL_ADD_FIELD_NULLABLE("dispense_time", "s", dispenseTime);
		MYSQL_ADD_FIELD_NULLABLE("limit_time", "s", limitTime);
		MYSQL_ADD_FIELD_NULLABLE("planned_dispense_time", "s", plannedDispenseTime);
		MYSQL_ADD_FIELD_NULLABLE("procedure_id", "s", procedureId);
		MYSQL_ADD_FIELD_NULLABLE("summary_no", "s", summaryNo);
		MYSQL_ADD_FIELD_NULLABLE("des_location_id", "s", desLocationId);
		MYSQL_ADD_FIELD_NULLABLE("rec_practitioner_id", "s", recPractitionerId);
		MYSQL_ADD_FIELD_NULLABLE("dosage_instruction", "s", dosageInstruction);
		MYSQL_ADD_FIELD_NULLABLE("method_code", "s", methodCode);
		MYSQL_ADD_FIELD_NULLABLE("frequency_code", "s", frequencyCode);
		MYSQL_ADD_FIELD_NULLABLE("dose", "d", dose);
		MYSQL_ADD_FIELD_NULLABLE("dose_unit_code", "s", doseUnitCode);
		MYSQL_ADD_FIELD_NULLABLE("max_unit", "d", maxUnit);
		MYSQL_ADD_FIELD_NULLABLE("support_info", "s", supportInfo);
		MYSQL_ADD_FIELD_NULLABLE("part_type_code", "s", partTypeCode);
		MYSQL_ADD_FIELD_NULLABLE("dispense_frequency_code", "s", dispenseFrequencyCode);
	}
};

typedef std::shared_ptr<MedicationDispenseDO> PtrMedicationDispenseDO;
#endif // !_MEDICATIONDISPENSE_DO_
