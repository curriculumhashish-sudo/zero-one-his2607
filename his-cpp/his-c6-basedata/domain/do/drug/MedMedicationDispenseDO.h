#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/24

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
#ifndef _MED_MEDICATION_DISPENSE_DO_H_
#define _MED_MEDICATION_DISPENSE_DO_H_

#include "../DoInclude.h"

/**
 * Persistence model for med_medication_dispense.
 */
class MedMedicationDispenseDO : public BaseDO
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
    MedMedicationDispenseDO() : BaseDO("med_medication_dispense")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
        MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
        MYSQL_ADD_FIELD("created_by", "s", createdBy);
        MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
        MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);
        MYSQL_ADD_FIELD("deleted_at", "dt", deletedAt);
        MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
        MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
        MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, false);
        MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
        MYSQL_ADD_FIELD("not_performed_reason_enum", "s", notPerformedReasonEnum);
        MYSQL_ADD_FIELD("status_changed_time", "dt", statusChangedTime);
        MYSQL_ADD_FIELD_NULLABLE("dispense_enum", "s", dispenseEnum, false);
        MYSQL_ADD_FIELD_NULLABLE("medication_id", "s", medicationId, false);
        MYSQL_ADD_FIELD_NULLABLE("patient_id", "s", patientId, false);
        MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
        MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, false);
        MYSQL_ADD_FIELD_NULLABLE("med_req_id", "s", medReqId, false);
        MYSQL_ADD_FIELD_NULLABLE("quantity", "d", quantity, false);
        MYSQL_ADD_FIELD_NULLABLE("unit_code", "s", unitCode, false);
        MYSQL_ADD_FIELD("dispense_quantity", "d", dispenseQuantity);
        MYSQL_ADD_FIELD("lot_number", "s", lotNumber);
        MYSQL_ADD_FIELD("trace_no", "s", traceNo);
        MYSQL_ADD_FIELD("preparer_id", "s", preparerId);
        MYSQL_ADD_FIELD("prepare_time", "dt", prepareTime);
        MYSQL_ADD_FIELD("practitioner_id", "s", practitionerId);
        MYSQL_ADD_FIELD("dispense_time", "dt", dispenseTime);
        MYSQL_ADD_FIELD("limit_time", "dt", limitTime);
        MYSQL_ADD_FIELD("planned_dispense_time", "dt", plannedDispenseTime);
        MYSQL_ADD_FIELD("procedure_id", "s", procedureId);
        MYSQL_ADD_FIELD("summary_no", "s", summaryNo);
        MYSQL_ADD_FIELD("des_location_id", "s", desLocationId);
        MYSQL_ADD_FIELD("rec_practitioner_id", "s", recPractitionerId);
        MYSQL_ADD_FIELD("dosage_instruction", "s", dosageInstruction);
        MYSQL_ADD_FIELD("method_code", "s", methodCode);
        MYSQL_ADD_FIELD("frequency_code", "s", frequencyCode);
        MYSQL_ADD_FIELD("dose", "d", dose);
        MYSQL_ADD_FIELD("dose_unit_code", "s", doseUnitCode);
        MYSQL_ADD_FIELD("max_unit", "d", maxUnit);
        MYSQL_ADD_FIELD("support_info", "s", supportInfo);
        MYSQL_ADD_FIELD("part_type_code", "s", partTypeCode);
        MYSQL_ADD_FIELD("dispense_frequency_code", "s", dispenseFrequencyCode);
    }
};

using PtrMedMedicationDispenseDO = std::shared_ptr<MedMedicationDispenseDO>;

#endif // !_MED_MEDICATION_DISPENSE_DO_H_
