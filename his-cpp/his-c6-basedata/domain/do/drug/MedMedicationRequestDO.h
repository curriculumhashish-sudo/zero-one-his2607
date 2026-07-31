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
 See the License for the specific language governing permissions 
 limitations under the License.
*/
#ifndef _MED_MEDICATION_REQUEST_DO_H_
#define _MED_MEDICATION_REQUEST_DO_H_

#include "../DoInclude.h"
#include <cstdint>

/**
 * Persistence model for med_medication_request.
 */
class MedMedicationRequestDO : public BaseDO
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
    MYSQL_SYNTHESIZE(string, groupId, GroupId);
    MYSQL_SYNTHESIZE(string, packageId, PackageId);
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(int, statusReason, StatusReason);
    MYSQL_SYNTHESIZE(string, statusChangedTime, StatusChangedTime);
    MYSQL_SYNTHESIZE(string, intentEnum, IntentEnum);
    MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);
    MYSQL_SYNTHESIZE(string, priorityEnum, PriorityEnum);
    MYSQL_SYNTHESIZE(bool, performFlag, PerformFlag);
    MYSQL_SYNTHESIZE(string, medicationId, MedicationId);
    MYSQL_SYNTHESIZE(string, patientId, PatientId);
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    MYSQL_SYNTHESIZE(string, requesterId, RequesterId);
    MYSQL_SYNTHESIZE(string, locationId, LocationId);
    MYSQL_SYNTHESIZE(string, orgId, OrgId);
    MYSQL_SYNTHESIZE(int64_t, performLocation, PerformLocation);
    MYSQL_SYNTHESIZE(int64_t, performOrg, PerformOrg);
    MYSQL_SYNTHESIZE(string, performerEnum, PerformerEnum);
    MYSQL_SYNTHESIZE(string, performerId, PerformerId);
    MYSQL_SYNTHESIZE(string, deviceDefId, DeviceDefId);
    MYSQL_SYNTHESIZE(string, recorderId, RecorderId);
    MYSQL_SYNTHESIZE(string, reqAuthoredTime, ReqAuthoredTime);
    MYSQL_SYNTHESIZE(bool, reportFlag, ReportFlag);
    MYSQL_SYNTHESIZE(string, supportInfo, SupportInfo);
    MYSQL_SYNTHESIZE(string, reasonJson, ReasonJson);
    MYSQL_SYNTHESIZE(string, therapyEnum, TherapyEnum);
    MYSQL_SYNTHESIZE(string, effectiveDoseStart, EffectiveDoseStart);
    MYSQL_SYNTHESIZE(double, quantity, Quantity);
    MYSQL_SYNTHESIZE(int, executeNum, ExecuteNum);
    MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
    MYSQL_SYNTHESIZE(string, methodCode, MethodCode);
    MYSQL_SYNTHESIZE(string, rateCode, RateCode);
    MYSQL_SYNTHESIZE(double, dose, Dose);
    MYSQL_SYNTHESIZE(string, doseUnitCode, DoseUnitCode);
    MYSQL_SYNTHESIZE(double, maxDose, MaxDose);
    MYSQL_SYNTHESIZE(double, firstDose, FirstDose);
    MYSQL_SYNTHESIZE(string, firstDoseUnitCode, FirstDoseUnitCode);
    MYSQL_SYNTHESIZE(string, firstDuration, FirstDuration);
    MYSQL_SYNTHESIZE(string, dispenseInterval, DispenseInterval);
    MYSQL_SYNTHESIZE(int, dispensePerQuantity, DispensePerQuantity);
    MYSQL_SYNTHESIZE(int, dispensePerDuration, DispensePerDuration);
    MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
    MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
    MYSQL_SYNTHESIZE(bool, skinTestFlag, SkinTestFlag);
    MYSQL_SYNTHESIZE(bool, infusionFlag, InfusionFlag);
    MYSQL_SYNTHESIZE(string, speedCode, SpeedCode);
    MYSQL_SYNTHESIZE(string, contractId, ContractId);
    MYSQL_SYNTHESIZE(string, conditionDefinitionId, ConditionDefinitionId);
    MYSQL_SYNTHESIZE(string, encounterDiagnosisId, EncounterDiagnosisId);
    MYSQL_SYNTHESIZE(string, performerCheckId, PerformerCheckId);
    MYSQL_SYNTHESIZE(string, checkTime, CheckTime);
    MYSQL_SYNTHESIZE(string, effectiveDoseEnd, EffectiveDoseEnd);
    MYSQL_SYNTHESIZE(string, refundMedicineId, RefundMedicineId);
    MYSQL_SYNTHESIZE(string, rxTypeCode, RxTypeCode);
    MYSQL_SYNTHESIZE(string, ybClassEnum, YbClassEnum);
    MYSQL_SYNTHESIZE(string, dosageInstruction, DosageInstruction);
    MYSQL_SYNTHESIZE(string, basedOnTable, BasedOnTable);
    MYSQL_SYNTHESIZE(string, basedOnId, BasedOnId);
    MYSQL_SYNTHESIZE(bool, tcmFlag, TcmFlag);
    MYSQL_SYNTHESIZE(string, contentJson, ContentJson);
    MYSQL_SYNTHESIZE(double, chineseHerbsDoseQuantity, ChineseHerbsDoseQuantity);
    MYSQL_SYNTHESIZE(bool, sufferingFlag, SufferingFlag);
    MYSQL_SYNTHESIZE(int, sortNumber, SortNumber);
    MYSQL_SYNTHESIZE(string, generateSourceEnum, GenerateSourceEnum);
    MYSQL_SYNTHESIZE(string, remark, Remark);

  public:
    MedMedicationRequestDO() : BaseDO("med_medication_request")
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
        MYSQL_ADD_FIELD("prescription_no", "s", prescriptionNo);
        MYSQL_ADD_FIELD("group_id", "s", groupId);
        MYSQL_ADD_FIELD("package_id", "s", packageId);
        MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
        MYSQL_ADD_FIELD("status_reason", "i", statusReason);
        MYSQL_ADD_FIELD("status_changed_time", "dt", statusChangedTime);
        MYSQL_ADD_FIELD("intent_enum", "s", intentEnum);
        MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
        MYSQL_ADD_FIELD("priority_enum", "s", priorityEnum);
        MYSQL_ADD_FIELD("perform_flag", "b", performFlag);
        MYSQL_ADD_FIELD_NULLABLE("medication_id", "s", medicationId, false);
        MYSQL_ADD_FIELD_NULLABLE("patient_id", "s", patientId, false);
        MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
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
        MYSQL_ADD_FIELD_NULLABLE("therapy_enum", "s", therapyEnum, false);
        MYSQL_ADD_FIELD("effective_dose_start", "dt", effectiveDoseStart);
        MYSQL_ADD_FIELD_NULLABLE("quantity", "d", quantity, false);
        MYSQL_ADD_FIELD("execute_num", "i", executeNum);
        MYSQL_ADD_FIELD_NULLABLE("unit_code", "s", unitCode, false);
        MYSQL_ADD_FIELD_NULLABLE("method_code", "s", methodCode, false);
        MYSQL_ADD_FIELD_NULLABLE("rate_code", "s", rateCode, false);
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

using PtrMedMedicationRequestDO = std::shared_ptr<MedMedicationRequestDO>;

#endif // !_MED_MEDICATION_REQUEST_DO_H_
