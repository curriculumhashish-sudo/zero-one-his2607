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
#ifndef _CHARGEITEMDO_H_
#define _CHARGEITEMDO_H_

#include "../DoInclude.h"

// Complete mapping for database table adm_charge_item.
class ChargeItemDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, generateSourceEnum, GenerateSourceEnum);
	MYSQL_SYNTHESIZE(string, contextEnum, ContextEnum);
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, parentId, ParentId);
	MYSQL_SYNTHESIZE(string, prescriptionNo, PrescriptionNo);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, serviceTable, ServiceTable);
	MYSQL_SYNTHESIZE(string, serviceId, ServiceId);
	MYSQL_SYNTHESIZE(string, accountId, AccountId);
	MYSQL_SYNTHESIZE(string, definitionId, DefinitionId);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(double, totalPrice, TotalPrice);
	MYSQL_SYNTHESIZE(string, occurrenceTime, OccurrenceTime);
	MYSQL_SYNTHESIZE(string, performerId, PerformerId);
	MYSQL_SYNTHESIZE(string, performingOrgId, PerformingOrgId);
	MYSQL_SYNTHESIZE(string, requestingOrgId, RequestingOrgId);
	MYSQL_SYNTHESIZE(string, costOrgId, CostOrgId);
	MYSQL_SYNTHESIZE(double, quantityValue, QuantityValue);
	MYSQL_SYNTHESIZE(string, quantityUnit, QuantityUnit);
	MYSQL_SYNTHESIZE(double, unitPrice, UnitPrice);
	MYSQL_SYNTHESIZE(string, defDetailId, DefDetailId);
	MYSQL_SYNTHESIZE(double, baseAmount, BaseAmount);
	MYSQL_SYNTHESIZE(double, discountAmount, DiscountAmount);
	MYSQL_SYNTHESIZE(double, surchargeAmount, SurchargeAmount);
	MYSQL_SYNTHESIZE(double, systemDiscountPrice, SystemDiscountPrice);
	MYSQL_SYNTHESIZE(double, manualAdjustedPrice, ManualAdjustedPrice);
	MYSQL_SYNTHESIZE(string, overrideReasonCode, OverrideReasonCode);
	MYSQL_SYNTHESIZE(string, overrideReasonText, OverrideReasonText);
	MYSQL_SYNTHESIZE(string, entererId, EntererId);
	MYSQL_SYNTHESIZE(string, enteredDate, EnteredDate);
	MYSQL_SYNTHESIZE(string, productTable, ProductTable);
	MYSQL_SYNTHESIZE(string, productId, ProductId);
	MYSQL_SYNTHESIZE(string, procedureId, ProcedureId);
	MYSQL_SYNTHESIZE(string, dispenseTable, DispenseTable);
	MYSQL_SYNTHESIZE(string, dispenseId, DispenseId);
	MYSQL_SYNTHESIZE(int, claimStateEnum, ClaimStateEnum);
	MYSQL_SYNTHESIZE(int, printCount, PrintCount);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(string, refundId, RefundId);
	MYSQL_SYNTHESIZE(string, conditionId, ConditionId);
	MYSQL_SYNTHESIZE(string, encounterDiagnosisId, EncounterDiagnosisId);
	MYSQL_SYNTHESIZE(bool, tcmFlag, TcmFlag);

public:
	ChargeItemDO() : BaseDO("adm_charge_item")
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
		MYSQL_ADD_FIELD_NULLABLE("generate_source_enum", "s", generateSourceEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("context_enum", "s", contextEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, true);
		MYSQL_ADD_FIELD_NULLABLE("parent_id", "s", parentId, true);
		MYSQL_ADD_FIELD_NULLABLE("prescription_no", "s", prescriptionNo, true);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD_NULLABLE("service_table", "s", serviceTable, true);
		MYSQL_ADD_FIELD_NULLABLE("service_id", "s", serviceId, true);
		MYSQL_ADD_FIELD_NULLABLE("account_id", "s", accountId, true);
		MYSQL_ADD_FIELD_NULLABLE("definition_id", "s", definitionId, true);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("total_price", "d", totalPrice, true);
		MYSQL_ADD_FIELD_NULLABLE("occurrence_time", "dt", occurrenceTime, true);
		MYSQL_ADD_FIELD_NULLABLE("performer_id", "s", performerId, true);
		MYSQL_ADD_FIELD_NULLABLE("performing_org_id", "s", performingOrgId, true);
		MYSQL_ADD_FIELD_NULLABLE("requesting_org_id", "s", requestingOrgId, true);
		MYSQL_ADD_FIELD_NULLABLE("cost_org_id", "s", costOrgId, true);
		MYSQL_ADD_FIELD_NULLABLE("quantity_value", "d", quantityValue, true);
		MYSQL_ADD_FIELD_NULLABLE("quantity_unit", "s", quantityUnit, true);
		MYSQL_ADD_FIELD_NULLABLE("unit_price", "d", unitPrice, true);
		MYSQL_ADD_FIELD_NULLABLE("def_detail_id", "s", defDetailId, true);
		MYSQL_ADD_FIELD_NULLABLE("base_amount", "d", baseAmount, true);
		MYSQL_ADD_FIELD_NULLABLE("discount_amount", "d", discountAmount, true);
		MYSQL_ADD_FIELD_NULLABLE("surcharge_amount", "d", surchargeAmount, true);
		MYSQL_ADD_FIELD_NULLABLE("system_discount_price", "d", systemDiscountPrice, true);
		MYSQL_ADD_FIELD_NULLABLE("manual_adjusted_price", "d", manualAdjustedPrice, true);
		MYSQL_ADD_FIELD_NULLABLE("override_reason_code", "s", overrideReasonCode, true);
		MYSQL_ADD_FIELD_NULLABLE("override_reason_text", "s", overrideReasonText, true);
		MYSQL_ADD_FIELD_NULLABLE("enterer_id", "s", entererId, true);
		MYSQL_ADD_FIELD_NULLABLE("entered_date", "dt", enteredDate, true);
		MYSQL_ADD_FIELD_NULLABLE("product_table", "s", productTable, true);
		MYSQL_ADD_FIELD_NULLABLE("product_id", "s", productId, true);
		MYSQL_ADD_FIELD_NULLABLE("procedure_id", "s", procedureId, true);
		MYSQL_ADD_FIELD_NULLABLE("dispense_table", "s", dispenseTable, true);
		MYSQL_ADD_FIELD_NULLABLE("dispense_id", "s", dispenseId, true);
		MYSQL_ADD_FIELD_NULLABLE("claim_state_enum", "i", claimStateEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("print_count", "i", printCount, true);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
		MYSQL_ADD_FIELD_NULLABLE("refund_id", "s", refundId, true);
		MYSQL_ADD_FIELD_NULLABLE("condition_id", "s", conditionId, true);
		MYSQL_ADD_FIELD_NULLABLE("encounter_diagnosis_id", "s", encounterDiagnosisId, true);
		MYSQL_ADD_FIELD_NULLABLE("tcm_flag", "b", tcmFlag, true);
	}
};

typedef std::shared_ptr<ChargeItemDO> PtrChargeItemDO;

#endif // _CHARGEITEMDO_H_
